/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#include "FDMUrlListReceiver.h"
#include "common.h"

NS_IMPL_ISUPPORTS1(CFDMUrlListReceiver, IFDMUrlListReceiver)

CFDMUrlListReceiver::CFDMUrlListReceiver()
{
	CoInitialize (NULL);
}

CFDMUrlListReceiver::~CFDMUrlListReceiver()
{
	m_spUrlListRcvr = NULL;
	CoUninitialize ();
}  

NS_IMETHODIMP CFDMUrlListReceiver::AddUrl(IFDMUrl *url)
{
    if (m_spUrlListRcvr == NULL)
		m_spUrlListRcvr.CreateInstance (__uuidof (WGUrlListReceiver));

	assert (m_spUrlListRcvr != NULL);
    if (m_spUrlListRcvr == NULL)
		return NS_ERROR_FAILURE;

	wchar_t *wsz;
	_bstr_t bstr;

	url->GetUrl (&wsz);
	bstr = wsz;
	CC (m_spUrlListRcvr->put_Url (bstr));
	delete [] wsz;

	url->GetReferer (&wsz);
	bstr = wsz;
	CC (m_spUrlListRcvr->put_Referer (bstr));
	delete [] wsz;

	url->GetComment (&wsz);
	bstr = wsz;
	CC (m_spUrlListRcvr->put_Comment (bstr));
	delete [] wsz;

	url->GetCookies (&wsz);
	bstr = wsz;
	CC (m_spUrlListRcvr->put_Cookies (bstr));
	delete [] wsz;

	CC (m_spUrlListRcvr->AddUrlToList ());

	return NS_OK;
}  

NS_IMETHODIMP CFDMUrlListReceiver::ShowAddUrlListDialog()
{
    assert (m_spUrlListRcvr != NULL);
	if (m_spUrlListRcvr == NULL)
		return NS_ERROR_FAILURE;

	CC (m_spUrlListRcvr->ShowAddUrlListDialog ());

	m_spUrlListRcvr = NULL;

    return NS_OK;
}
