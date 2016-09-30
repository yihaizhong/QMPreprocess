#ifndef XYZPOSREADER_H
#define XYZPOSREADER_H

#include "posreader.h"

namespace qm{


class XYZPosReader : public PosReader
{
public:
	XYZPosReader();
	~XYZPosReader();
	/**
	* @brief read the pos, format: id x y z, and set the o=p=k=0
	* @param slist the string list of above
	* @return pos data
	*/
	virtual PosData read(QStringList &slist);
private:
	
};
}
#endif // XYZPOSREADER_H
