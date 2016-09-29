#ifndef POSREADER_H
#define POSREADER_H
class QStringList;
namespace qm{

struct PosData;
/**
 * @brief  interface of reading pos data from a string list
 */
class PosReader
{
public:
	PosReader();
	~PosReader();
    /**
     * @brief read the default format: id x y z phi omega kappa
     * @param slist the string list of above
     * @return pos data
     */
	virtual PosData read(QStringList &slist);
};
typedef PosReader XYZOPKPosReader;
}

#endif // POSREADER_H
