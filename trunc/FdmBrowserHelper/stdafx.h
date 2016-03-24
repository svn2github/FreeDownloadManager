/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "targetver.h"

#include <windows.h>
#include <ImageHlp.h>
#include <atlbase.h>
#include <atlwin.h>

#include <stdio.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <future>
#include <regex>

#include "../Include.Add/picojson.h"
#define JsonSerializationOutputStreamClass vmsJson2SerializationOutputStream

#include "../common/vms_sifdm_cl/base.h"
#include "../common/vms_sifdm_cl/threadsafe/vmsThreadSafe4.h"
#include "../common/vms_sifdm_cl/json/vmsJson2SerializationStream.h"
#include "../common/vms_sifdm_cl/json/util.h"
#include "../common/vms_sifdm_cl/inet/uri/UriCodec.h"
#include "../common/vms_sifdm_cl/data/base64.h"
#include "../common/vms_sifdm_cl/serialization/vmsSerializableObjVector.h"
#include "../common/vms_sifdm_cl/win/com/vmsComInitialize.h"
#include "../common/vms_sifdm_cl/inet/uri/util.h"
#include "../common/vms_sifdm_cl/thread/vmsCreatesThreads2.h"
#include "../common/vms_sifdm_cl/win/vmsDdeWindow.h"
#include "../common/vms_sifdm_cl/win/process/util.h"
#include "../common/vms_sifdm_cl/win/process/util2.h"
#include "../common/vms_sifdm_cl/win/defs.h"
