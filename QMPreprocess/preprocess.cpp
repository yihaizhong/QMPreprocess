#include "preprocess.h"
#include <functional>
#include <QVector>
#include <QString>
#include <QFileInfo>
#include <QProcess>
#include <QTextStream>
#include <QDomDocument>
#include <QDir>

#include "opencv2/opencv.hpp"

#include "imagemodule.h"
#include "posmodule.h"
#include "sensorinfo.h"
#include "posdata.h"
#include "methods.h"
#define USE_ELISE 1
#if USE_ELISE
#include "StdAfx.h"
#ifdef _DEBUG
#pragma comment(lib,"../../mm/lib/elised.lib")
#pragma comment(lib,"../../mm/lib/ANNd.lib")
#else
#pragma comment(lib,"../../mm/lib/ANN.lib")
#pragma comment(lib,"../../mm/lib/elise.lib")
#endif
#endif





namespace qm{


Preprocess::Preprocess(QVector<QString> &image_files) :
image_files_(image_files), image_module_(nullptr), pos_module_(nullptr)
{

}

Preprocess::~Preprocess()
{
	if (image_module_)
		delete image_module_;
	if (pos_module_)
		delete pos_module_;
}


void Preprocess::setModule(eImageModule image_module, ePosModule pos_module)
{
	if (image_module_)
		delete image_module_;
	if (pos_module_)
		delete pos_module_;
	switch (image_module)
	{
	case EXIV2:
		image_module_ = new Exiv2ImageModule;
		break;
	case EXIFTOOL:
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
	suffix.push_front(".");
	pos_module_->appendContentToId(suffix);
	pos_module_->writePosFile();
}

void Preprocess::makeImagePairFile(QString &pair_file)
{
	pos_module_->writePairFile(pair_file);
}

void AddSFS(const QString &xml)
{
	QFile f(xml);
	f.open(QIODevice::ReadOnly | QIODevice::Text);
	QDomDocument doc;
	doc.setContent(&f);
	f.close();
	QDomNode root = doc.documentElement();
	QDomNode kna_node = doc.createElement("KeyedNamesAssociations");
	root.appendChild(kna_node);
	QDomNode cals_node = doc.createElement("Calcs");
	kna_node.appendChild(cals_node);
	QDomNode arrite_node = doc.createElement("Arrite");
	cals_node.appendChild(arrite_node);
	QDomText arrite = doc.createTextNode(" 1 1 ");
	arrite_node.appendChild(arrite);
	QDomNode direct_node = doc.createElement("Direct");
	cals_node.appendChild(direct_node);

	QDomNode ptf_node = doc.createElement("PatternTransform");
	direct_node.appendChild(ptf_node);
	QDomText ptf = doc.createTextNode(".*");
	ptf_node.appendChild(ptf);

	QDomNode cn_node = doc.createElement("CalcName");
	direct_node.appendChild(cn_node);
	QDomText cn = doc.createTextNode("SFS");
	cn_node.appendChild(cn);

	QDomNode key_node = doc.createElement("Key");
	kna_node.appendChild(key_node);
	QDomText key = doc.createTextNode("NKS-Assoc-SFS");
	key_node.appendChild(key);

	f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
	QTextStream stream(&f);
	doc.save(stream, 4, doc.EncodingFromTextStream);
	f.close();
}
void LoadAndSave(const std::string &in_file, const std::string &out_file)
{
	cv::imwrite(out_file, cv::imread(in_file, cv::IMREAD_GRAYSCALE));
}
void Preprocess::enhanceImage()
{
#if USE_ELISE
	using namespace cv;
	QFileInfo finfo(image_files_[0]);
	AddSFS(finfo.absolutePath() + "/MicMac-LocalChantierDescripteur.xml");
	auto pattern = (finfo.absolutePath() + "/.*." + finfo.suffix()).toLocal8Bit();
	/*char* argv[] = { "haha", "All", pattern.data(), "-1" };
	GenerateImgInfo(4, argv);*/
	QProcess p;
	p.start("GenerateImgInfo.exe \""+pattern+"\"");
	p.waitForFinished(INT_MAX);
	QDir().mkdir(finfo.absolutePath() + "/SFS");
	std::vector<std::string> tif_files;
	yt::getFilesPath((finfo.absolutePath() + "/Tmp-MM-Dir/*.tif").toStdString(),
		tif_files);
	std::vector<std::string> in_files,out_files;
	for (int i = 0; i < tif_files.size();++i)
	{
		QString file=QString::fromStdString(tif_files[i]).replace("\\","/");
		if (file.contains("_sfs.tif"))
		{
			QString nfile(file);
			nfile.replace("/Tmp-MM-Dir/", "/SFS/");
			nfile.remove("_sfs.tif");
			in_files.push_back(tif_files[i]);
			out_files.push_back(nfile.toStdString());
		}
	}
	int bat = 50;
	int cnt = in_files.size() / bat;
	int rm = in_files.size() - bat*cnt;
	vector<string> tin(bat),tout(bat);
	for (size_t i = 0; i < cnt; i++)
	{
		qCopy(in_files.begin() + i*bat, in_files.begin() + i*bat + bat, tin.begin());
		qCopy(out_files.begin() + i*bat, out_files.begin() + i*bat + bat, tout.begin());
		yt::multiCall(LoadAndSave, tin, tout);
	}
	tin.resize(rm);
	tout.resize(rm);
	qCopy(in_files.begin() + cnt*bat, in_files.end(), tin.begin());
	qCopy(out_files.begin() + cnt*bat, out_files.end(), tout.begin());
	yt::multiCall(LoadAndSave, tin, tout);
#endif
}

QVector<QString> Preprocess::get_image_files() const
{
	return image_files_;
}

double Preprocess::getPosHeight()
{
	return pos_module_->getAverageHeight();
}

}