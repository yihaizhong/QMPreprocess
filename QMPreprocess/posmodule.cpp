
#include "posmodule.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDomDocument>
#include "posdata.h"
#include "xyzposreader.h"
#include "xyzopkposreader.h"
#include "lbhposreader.h"
#include "lbhopkposreader.h"
#include "common.h"
namespace qm{


PosModule::PosModule() :pos_file_(""), reader_(nullptr)
{

}

PosModule::~PosModule()
{
	if (reader_)
		delete reader_;
}

void PosModule::set_pos_file(QString &pos_file)
{
	pos_file_ = pos_file;
}

void PosModule::set_pos_format(ePosFormat &format)
{
	if (reader_)
		delete reader_;
	switch (format)
	{
	case XYZOPK:
		reader_ = new XYZOPKPosReader;
		break;
	case LBHOPK:
		reader_ = new LBHOPKPosReader;
		break;
	case XYZ:
		reader_ = new XYZPosReader;
		break;
	case LBH:
		reader_ = new LBHPosReader;
		break;
	default:
		throw QString("there is no pos format for this");
		break;
	}
}

void PosModule::forEach(void(*fun)(PosData &pos_data))
{
	for (auto bg = pos_data_.begin(); bg != pos_data_.end();++bg)
	{
		fun(*bg);
	}
}

void PosModule::read()
{
	QFile file(pos_file_);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		throw QString("file not found!->" + pos_file_);
	QTextStream stream(&file);
	QString line;
	PosData pos_data;
	while (!(line = stream.readLine()).isNull())
	{
		line = line.trimmed().replace('\t',' ');
		if (line.isEmpty())
			continue;
		QStringList slist = line.split(QRegExp(" +|,"));
		pos_data = reader_->read(slist);
		pos_data_.push_back(pos_data);
	}
	file.close();
}

void PosModule::appendContentToId(QString &content)
{
	for (auto bg = pos_data_.begin(); bg != pos_data_.end(); ++bg)
	{
		bg->id.append(content);
	}
}

void WritePosInfo(PosData &pos_data, QString &file)
{
	QFile xml_file(file);
	if (!xml_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		throw QString("can not create or open the file->" + file);
	QDomDocument doc;
	auto instruction = doc.createProcessingInstruction("xml", "version=\"1.0\"");
	doc.appendChild(instruction);
	QDomElement root = doc.createElement("ExportAPERO");
	doc.appendChild(root);

	QDomElement orientation = doc.createElement("OrientationConique");
	root.appendChild(orientation);

	QDomElement proj_node = doc.createElement("TypeProj");
	QDomText proj = doc.createTextNode("eProjStenope");
	proj_node.appendChild(proj);
	orientation.appendChild(proj_node);

	QDomElement zone_node = doc.createElement("ZoneUtileInPixel");
	QDomText zone = doc.createTextNode("false");
	zone_node.appendChild(zone);
	orientation.appendChild(zone_node);

	QDomElement rel_node = doc.createElement("RelativeNameFI");
	QDomText rel = doc.createTextNode("true");
	rel_node.appendChild(rel);
	orientation.appendChild(rel_node);

	QDomElement externe_node = doc.createElement("Externe");
	orientation.appendChild(externe_node);

	QDomElement time_node = doc.createElement("Time");
	QDomText time = doc.createTextNode("0");
	time_node.appendChild(time);
	externe_node.appendChild(time_node);

	QDomElement conv_node = doc.createElement("KnownConv");
	QDomText conv = doc.createTextNode("eConvApero_DistM2C");
	conv_node.appendChild(conv);
	externe_node.appendChild(conv_node);

	QDomElement center_node = doc.createElement("Centre");
	QString x = QString::number(pos_data.x, 'f');
	QString y = QString::number(pos_data.y, 'f');
	QString z = QString::number(pos_data.z, 'f');
	QDomText center = doc.createTextNode(x+" "+y+" "+z);
	center_node.appendChild(center);
	externe_node.appendChild(center_node);

	QDomElement vit_node = doc.createElement("VitesseFiable");
	QDomText vit = doc.createTextNode("true");
	vit_node.appendChild(vit);
	externe_node.appendChild(vit_node);

	QDomElement rot_node = doc.createElement("rotesseFiable");
	externe_node.appendChild(rot_node);

	QDomElement angle_node = doc.createElement("CodageAngulaire");
	QString omega = QString::number(pos_data.omega, 'f');
	QString phi = QString::number(pos_data.phi, 'f');
	QString kappa = QString::number(pos_data.kappa, 'f');
	QDomText angle = doc.createTextNode(omega+" "+phi+" "+kappa);
	angle_node.appendChild(angle);
	rot_node.appendChild(angle_node);

	QDomElement ori_node = doc.createElement("ConvOri");
	orientation.appendChild(ori_node);

	ori_node.appendChild(conv_node);

	QTextStream stream(&xml_file);
	doc.save(stream, 4, doc.EncodingFromTextStream);
	xml_file.close();

}
void PosModule::writePosFile()
{
	QFileInfo file_info(pos_file_);
	QString work_dir(file_info.absolutePath());
	work_dir.append("/Ori-Pos");
	QDir().mkdir(work_dir);
	for (auto bg = pos_data_.begin(); bg != pos_data_.end(); ++bg)
	{
		QString info_file(work_dir + "/Orientation-" + bg->id + ".xml");
		WritePosInfo(*bg, info_file);
	}
}

//find a threshold 
double FindThreshod(QVector<PosData> &pos_data)
{
	QVector<double> dist;
	//get all distance and save into dist
	for (int i = 0; i < pos_data.size() - 1;++i)
	{
		double temp = sqrt(pow(pos_data[i].x - pos_data[i + 1].x, 2) +
					pow(pos_data[i].y - pos_data[i + 1].y, 2) +
					pow(pos_data[i].z - pos_data[i + 1].z, 2));
		dist.push_back(temp);
	}
	//reorder distance
	qSort(dist);
		
	//std::sort(dist.begin(), dist.end());

	//return the middle number
	return 4 * dist[dist.size() / 2];


}
//topology relationship

void PosModule::writePairFile(QString &pair_file)
{
	
	QFile file(pair_file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		throw QString("can not create or open the file -> " + pair_file);
	double threshold = FindThreshod(pos_data_);
	QDomDocument doc;
	auto instruction = doc.createProcessingInstruction("xml", "version=\"1.0\"");
	doc.appendChild(instruction);
	auto root = doc.createElement("SauvegardeNamedRel");
	doc.appendChild(root);

	for (int i = 0; i < pos_data_.size(); ++i)
	{
		for (int j = 0; j < pos_data_.size() - 1;++j)
		{
			double dis = sqrt(pow(pos_data_[i].x - pos_data_[j].x, 2.0) +
					pow(pos_data_[i].y - pos_data_[j].y, 2.0) +
					pow(pos_data_[i].z - pos_data_[j].z, 2));
			if (dis <= threshold)
			{
				QDomElement cple_node = doc.createElement("Cple");
				root.appendChild(cple_node);

				QDomText cple = doc.createTextNode(pos_data_[i].id + " " + pos_data_[j].id);
				cple_node.appendChild(cple);
			}
		}

	}
	QTextStream stream(&file);
	doc.save(stream, 4, doc.EncodingFromTextStream);
	file.close();
}

}