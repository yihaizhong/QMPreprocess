#include "posmodule.h"
#include <QFile>
#include <QTextStream>
#include "posreader.h"
#include "posdata.h"
namespace qm{


PosModule::PosModule()
{

}

PosModule::~PosModule()
{

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
		break;
	case XYZ:
		break;
	case LBH:
		break;
	default:
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
		line = line.trimmed();
		if (line.isEmpty())
			continue;
		QStringList slist = line.split(" +|,");
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

void PosModule::writePosFile()
{

}

void PosModule::writePairFile(QString &pair_file)
{
	
}

}