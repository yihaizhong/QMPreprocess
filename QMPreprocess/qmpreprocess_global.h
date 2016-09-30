#ifndef QMPREPROCESS_GLOBAL_H
#define QMPREPROCESS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QMPREPROCESS_LIB
# define DLL_EXPORT Q_DECL_EXPORT
#else
# define DLL_EXPORT Q_DECL_IMPORT
#endif
#include "common.h"
#endif // QMPREPROCESS_GLOBAL_H
