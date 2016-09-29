#include "sensorinfo.h"
#include "exiv2/exiv2.hpp"

namespace qm{


SensorInfo::SensorInfo()
{

}

SensorInfo::~SensorInfo()
{

}

void SensorInfo::readExifInfo(QString &image_file)
{
	using namespace Exiv2;

	try
	{
		Image::AutoPtr image = ImageFactory::open(image_file.toStdString());
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

	}
	catch (...)
	{
		throw QString("can not open the file : " + image_file);
	}
}

void SensorInfo::writeToFile(QString &info_file)
{

}

QString SensorInfo::came_db_file_ = "cameradb.xml";

}