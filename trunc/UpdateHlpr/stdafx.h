/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <functional>
#include <stdint.h>
#include <memory>
#include <algorithm>

#include "../common/vms_sifdm_cl/config.h"
#include "../common/vms_sifdm_cl/verify.h"
#include "../common/vms_sifdm_cl/make_sure.h"
#include "../common/vms_sifdm_cl/tstring.h"
#include "../common/vms_sifdm_cl/stringh.h"
#include "../common/vms_sifdm_cl/vmsWinHandle.h"
#include "../common/vms_sifdm_cl/vmsCriticalSection.h"
#include "../common/vms_sifdm_cl/serialization/vmsSerializable.h"
#include "../common/vms_sifdm_cl/error/vmsError.h"
#include "../common/vms_sifdm_cl/win/windows_error.h"
#include "../common/vms_sifdm_cl/win/tools.h"
#include "../common/vms_sifdm_cl/vmsExeProgramData.h"
#include "../common/vms_sifdm_cl/fileio/util.h"
#include "../common/vms_sifdm_cl/vmsTmpFileName.h"
