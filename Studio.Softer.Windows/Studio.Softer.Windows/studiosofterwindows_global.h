#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(STUDIOSOFTERWINDOWS_LIB)
#  define STUDIOSOFTERWINDOWS_EXPORT Q_DECL_EXPORT
# else
#  define STUDIOSOFTERWINDOWS_EXPORT Q_DECL_IMPORT
# endif
#else
# define STUDIOSOFTERWINDOWS_EXPORT
#endif
