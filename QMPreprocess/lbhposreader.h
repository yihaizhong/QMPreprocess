#ifndef LBHPOSREADER_H
#define LBHPOSREADER_H

#include "posreader.h"

namespace qm{


class LBHPosReader:public PosReader
{
public:
	LBHPosReader();
	~LBHPosReader();
	virtual PosData read(QStringList &slist);
protected:
	int longitude0_;
};
}
#endif // LBHPOSREADER_H
