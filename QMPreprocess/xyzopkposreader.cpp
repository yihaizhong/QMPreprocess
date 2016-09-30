#include "xyzopkposreader.h"
#include "posdata.h"
#include <QVector>
namespace qm{


XYZOPKPosReader::XYZOPKPosReader()
{

}

XYZOPKPosReader::~XYZOPKPosReader()
{

}

PosData XYZOPKPosReader::read(QStringList &slist)
{
	QStringList xyzlist;
	xyzlist << slist[0] << slist[1] << slist[2] << slist[3];
	PosData pos_data = XYZPosReader::read(xyzlist);

	QVector<bool> ok(3, false);
	if (slist.size() == 7)
	{
		pos_data.omega = slist[4].toDouble(&ok[0]);
		pos_data.phi = slist[5].toDouble(&ok[1]);
		pos_data.kappa = slist[6].toDouble(&ok[2]);

	}
	if (ok.contains(false))
		throw QString("bad format!");
	return pos_data;
}
}