/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsCookie.h"        

vmsCookie::vmsCookie()
{

}

vmsCookie::~vmsCookie()
{

}

void vmsCookie::Set(LPCSTR pszCookie)
{
	m_vItems.clear ();

	if (pszCookie == NULL)
		return;

	#define skip_spaces while (*pszCookie == ' ') pszCookie++

	while (*pszCookie)
	{
		skip_spaces;
		vmsSingleCookie c;
		while (*pszCookie && *pszCookie != '=')
			c.strName += *pszCookie++;
		if (*pszCookie)
			pszCookie++;

		while (*pszCookie && *pszCookie != ';')
			c.strValue += *pszCookie++;
		if (*pszCookie)
			pszCookie++;

		if (!c.strName.IsEmpty ())
			m_vItems.add (c);			
	}
}

int vmsCookie::get_ItemCount()
{
	return m_vItems.size ();
}

vmsSingleCookie* vmsCookie::get_Item(int nIndex)
{
	return &m_vItems [nIndex];
}

int vmsCookie::FindItemByName(LPCSTR pszName)
{
	for (int i = 0; i < m_vItems.size (); i++)
	{
		if (lstrcmp (m_vItems [i].strName, pszName) == 0)
			return i;
	}

	return -1;
}

fsString vmsCookie::toString()
{
	fsString str;

	for (int i = 0; i < m_vItems.size (); i++)
	{
		if (i)
			str += "; ";
		str += m_vItems [i].strName;
		str += '=';
		str += m_vItems [i].strValue;
	}

	return str;
}

int vmsCookie::GetCommonItemCount(vmsCookie *pc)
{
	int c = 0;

	for (int i = 0; i < pc->get_ItemCount (); i++)
	{
		int n = FindItemByName (pc->get_Item (i)->strName);
		if (n != -1)
		{
			if (pc->get_Item (i)->strValue == m_vItems [n].strValue)
				c++;
		}
	}

	return c;
}

void vmsCookie::Add(vmsCookie *pc)
{
	for (int i = 0; i < pc->get_ItemCount (); i++)
	{
		if (-1 == FindItemByName (pc->get_Item (i)->strName))
			m_vItems.add (*pc->get_Item (i));
	}
}
