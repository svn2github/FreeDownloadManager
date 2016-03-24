/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "windows.h"
#include <tchar.h>

void fsIFOnMemoryError ()
{
	if (IDCANCEL == MessageBox (NULL, _T("Out of memory! Please close some applications and press Retry or Cancel to exit without saving any data"), _T("Out of memory"), MB_ICONEXCLAMATION|MB_RETRYCANCEL))
		ExitProcess (0);
}