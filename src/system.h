#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef WINDOWS_VERSION
#define MKDIR(X) mkdir(X)
#else
#include <sys/stat.h>
#define MKDIR(X) mkdir(X, S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH )
#endif // WINDOWS_VERSION

#endif /* SYSTEM_H */
