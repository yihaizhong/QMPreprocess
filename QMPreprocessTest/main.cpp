
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QString>
#include <QVector>
#include <QStringList>
#include "preprocess.h"
#include "common.h"
#include "sensorinfo.h"
#include "sensorinfodb.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	//QString work_dir(argv[1]);
	QDir work_dir(QString::fromLocal8Bit(argv[1]));
	auto info_list = work_dir.entryInfoList(QStringList() << "*.jpg" << "*.tif", QDir::Files);
	QVector<QString> image_files;
	for (auto bg = info_list.begin(); bg != info_list.end();++bg)
	{
		image_files.push_back(bg->filePath());
	}
	qm::Preprocess pp(image_files);
	pp.setModule(qm::Preprocess::EXIV2, qm::Preprocess::PROJ4);
	//pp.setImageHorizontal();
	//if (!pp.checkSensorInfoInImage(image_files[0]))
	//{
	//	qm::SensorInfo *sensor_info = new qm::SensorInfo;
	//	sensor_info->readExifInfo(image_files[0]);
	//	sensor_info->set_width_mm(23.5);
	//	sensor_info->set_height_mm(15.6);
	//	pp.setSensor(sensor_info);
	//	qm::SensorInfoDB::instance()->addSensorInfo(sensor_info);
	//	delete sensor_info;
	//}
	//QString sensor_file(work_dir.absolutePath() + "/MicMac-LocalChantierDescripteur.xml");
	//pp.makeSensorFile(sensor_file);
	//pp.makeImageInfoFile();
	QString pos_file(work_dir.absolutePath() + "/pos.txt");
	pp.readPos(pos_file, qm::XYZOPK);
	pp.makeImagePosFile();
	QString pair_file(work_dir.absolutePath() + "/GrapheHom.xml");
	pp.makeImagePairFile(pair_file);
	return a.exec();
}
