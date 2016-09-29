#include "preprocess.h"
#include <functional>
#include <QVector>
#include <QString>
#include <QFileInfo>
#include "imagemodule.h"
#include "posmodule.h"
#include "sensorinfo.h"
#include "posdata.h"

namespace qm{


Preprocess::Preprocess(QVector<QString> &image_files) :
	image_files_(image_files)
{

}

Preprocess::~Preprocess()
{

}


void Preprocess::setModule(eImageModule image_module, ePosModule pos_module)
{
	if (image_module_)
		delete image_module_;
	if (pos_module_)
		delete pos_module_;
	switch (image_module)
	{
	case qm::EXIV2:
		image_module_ = new Exiv2ImageModule;
		break;
	case qm::EXIFTOOL:
		break;
	default:
		image_module_ = nullptr;
		break;
	}
	switch (pos_module)
	{
	case PROJ4:
		pos_module_ = new Proj4PosModule;
		break;
	case GDAL:
		break;
	default:
		pos_module_ = nullptr;
		break;
	}
	
}


void Preprocess::setImageHorizontal()
{
	for (auto bg = image_files_.begin(); bg != image_files_.end();++bg)
	{
		image_module_->set_image_file(*bg);
		image_module_->setHorizontal();
	}
}

bool Preprocess::checkSensorInfoInImage(QString &image_file)
{
	image_module_->set_image_file(image_file);
	return image_module_->checkSensorInfo();
}

void Preprocess::setSensor(SensorInfo *sensor)
{
	image_module_->set_sensor_info(sensor);
}

void Preprocess::makeSensorFile(QString &sensor_file)
{
	SensorInfo *sensor_info = image_module_->get_sensor_info();
	sensor_info->writeToFile(sensor_file);
}

void Preprocess::makeImageInfoFile()
{
	for (auto bg = image_files_.begin(); bg != image_files_.end(); ++bg)
	{
		image_module_->set_image_file(*bg);
		image_module_->writeInfoFile();
	}
}

void Preprocess::readPos(QString &pos_file, ePosFormat pos_foramt)
{
	pos_module_->set_pos_file(pos_file);
	pos_module_->set_pos_format(pos_foramt);
	pos_module_->read();
}

void Preprocess::makeImagePosFile()
{
	QString suffix = QFileInfo(image_files_[0]).suffix();
	pos_module_->appendContentToId(suffix);
	pos_module_->writePosFile();
}

void Preprocess::makeImagePairFile(QString &pair_file)
{
	pos_module_->writePairFile(pair_file);
}

void Preprocess::enhanceImage()
{
	for (auto bg = image_files_.begin(); bg != image_files_.end(); ++bg)
	{
		image_module_->set_image_file(*bg);
		image_module_->enhance();
	}
}

}