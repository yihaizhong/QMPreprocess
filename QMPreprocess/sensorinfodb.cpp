#include "sensorinfodb.h"
#include "sensorinfo.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
namespace qm{


void SensorInfoDB::readFromDBFile()
{
	QFile dbfile(db_file_);
	if (!dbfile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	//QTextStream stream(&dbfile);
	QDomDocument doc;
	if (!doc.setContent(&dbfile))
		throw QString("can not open xml file:" + db_file_);
	QDomNodeList camera_list = doc.documentElement().elementsByTagName("CameraEntry");
	for (int i = 0; i < camera_list.size();++i)
	{
		auto camera = camera_list.at(i);
		QString camera_name = camera.toElement().elementsByTagName("Name").at(0).toElement().text();
		QString szcaptmm = camera.toElement().elementsByTagName("SzCaptMm").at(0).toElement().text();
		QStringList wh = szcaptmm.split(" +|,");
		if (wh.size()!=2)
			continue;
		SensorInfo *sensor_info = new SensorInfo;
		sensor_db_[camera_name] = sensor_info;
	}
	dbfile.close();
}

void SensorInfoDB::writeToDBFile()
{
	QFile dbfile(db_file_);
	if (!dbfile.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Truncate))
		return;
	QTextStream stream(&dbfile);
	QDomDocument doc;
	auto instruction = doc.createProcessingInstruction("xml", "version=\"1.0\"");
	doc.appendChild(instruction);
	auto root = doc.createElement("CameraDataBase");
	for (auto bg = sensor_db_.begin(); bg != sensor_db_.end();++bg)
	{
		SensorInfo *sensor_info = *bg;
		auto camera_entry = doc.createElement("CameraEntry");
		QDomElement name_node = doc.createElement("Name");
		auto name = doc.createTextNode(sensor_info->get_camera_type());
		name_node.appendChild(name);
		camera_entry.appendChild(name_node);
		QDomElement szcapt_node = doc.createElement("SzCaptMm");
		auto szcapt = doc.createTextNode(QString::number(sensor_info->get_width_mm()) +
			" " + QString::number(sensor_info->get_height_mm()));
		szcapt_node.appendChild(szcapt);
		camera_entry.appendChild(szcapt_node);
		root.appendChild(camera_entry);
	}
	doc.save(stream, 4, doc.EncodingFromTextStream);
	dbfile.close();
}

SensorInfoDB::SensorInfoDB()
{
	readFromDBFile();
}

SensorInfoDB* SensorInfoDB::instance_ = nullptr;

QString SensorInfoDB::db_file_ = "cameradb.xml";

SensorInfoDB::GC SensorInfoDB::gc;

SensorInfoDB::~SensorInfoDB()
{

}

SensorInfoDB* SensorInfoDB::instance()
{
	if (!instance_)
	{
		instance_ = new SensorInfoDB;
		return instance_;
	}
	else
	{
		return instance_;
	}
}

bool SensorInfoDB::isInDB(SensorInfo *sensor_info)
{
	auto rel = sensor_db_.find(sensor_info->get_camera_type());
	if (rel != sensor_db_.end())
	{
		double width = (*rel)->get_width_mm();
		double height = (*rel)->get_height_mm();
		sensor_info->set_width_mm(width);
		sensor_info->set_height_mm(height);
		return true;
	}
	else
	{
		return false;
	}
}

void SensorInfoDB::addSensorInfo(SensorInfo *sensor_info)
{
	if (!isInDB(sensor_info))
	{
		writeToDBFile();
	}
}

void SensorInfoDB::set_db_file(QString &db_file)
{
	db_file_ = db_file;
}

SensorInfoDB::GC::GC()
{

}

SensorInfoDB::GC::~GC()
{
	if (instance_)
	{
		delete instance_;
		instance_ = nullptr;
	}
}
}