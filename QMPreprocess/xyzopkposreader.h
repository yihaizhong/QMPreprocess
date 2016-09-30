#ifndef XYZOPKPOSREADER_H
#define XYZOPKPOSREADER_H

#include "xyzposreader.h"

namespace qm{


class XYZOPKPosReader : public XYZPosReader
{
public:
	XYZOPKPosReader();
	~XYZOPKPosReader();
	/**
	* @brief read the default format: id x y z omega phi kappa
	* @param slist the string list of above
	* @return pos data
	*/
	virtual PosData read(QStringList &slist);
private:
	
};
}
#endif // XYZOPKPOSREADER_H
