#ifndef POSDATA_H
#define POSDATA_H

#include <QString>
namespace qm{


struct PosData
{
	QString id;
	double x;
	double y;
	double z;
	double phi;
	double omega;
	double kappa;
	double longitude;
	double latitude;
};
}
#endif // POSDATA_H
