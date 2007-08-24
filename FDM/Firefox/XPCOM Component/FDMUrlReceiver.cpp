/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#include "FDMUrlReceiver.h"
#include "common.h"

NS_IMPL_ISUPPORTS1(CFDMUrlReceiver, IFDMUrlReceiver)

CFDMUrlReceiver::CFDMUrlReceiver()
{
	CoInitialize (NULL);
}

CFDMUrlReceiver::~CFDMUrlReceiver()
{
	m_spUrlRcvr = NULL;
	CoUninitialize ();
}  

NS_IMETHODIMP CFDMUrlReceiver::SetUrl(IFDMUrl *url)
{
	if (m_spUrlRcvr == NULL)
		m_spUrlRcvr.CreateInstance (__uuidof (WGUrlReceiver));

	assert (m_spUrlRcvr != NULL);
    if (m_spUrlRcvr == NULL)
		return NS_ERROR_FAILURE;

	CC (ApplyUrlToReceiver (m_spUrlRcvr, url));

	return NS_OK;
}  

NS_IMETHODIMP CFDMUrlReceiver::ShowAddDownloadDialog()
{
	assert (m_spUrlRcvr != NULL);
	if (m_spUrlRcvr == NULL)
		return NS_ERROR_FAILURE;

	CC (m_spUrlRcvr->ShowAddDownloadDialog ());

	m_spUrlRcvr = NULL;

    return NS_OK;
}

nsresult CFDMUrlReceiver::ApplyUrlToReceiver(IWGUrlReceiver *pUrlRcvr, IFDMUrl *url)
{
	wchar_t *wsz;
	_bstr_t bstr;

	url->GetUrl (&wsz);
	bstr = wsz;
	CC (pUrlRcvr->put_Url (bstr));
	delete [] wsz;

	url->GetReferer (&wsz);
	bstr = wsz;
	CC (pUrlRcvr->put_Referer (bstr));
	delete [] wsz;

	url->GetComment (&wsz);
	bstr = wsz;
	CC (pUrlRcvr->put_Comment (bstr));
	delete [] wsz;

	url->GetCookies (&wsz);
	bstr = wsz;
	CC (pUrlRcvr->put_Cookies (bstr));
	delete [] wsz;

	url->GetPostData (&wsz);
	bstr = wsz;
	CC (pUrlRcvr->put_PostData (bstr));
	delete [] wsz;

	return NS_OK;
}
