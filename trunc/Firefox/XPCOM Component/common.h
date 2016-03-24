/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef __COMMON_H_
#define __COMMON_H_

#define _CRT_SECURE_NO_WARNINGS

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#include <windows.h>
#include <DbgHelp.h>
#include <ObjBase.h>
#include <MsXml.h>
#include <comdef.h>
#include <ShlObj.h>

_COM_SMARTPTR_TYPEDEF(IXMLDOMDocument, __uuidof(IXMLDOMDocument));
_COM_SMARTPTR_TYPEDEF(IXMLDOMNode, __uuidof(IXMLDOMNode));
_COM_SMARTPTR_TYPEDEF(IXMLDOMElement, __uuidof(IXMLDOMElement));
_COM_SMARTPTR_TYPEDEF(IXMLDOMNodeList, __uuidof(IXMLDOMNodeList));

#include <atlbase.h>

#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <memory>
#include <functional>
#include <stdint.h>

#include "../../common/vms_sifdm_cl/base.h"
#include "../../common/vms_sifdm_cl/win/tools.h"
#include "../../common/vms_sifdm_cl/fileio/util.h"
#include "../../common/vms_sifdm_cl/win/vmsBinaryFileVersionInfo.h"

#include "../../common/vmsFdmTranslations.h"

#define CC(cmd) {HRESULT hr = cmd; assert (SUCCEEDED (hr)); if (FAILED (hr)) return NS_ERROR_FAILURE;}

#if defined (XUL_SDK_VER) && XUL_SDK_VER >= 22

#if XUL_SDK_VER >= 30

#if XUL_SDK_VER >= 39
#include "ivmsfdmff39.h"
#else
#include "ivmsfdmff30.h"
#endif

#define XULSDK_PRBool	bool
#define PRUint32 uint32_t
#define PRUint8 uint8_t
#define PRUnicharIDL char16_t
#define wchar_t_IDL	char16_t

#else 

#include "ivmsfdmff22.h"
#define XULSDK_PRBool	bool
#define PRUnicharIDL PRUnichar
#define wchar_t_IDL	wchar_t

#endif

#else

#include "ivmsfdmff.h"
#define XULSDK_PRBool	PRBool
#define PRUnicharIDL PRUnichar
#define wchar_t_IDL	wchar_t

#endif 

inline wchar_t* pwchar_t_IDL_to_pwchar_t (wchar_t_IDL* p)
{
	return reinterpret_cast <wchar_t*> (p);
}

inline const wchar_t* pwchar_t_IDL_to_pwchar_t (const wchar_t_IDL* p)
{
	return reinterpret_cast <const wchar_t*> (p);
}

inline wchar_t_IDL* pwchar_t_to_pwchar_t_IDL (wchar_t* p)
{
	return reinterpret_cast <wchar_t_IDL*> (p);
}

inline const wchar_t_IDL* pwchar_t_to_pwchar_t_IDL (const wchar_t* p)
{
	return reinterpret_cast <const wchar_t_IDL*> (p);
}

#endif 