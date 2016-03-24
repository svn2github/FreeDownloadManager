/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS 

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             

#include <windows.h>

#include <atlbase.h>
#include <atlwin.h>

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <queue>

#include "../Include.Add/picojson.h"
#define JsonSerializationOutputStreamClass vmsJson2SerializationOutputStream

#include "../common/vms_sifdm_cl/base.h"
#include "../common/vms_sifdm_cl/json/vmsjson2serializationstream.h"
#include "../common/vms_sifdm_cl/json/util.h"
#include "../common/vms_sifdm_cl/win/vmsDdeWindow.h"
#include "../common/vms_sifdm_cl/win/com/vmsComInitialize.h"
