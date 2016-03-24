/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef __COMMON_H_
#define __COMMON_H_

#define _CRT_SECURE_NO_WARNINGS

#include <tchar.h>
#include <string>
#include <atlbase.h>
#include <assert.h>
#include "../common/vms_sifdm_cl/tstring.h"
#include "../common/vms_sifdm_cl/stringh.h"

#include "system.h"
#include "fsinet.h"

#define SAFE_DELETE_ARRAY(a) {if (a) {delete [] a; a = NULL;}}

#endif 