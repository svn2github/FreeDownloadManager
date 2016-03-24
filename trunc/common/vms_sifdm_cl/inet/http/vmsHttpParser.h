/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsHttpParser  
{
public:
	struct HdrField
	{
		std::string strName;
		std::string strValue;
	};

public:
	const HdrField* FieldByName (LPCSTR pszName) const
	{
		for (size_t i = 0; i < m_vFields.size (); i++)
		{
			if (stricmp (m_vFields [i].strName.c_str (), pszName) == 0)
				return &m_vFields [i];
		}

		return NULL;
	}

	void ParseHeader (LPCSTR pszHdr)
	{
		m_vFields.clear ();

		pszHdr = strstr (pszHdr, "\r\n"); 
		if (!pszHdr)
			return;
		pszHdr += 2;

		HdrField hdrf;
		while (ExtractHdrField (pszHdr, hdrf))
			m_vFields.push_back (hdrf);
	}

	vmsHttpParser()
	{
	}

	virtual ~vmsHttpParser()
	{
	}

protected:
	static void ExtractFieldValue (LPCSTR pszHdr, LPCSTR pszFieldName, std::string *pstrResult)
	{
		*pstrResult = "";
		std::string str = "\r\n";
		str += pszFieldName;
		str += ':';
		LPCSTR psz = strstr (pszHdr, str.c_str ());
		if (psz == NULL)
			return;
		psz += str.length ();
		while (*psz == ' ')
			psz++;
		while (*psz && *psz != '\r' && *psz != '\n')
			*pstrResult += *psz++;
	}

	bool ExtractHdrField (LPCSTR &pszHdr, HdrField &hdrf)
	{
		std::string strName;
		std::string strValue;

		while (*pszHdr != ':' && *pszHdr != 0)
			strName += *pszHdr++;

		if (*pszHdr == 0)
			return false;

		pszHdr++;
		while (*pszHdr == ' ')
			pszHdr++;

		while (*pszHdr != 0 && *pszHdr != '\r' && *pszHdr != '\n')
			strValue += *pszHdr++;

		if (*pszHdr == 0)
			return false;
		if (pszHdr [0] != '\r' || pszHdr [1] != '\n')
			return false;
		pszHdr += 2;

		hdrf.strName = strName;
		hdrf.strValue = strValue;

		return true;
	}

	std::vector <HdrField> m_vFields;
};

