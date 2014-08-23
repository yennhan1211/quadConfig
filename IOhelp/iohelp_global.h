#ifndef IOHELP_GLOBAL_H
#define IOHELP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IOHELP_LIBRARY)
#  define IOHELPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IOHELPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // IOHELP_GLOBAL_H
