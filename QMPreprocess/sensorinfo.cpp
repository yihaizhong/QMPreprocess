#include "sensorinfo.h"

#include <QFile>
#include <QTextStream>
#include <QDomDocument>

#include "opencv2/opencv.hpp"
#include "exiv2/exiv2.hpp"
#include "sensorinfodb.h"
#include "sensorinfodb.h"
#include "QFileInfo"

namespace qm{


SensorInfo::SensorInfo()
{
	camera_type_ = "custom";
	bay_pat_ = "RVWB";
	orientation_ = "Horizontal (normal)";
	focal_length_ = -1;
	focal_length35_ = -1;
	width_ = -1;
	height_ = -1;
	width_mm_ = -1;
	height_mm_ = -1;
}

SensorInfo::SensorInfo(const SensorInfo& sensor_info)
{
	camera_type_ = sensor_info.camera_type_;
	bay_pat_ = sensor_info.bay_pat_;
	orientation_ = sensor_info.orientation_;
	focal_length_ = sensor_info.focal_length_;
	focal_length35_ = sensor_info.focal_length35_;
	width_ = sensor_info.width_;
	height_ = sensor_info.height_;
	width_mm_ = sensor_info.width_mm_;
	height_mm_ = sensor_info.height_mm_;
}

SensorInfo& SensorInfo::operator=(const SensorInfo& sensor_info)
{
	camera_type_ = sensor_info.camera_type_;
	bay_pat_ = sensor_info.bay_pat_;
	orientation_ = sensor_info.orientation_;
	focal_length_ = sensor_info.focal_length_;
	focal_length35_ = sensor_info.focal_length35_;
	width_ = sensor_info.width_;
	height_ = sensor_info.height_;
	width_mm_ = sensor_info.width_mm_;
	height_mm_ = sensor_info.height_mm_;
	return *this;
}

SensorInfo::~SensorInfo()
{

}


QString SensorInfo::get_camera_type() const
{
	return camera_type_;
}

void SensorInfo::set_camera_type(QString &camera_type)
{
	camera_type_ = camera_type;
}

double SensorInfo::get_focal_length() const
{
	return focal_length_;
}

void SensorInfo::set_focal_length(double focal_length)
{
	focal_length_ = focal_length;
}

double SensorInfo::get_height_mm() const
{
	return height_mm_;
}

void SensorInfo::set_height_mm(double height_mm)
{
	height_mm_ = height_mm;
}

void SensorInfo::readExifInfo(QString &image_file)
{
	using namespace Exiv2;

	try
	{
		Image::AutoPtr image = ImageFactory::open(image_file.toLocal8Bit().constData());
		image->readMetadata();
		Exiv2::ExifData &exifData = image->exifData();
		auto rel = exifData.findKey(Exiv2::ExifKey("Exif.Image.Model"));
		if (rel != exifData.end())
		{
			camera_type_ = QString::fromStdString(rel->getValue()->toString());
		}
		else
		{
			camera_type_ = "";
		}

		bay_pat_ = "RVWB";

		rel = exifData.findKey(Exiv2::ExifKey("Exif.Image.Orientation"));
		if (rel != exifData.end())
		{
			if (rel->getValue()->toLong() != 1)
				throw QString("not set the image horizontal! ");
		}
		orientation_ = "Horizontal (normal)";

		rel = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLength"));
		if (rel != exifData.end())
		{
			focal_length_ = rel->getValue()->toFloat();
		}
		else
		{
			focal_length_ = -1;
		}

		//rel = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLengthIn35mmFilm"));
		//if (rel != exifData.end())
		//{
		//	focal_length35_ = rel->getValue()->toFloat();
		//}
		//else
		//{
		//	focal_length35_ = focal_length_;
		//}
		rel = exifData.findKey(Exiv2::ExifKey("Exif.Photo.PixelXDimension"));
		if (rel != exifData.end())
		{
			width_ = exifData["Exif.Photo.PixelXDimension"].getValue()->toLong();
			height_ = exifData["Exif.Photo.PixelYDimension"].getValue()->toLong();
		}
		else
		{
			cv::Mat img = cv::imread(image_file.toStdString());
			width_ = img.cols;
			height_ = img.rows;
		}


		//find if the sensor data base contains the information
		//SensorInfoDB::instance()->isInDB(this);
	}
	catch (...)
	{
		throw QString("can not open the file : " + image_file);
	}
}

void SensorInfo::writeToFile(QString &info_file)
{
	QFile sensor_file(info_file);
	if (!sensor_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		throw QString("can not create or open the file->" + info_file);
	QDomDocument doc;
	auto instruction = doc.createProcessingInstruction("xml", "version=\"1.0\"");
	doc.appendChild(instruction);
	QDomElement root = doc.createElement("ChantierDescripteur");
	QDomElement local_data = doc.createElement("LocCamDataBase");
	QDomElement camera_entry = doc.createElement("CameraEntry");

	QDomElement name_node = doc.createElement("Name");
	QDomText name = doc.createTextNode(camera_type_);
	name_node.appendChild(name);
	camera_entry.appendChild(name_node);

	QDomElement szcapt_node = doc.createElement("SzCaptMm");
	auto szcapt = doc.createTextNode(QString::number(width_mm_) +
		" " + QString::number(height_mm_));
	szcapt_node.appendChild(szcapt);
	camera_entry.appendChild(szcapt_node);

	QDomElement shortname_node = doc.createElement("ShortName");
	QDomText shortname = doc.createTextNode(camera_type_);
	shortname_node.appendChild(shortname);
	camera_entry.appendChild(shortname_node);

	local_data.appendChild(camera_entry);
	root.appendChild(local_data);

	doc.appendChild(root);
	QTextStream stream(&sensor_file);
	doc.save(stream, 4, doc.EncodingFromTextStream);
	sensor_file.close();

	//this is for calculating the resolution when there is no pos 
	/*QFile camera_file(QFileInfo(info_file).absolutePath() + "camera.xml");
	if (!camera_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		throw QString("can not create or open the file->" + camera_file.fileName());

	QDomDocument camera_doc;
	instruction = camera_doc.createProcessingInstruction("xml", "version=\"1.0\"");
	camera_doc.appendChild(instruction);
	root = camera_doc.createElement("Camera");
	camera_doc.appendChild(root);
	root.appendChild(name_node);
	QDomElement pr_node = camera_doc.createElement("PixelResolution");
	auto pr = camera_doc.createTextNode(QString::number(width_mm_ / width_));
	root.appendChild(pr_node);
	QDomElement fl_node = camera_doc.createElement("FocalLenthMM");
	auto fl = camera_doc.createTextNode(QString::number(focal_length_));
	root.appendChild(fl_node);

	QTextStream stream_camera(&camera_file);
	camera_doc.save(stream_camera, 4, camera_doc.EncodingFromTextStream);
	camera_file.close();*/
}

QString SensorInfo::get_bay_pat() const
{
	return bay_pat_;
}

QString SensorInfo::get_orientation() const
{
	return orientation_;
}

double SensorInfo::get_focal_length35() const
{
	return focal_length35_;
}

int SensorInfo::get_width() const
{
	return width_;
}

int SensorInfo::get_height() const
{
	return height_;
}

double SensorInfo::get_width_mm() const
{
	return width_mm_;
}

void SensorInfo::set_width_mm(double width_mm)
{
	width_mm_ = width_mm;
}

}