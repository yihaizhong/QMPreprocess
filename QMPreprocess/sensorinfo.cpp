#include "sensorinfo.h"

#include <QFile>
#include <QTextStream>
#include <QDomDocument>

#include "exiv2/exiv2.hpp"
#include "sensorinfodb.h"
#include "sensorinfodb.h"

namespace qm{


SensorInfo::SensorInfo()
{

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

double SensorInfo::get_focal_length() const
{
	return focal_length_;
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

		width_ = exifData["Exif.Photo.PixelXDimension"].getValue()->toLong();
		height_ = exifData["Exif.Photo.PixelYDimension"].getValue()->toLong();

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