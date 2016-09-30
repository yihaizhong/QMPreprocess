#include "xyzposreader.h"
#include <QVector>
#include "posdata.h"
namespace qm{


XYZPosReader::XYZPosReader()
{

}

XYZPosReader::~XYZPosReader()
{

}


PosData XYZPosReader::read(QStringList &slist)
{
	PosData pos_data;
	QVector<bool> ok(3, false);
	pos_data.id = slist[0];
	pos_data.x = slist[1].toDouble(&ok[0]);
	pos_data.y = slist[2].toDouble(&ok[1]);
	pos_data.z = slist[3].toDouble(&ok[2]);
	if (ok.contains(false))
		throw QString("bad format!");
	return pos_data;
}
}