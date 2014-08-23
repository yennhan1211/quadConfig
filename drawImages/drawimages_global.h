#ifndef DRAWIMAGES_GLOBAL_H
#define DRAWIMAGES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DRAWIMAGES_LIBRARY)
#  define DRAWIMAGESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DRAWIMAGESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DRAWIMAGES_GLOBAL_H
