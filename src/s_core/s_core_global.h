#ifndef S_CORE_GLOBAL_H
#define S_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(S_CORE_LIBRARY)
#  define S_CORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define S_CORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // S_CORE_GLOBAL_H
