#ifndef IGRAPH_CONFIG_H
#define IGRAPH_CONFIG_H

#define HAVE_EXPM1 1
#define HAVE_FMIN 1
/* #undef HAVE_FINITE */
#define HAVE_ISFINITE 1
#define HAVE_LOG2 1
#define HAVE_LOG1P 1
#define HAVE_RINT 1
#define HAVE_RINTF 1
#define HAVE_ROUND 1
/* #undef HAVE_STPCPY */
/* #undef HAVE_STRCASECMP */
#define HAVE__STRICMP 1
#define HAVE_STRDUP 1

#define HAVE_GLPK 1
/* #undef HAVE_LIBXML */

#define INTERNAL_BLAS 1
#define INTERNAL_LAPACK 1
#define INTERNAL_ARPACK 1
#define INTERNAL_GMP 1

#define IGRAPH_F77_SAVE static 
#define IGRAPH_THREAD_LOCAL 

#define PACKAGE_VERSION "0.9.2"
#define PACKAGE_VERSION_MAJOR 0
#define PACKAGE_VERSION_MINOR 9
#define PACKAGE_VERSION_PATCH 2
#define PACKAGE_VERSION_PRERELEASE "cmake-experimental"

#endif
