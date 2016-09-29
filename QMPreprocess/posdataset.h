#ifndef POSDATASET_H
#define POSDATASET_H
#include "qmpreprocess_global.h"
#include <QVector>
class QString;
namespace qm{
struct PosData;
class PosReader;
enum ePosFormat
{
	XYZOPK,
	LBHOPK,
	XYZ,
	LBH
};
class DLL_EXPORT PosDataset
{
public:
	PosDataset();
	~PosDataset();
    /**
     * @brief get the count of pos dataset
     * @return
     */
	int size()const;
    /**
     * @brief get data at index i,if i is out of range,throw a QString exception
     * @param i
     * @return
     */
	PosData getDataAt(int i)const;
    /**
     * @brief read pos data from a file through the pos reader
     * @param pos_file
     * @param pos_reader
     */
	void readFromFile(QString &pos_file, PosReader &pos_reader);
    /**
     * @brief do the same thing for all the pos data
     */
	void forEach(void(*fun)(PosData &));
protected:
	QVector<PosData> pos_dataset_;
};
}

#endif // POSDATASET_H
