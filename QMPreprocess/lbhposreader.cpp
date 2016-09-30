#include "lbhposreader.h"
#include <QVector>
#include "posdata.h"
#include "proj_api.h"
namespace qm{


LBHPosReader::LBHPosReader() :longitude0_(-9999)
{

}

LBHPosReader::~LBHPosReader()
{

}

PosData LBHPosReader::read(QStringList &slist)
{
	PosData pos_data;
	QVector<bool> ok(3, false);
	pos_data.id = slist[0];
	pos_data.longitude = slist[1].toDouble(&ok[0]);
	pos_data.latitude = slist[2].toDouble(&ok[1]);
	pos_data.z = slist[3].toDouble(&ok[2]);
	if (ok.contains(false))
		throw QString("bad format");

	int longitude = int(pos_data.longitude+1.5)/3*3;
	if (abs(longitude0_ - longitude) > 3)
		longitude0_ = longitude;
	QString param("+proj=tmerc ");
	param.append("+ellps=WGS84 ");
	param.append("+lon_0=" + QString::number(longitude0_));
	param.append("e +x_0=500000 +y0=0");
	projPJ proj = pj_init_plus(param.toLocal8Bit().data());
	if (!proj)
		throw QString("init proj failed!");

	projLP lp;
	lp.u = pos_data.longitude;
	lp.v = pos_data.latitude;
	projXY xy= pj_fwd(lp, proj);
	pos_data.x = xy.u;
	pos_data.y = xy.v;

	return pos_data;
}
}