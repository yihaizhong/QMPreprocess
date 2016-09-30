#include "imagemodule.h"
#include <QFileInfo>
#include <QDir>
#include <QDomDocument>
#include <QTextStream>
#include "sensorinfo.h"
#include "sensorinfodb.h"
#include "exiv2/exiv2.hpp"

namespace qm{


ImageModule::ImageModule() :image_file_("")
{
	sensor_info_ = new SensorInfo;
}

ImageModule::~ImageModule()
{
	if (sensor_info_)
		delete sensor_info_;
}

void ImageModule::set_image_file(QString &image_file)
{
	image_file_ = image_file;
}

SensorInfo* ImageModule::get_sensor_info() const
{
	return sensor_info_;
}

void ImageModule::set_sensor_info(SensorInfo *sensor_info)
{
	*sensor_info_ = *sensor_info;
}

void ImageModule::setHorizontal()
{
	using namespace Exiv2;
	try
	{
		Image::AutoPtr image = ImageFactory::open(image_file_.toLocal8Bit().constData());
		image->readMetadata();
		//image->
		Exiv2::ExifData &exifData = image->exifData();
		auto rel = exifData.findKey(Exiv2::ExifKey("Exif.Image.Orientation"));
		QFileInfo file_info(image_file_);
		QString dir(file_info.absolutePath());
		QString bk_dir = dir + "/bk";
		QString file_name(file_info.fileName());
		if (rel != exifData.end())
		{
			if (rel->getValue()->toLong() != 1)
			{
				QDir().mkdir(bk_dir);
				QFile::copy(image_file_, bk_dir + "/" + file_name);
				exifData["Exif.Image.Orientation"].setValue("1");
				image->writeMetadata();
			}
		}
	}
	catch (...)
	{
		throw QString("can not open the image file: " + image_file_);
	}

}

bool ImageModule::checkSensorInfo()
{
	sensor_info_->readExifInfo(image_file_);
	if (sensor_info_->get_focal_length() < 0 || 
		sensor_info_->get_camera_type().isEmpty()||
		!SensorInfoDB::instance()->isInDB(sensor_info_))
		return false;
	else
		return true;
}

void ImageModule::writeInfoFile()
{
	QFileInfo file_info(image_file_);
	QString work_dir(file_info.absolutePath());
	QString file_name(file_info.fileName());
	work_dir.append("/Tmp-MM-Dir");
	QDir().mkdir(work_dir);
	QString xmlfile_name = file_name + "-MDT-4227.xml";
	xmlfile_name.push_front(work_dir + "/");
	QFile xml_file(xmlfile_name);
	if (!xml_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		throw QString("can not create or open the file:" + xmlfile_name);
	QDomDocument doc;
	auto instruction = doc.createProcessingInstruction("xml", "version=\"1.0\"");
	doc.appendChild(instruction);
	QDomElement root = doc.createElement("XmlXifInfo");

	QDomElement hg = doc.createElement("HGrev");
	hg.appendChild(doc.createTextNode("-1"));
	root.appendChild(hg);

	QDomElement foc = doc.createElement("FocMM");
	foc.appendChild(doc.createTextNode(QString::number(sensor_info_->get_focal_length())));
	root.appendChild(foc);

	//QDomElement foc35 = doc.createElement("Foc35");
	//foc35.appendChild(doc.createTextNode(QString::number(focal_length35_)));
	//root.appendChild(foc35);

	QDomElement sz = doc.createElement("Sz");
	sz.appendChild(doc.createTextNode(QString::number(sensor_info_->get_width()) 
		+ " " + QString::number(sensor_info_->get_height())));
	root.appendChild(sz);

	QDomElement cam = doc.createElement("Cam");
	cam.appendChild(doc.createTextNode(sensor_info_->get_camera_type()));
	root.appendChild(cam);

	QDomElement bay_pat = doc.createElement("BayPat");
	bay_pat.appendChild(doc.createTextNode(sensor_info_->get_bay_pat()));
	root.appendChild(bay_pat);

	QDomElement orientation = doc.createElement("Orientation");
	orientation.appendChild(doc.createTextNode(sensor_info_->get_orientation()));
	root.appendChild(orientation);

	doc.appendChild(root);

	QTextStream steam(&xml_file);
	doc.save(steam, 4, doc.EncodingFromTextStream);
	xml_file.close();
}

void ImageModule::enhance()
{
	//not implement now
}

}