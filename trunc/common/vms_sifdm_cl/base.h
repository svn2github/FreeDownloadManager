/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "config.h"
#include "verify.h"
#include "make_sure.h"
#include "tstring.h"
#include "stringh.h"
#include "vmsWinHandle.h"
#include "vmsCriticalSection.h"
#include "serialization/vmsSerializable.h"
#include "serialization/vmsSerializationStreamBinding.h"
#include "error/vmsError.h"
#include "win/windows_error.h"
#include "inet/http_error.h"
#include "win/tools.h"
#include "vmsCreatesThreads.h"
#include "fileio/util.h"
#include "progress/vmsOperationProgressBase.h"
#include "progress/vmsOperationProgressCallback.h"
#include "xml/vmsXmlUtil.h"
#include "vmsCommandLineParser.h"
#include "win/vmsBinaryFileVersionInfo.h"