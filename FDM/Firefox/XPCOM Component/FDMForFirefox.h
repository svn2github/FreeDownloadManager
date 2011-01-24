/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __FDMFORFIREFOX_H_
#define __FDMFORFIREFOX_H_

#include "fsLangMgr.h"	
#include "ivmsfdmff.h"
#include <atlbase.h>

#define FDMFORFIREFOX_CONTRACTID "@freedownloadmanager.org/FDMForFirefox;1"
#define FDMFORFIREFOX_CLASSNAME "Free Download Manager component for Firefox"

#define FDMFORFIREFOX_CID { 0x2b141cfe, 0xd16c, 0x499a, { 0x91, 0x5, 0x6, 0xab, 0xdc, 0xe9, 0x85, 0x66 } }

class CFDMForFirefox : public IFDMForFirefox
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IFDMFORFIREFOX

  static bool is_FlashGotInstalled();

  CFDMForFirefox();

private:
  ~CFDMForFirefox();

protected:
	PRBool TransferUrlToFDM (IFDMUrl* url);
	bool IsUrlShouldBeSkipped (IFDMUrl *url, const wchar_t* pwszSuggFileName);
	bool IsServerToSkip (IFDMUrl *url);
	CRegKey m_keyFDM, m_keyFDMMonitor, m_keyFDMFfStgs;
	fsLangMgr m_lm;
};    

#endif 