#include "posreader.h"
#include <QStringList>
#include "posdata.h"
namespace qm{


PosReader::PosReader()
{

}

PosReader::~PosReader()
{

}

PosData PosReader::read(QStringList &slist)
{
	if (slist.size() < 7)
		throw QString("bad format!");
	PosData pos_data;
	bool ok = true;
	pos_data.id = slist[0];
	pos_data.x = slist[1].toDouble(&ok);
	pos_data.y = slist[2].toDouble(&ok);
	pos_data.z = slist[3].toDouble(&ok);
	pos_data.phi = slist[4].toDouble(&ok);
	pos_data.omega = slist[5].toDouble(&ok);
	pos_data.kappa = slist[6].toDouble(&ok);
	if (!ok)
		throw QString("bad format!");
	return pos_data;
}

}