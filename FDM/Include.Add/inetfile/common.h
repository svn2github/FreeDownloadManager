#ifndef __COMMON_H_
#define __COMMON_H_

#include "system.h"
#include "fsinet.h"

#define SAFE_DELETE_ARRAY(a) {if (a) {delete [] a; a = NULL;}}

#endif 