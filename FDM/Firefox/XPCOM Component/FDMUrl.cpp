/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#include "FDMUrl.h"
#include <windows.h>

NS_IMPL_ISUPPORTS1(CFDMUrl, IFDMUrl)

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
    *aUrl = new PRUnichar [m_strUrl.length () + 1];
	wcscpy (*aUrl, m_strUrl);
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetUrl(const PRUnichar * aUrl)
{
	m_strUrl = aUrl;
	return NS_OK;
}  

NS_IMETHODIMP CFDMUrl::GetReferer(PRUnichar * *aReferer)
{
    *aReferer = new PRUnichar [m_strReferer.length () + 1];
	wcscpy (*aReferer, m_strReferer);
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetReferer(const PRUnichar * aReferer)
{
    m_strReferer = aReferer;
	return NS_OK;
}  

NS_IMETHODIMP CFDMUrl::GetComment(PRUnichar * *aComment)
{
    *aComment = new PRUnichar [m_strComment.length () + 1];
	wcscpy (*aComment, m_strComment);
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetComment(const PRUnichar * aComment)
{
	m_strComment = aComment;
    return NS_OK;
}  

NS_IMETHODIMP CFDMUrl::GetCookies(PRUnichar * *aCookies)
{
    *aCookies = new PRUnichar [m_strCookies.length () + 1];
	wcscpy (*aCookies, m_strCookies);
	return NS_OK;
}
NS_IMETHODIMP CFDMUrl::SetCookies(const PRUnichar * aCookies)
{
    m_strCookies = aCookies;
	return NS_OK;
}  

NS_IMETHODIMP CFDMUrl::GetPostData(PRUnichar * *aPostData)
{
    *aPostData = new PRUnichar [m_strPostData.length () + 1];
	wcscpy (*aPostData, m_strPostData);
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
