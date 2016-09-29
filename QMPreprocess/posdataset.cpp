#include "posdataset.h"
//#include "posdata.h"
//#include "posreader.h"
#include <QFile>
#include <QTextStream>
namespace qm{


PosDataset::PosDataset()
{

}

PosDataset::~PosDataset()
{

}

int PosDataset::size() const
{
	return pos_dataset_.size();
}

qm::PosData PosDataset::getDataAt(int i) const
{
	return pos_dataset_.at(i);
}



void PosDataset::readFromFile(QString &pos_file, PosReader &pos_reader)
{
	QFile file(pos_file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		throw QString("file not found!->" + pos_file);
	QTextStream stream(&file);
	QString line;
	PosData pos_data;
	while (!(line=stream.readLine()).isNull())
	{
		line=line.trimmed();
		if (line.isEmpty())
			continue;
		QStringList slist = line.split(" +|,");
		pos_data = pos_reader.read(slist);
		pos_dataset_.push_back(pos_data);
	}
	file.close();
}

void PosDataset::forEach(void(*fun)(PosData &))
{
	for (auto bg = pos_dataset_.begin(); bg != pos_dataset_.end();++bg)
	{
		fun(*bg);
	}
}

//void PosDataset::getDataFromStringList(QStringList &slist)
//{
//	
//}

}