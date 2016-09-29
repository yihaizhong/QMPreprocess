#include "imagemodule.h"
#include <QFileInfo>
#include <QDir>
#include "sensorinfo.h"
#include "exiv2/exiv2.hpp"

namespace qm{


ImageModule::ImageModule()
{

}

ImageModule::~ImageModule()
{

}

void ImageModule::set_image_file(QString &image_file)
{
	image_file = image_file_;
}

SensorInfo* ImageModule::get_sensor_info() const
{
	return sensor_info_;
}

void ImageModule::set_sensor_info(SensorInfo *sensor_info)
{
	*sensor_info_ = *sensor_info_;
}

void ImageModule::setHorizontal()
{
	using namespace Exiv2;
	try
	{
		Image::AutoPtr image = ImageFactory::open(image_file_.toStdString());
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
	if (sensor_info_->get_focal_length() < 0 || sensor_info_->get_camera_type().isEmpty())
		return false;
	else
		return true;
}

void ImageModule::writeInfoFile()
{

}

void ImageModule::enhance()
{

}

}