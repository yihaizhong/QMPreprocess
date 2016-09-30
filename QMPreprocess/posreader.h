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
     * @brief read the default format: id x y z 
     * @param slist the string list of above
     * @return pos data
     */
	virtual PosData read(QStringList &slist) = 0;
};
}

#endif // POSREADER_H
