#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(STUDIOSOFTERCONTROLS_LIB)
#  define STUDIOSOFTERCONTROLS_EXPORT Q_DECL_EXPORT
# else
#  define STUDIOSOFTERCONTROLS_EXPORT Q_DECL_IMPORT
# endif
#else
# define STUDIOSOFTERCONTROLS_EXPORT
#endif
