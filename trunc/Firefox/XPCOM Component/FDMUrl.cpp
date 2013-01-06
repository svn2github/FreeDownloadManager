/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "FDMUrl.h"
#include <windows.h>
#include <nsmemory.h>
#include "xrfix.h"
#include "vmsHttpRedirectList.h"

XRFIX_NS_IMPL_ISUPPORTS1(CFDMUrl, IFDMUrl, IFDMURL_IID)

CFDMUrl::CFDMUrl()
{

}

CFDMUrl::~CFDMUrl()
{
  
}

NS_IMETHODIMP CFDMUrl::GetUrl(PRUnichar * *aUrl)
{
    *aUrl = (PRUnichar*) nsMemory::Clone (m_wstrUrl.c_str (), (m_wstrUrl.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetUrl(const PRUnichar * aUrl)
{
	if (!aUrl)
		return NS_ERROR_INVALID_ARG;

	m_wstrUrl = aUrl;

	vmsHttpRedirectList::o ().Lock (true);
	int nIndex = vmsHttpRedirectList::o ().findRedirectIndex (aUrl);
	if (nIndex != -1)
		m_wstrOriginalUrl = vmsHttpRedirectList::o ().getRedirect (nIndex)->wstrOriginalUrl;
	vmsHttpRedirectList::o ().Lock (false);

	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetReferer(PRUnichar * *aReferer)
{
    *aReferer = (PRUnichar*) nsMemory::Clone (m_wstrReferer.c_str (), (m_wstrReferer.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetReferer(const PRUnichar * aReferer)
{
	m_wstrReferer = L"";

	if (!aReferer)
		return NS_OK;

	if (_wcsicmp (aReferer, L"about:blank"))
		m_wstrReferer = aReferer;

	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetComment(PRUnichar * *aComment)
{
    *aComment = (PRUnichar*) nsMemory::Clone (m_wstrComment.c_str (), (m_wstrComment.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetComment(const PRUnichar * aComment)
{
	m_wstrComment = aComment ? aComment : L"";
    return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetCookies(PRUnichar * *aCookies)
{
    *aCookies = (PRUnichar*) nsMemory::Clone (m_wstrCookies.c_str (), (m_wstrCookies.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetCookies(const PRUnichar * aCookies)
{
    m_wstrCookies = aCookies ? aCookies : L"";
	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetPostData(PRUnichar * *aPostData)
{
	*aPostData = (PRUnichar*) nsMemory::Clone (m_wstrPostData.c_str (), (m_wstrPostData.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::SetPostData(const PRUnichar * aPostData)
{
	if (aPostData)
		m_wstrPostData = aPostData;
	else
		m_wstrPostData = L"";

    return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetOriginalUrl(PRUnichar * *aUrl)
{
	*aUrl = (PRUnichar*) nsMemory::Clone (m_wstrOriginalUrl.c_str (), 
		(m_wstrOriginalUrl.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetOriginalUrl(const PRUnichar * aUrl)
{
	m_wstrOriginalUrl = aUrl ? aUrl : L"";
	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetUserAgent(PRUnichar * *aUrl)
{
	*aUrl = (PRUnichar*) nsMemory::Clone (m_wstrUserAgent.c_str (), 
		(m_wstrUserAgent.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetUserAgent(const PRUnichar * aUrl)
{
	m_wstrUserAgent = aUrl ? aUrl : L"";
	return NS_OK;
}