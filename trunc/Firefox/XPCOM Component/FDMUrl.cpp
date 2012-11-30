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
	m_strUrl = "";
	m_strReferer = "";
	m_strComment = "";
	m_strCookies = "";
	m_strPostData = "";
}

CFDMUrl::~CFDMUrl()
{
  
}

NS_IMETHODIMP CFDMUrl::GetUrl(PRUnichar * *aUrl)
{
    *aUrl = (PRUnichar*) nsMemory::Clone ((LPWSTR)m_strUrl ? (LPWSTR)m_strUrl : L"", 
		(m_strUrl.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetUrl(const PRUnichar * aUrl)
{
	m_strUrl = aUrl;

	vmsHttpRedirectList::o ().Lock (true);
	int nIndex = vmsHttpRedirectList::o ().findRedirectIndex (aUrl);
	if (nIndex != -1)
		m_wstrOriginalUrl = vmsHttpRedirectList::o ().getRedirect (nIndex)->wstrOriginalUrl;
	vmsHttpRedirectList::o ().Lock (false);

	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetReferer(PRUnichar * *aReferer)
{
    *aReferer = (PRUnichar*) nsMemory::Clone ((LPWSTR)m_strReferer ? (LPWSTR)m_strReferer : L"", 
		(m_strReferer.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetReferer(const PRUnichar * aReferer)
{
	if (!aReferer)
	{
		m_strReferer = L"";
		return NS_OK;
	}
	if (_wcsicmp (aReferer, L"about:blank"))
		m_strReferer = aReferer;
	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetComment(PRUnichar * *aComment)
{
    *aComment = (PRUnichar*) nsMemory::Clone ((LPWSTR)m_strComment ? (LPWSTR)m_strComment : L"", 
		(m_strComment.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetComment(const PRUnichar * aComment)
{
	m_strComment = aComment;
    return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetCookies(PRUnichar * *aCookies)
{
    *aCookies = (PRUnichar*) nsMemory::Clone ((LPWSTR)m_strCookies ? (LPWSTR)m_strCookies : L"", 
		(m_strCookies.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetCookies(const PRUnichar * aCookies)
{
    m_strCookies = aCookies;
	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::GetPostData(PRUnichar * *aPostData)
{
	*aPostData = (PRUnichar*) nsMemory::Clone ((LPWSTR)m_strPostData ? (LPWSTR)m_strPostData : L"", 
		(m_strPostData.length ()+1) * sizeof (wchar_t));
	return NS_OK;
}

NS_IMETHODIMP CFDMUrl::SetPostData(const PRUnichar * aPostData)
{
	if (aPostData)
		m_strPostData = aPostData;
	else
		m_strPostData = L"";

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
	m_wstrOriginalUrl = aUrl;
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
	m_wstrUserAgent = aUrl;
	return NS_OK;
}