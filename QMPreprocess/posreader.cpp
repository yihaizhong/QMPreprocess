#include "posreader.h"
#include <QStringList>
#include <QVector>
#include "posdata.h"
namespace qm{


PosReader::PosReader()
{

}

PosReader::~PosReader()
{

}


//PosData PosReader::read(QStringList &slist)
//{
//	PosData pos_data;
//	QVector<bool> ok(6,false);
//	pos_data.id = slist[0];
//	pos_data.x = slist[1].toDouble(&ok[0]);
//	pos_data.y = slist[2].toDouble(&ok[1]);
//	pos_data.z = slist[3].toDouble(&ok[2]);
//	if (slist.size() == 7)
//	{
//		pos_data.omega = slist[4].toDouble(&ok[3]);
//		pos_data.phi = slist[5].toDouble(&ok[4]);
//		pos_data.kappa = slist[6].toDouble(&ok[5]);
//		
//	}
//	//if (slist.size() == 3)
//	//{
//	//	pos_data.omega = pos_data.phi = pos_data.kappa = 0;
//	//	ok[3] = ok[4] = ok[5] = 0;
//	//}
//	if (ok.contains(false))
//		throw QString("bad format!");
//	return pos_data;
//}

}