#ifndef LBHOPKPOSREADER_H
#define LBHOPKPOSREADER_H

#include "lbhposreader.h"

namespace qm{

class LBHOPKPosReader:public LBHPosReader
{
public:
	LBHOPKPosReader();
	~LBHOPKPosReader();
	virtual PosData read(QStringList &slist);
private:
	
};
}

#endif // LBHOPKPOSREADER_H
