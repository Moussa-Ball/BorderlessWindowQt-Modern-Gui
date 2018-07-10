#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(STUDIOSOFTER_LIB)
#  define STUDIOSOFTER_EXPORT Q_DECL_EXPORT
# else
#  define STUDIOSOFTER_EXPORT Q_DECL_IMPORT
# endif
#else
# define STUDIOSOFTER_EXPORT
#endif
