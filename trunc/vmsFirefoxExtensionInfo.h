/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#pragma once

#include "sqlite/sqlite3.h"

class vmsFirefoxExtensionInfo
{
public:

	vmsFirefoxExtensionInfo(void) : 
	  m_iSoftDisabled (0), m_iAppDisabled (0), m_iUserDisabled (0)
	{
	}

	virtual ~vmsFirefoxExtensionInfo(void)
	{
	}

	bool Read (LPCTSTR ptszDbFile, LPCTSTR ptszExtensionId)
	{
		USES_CONVERSION;
		sqlite3 *db = NULL;
		if (SQLITE_OK != sqlite3_open16 (T2CW (ptszDbFile), &db))
			return false;

		tstring tstrQ = _T ("SELECT * FROM addon WHERE id==\"");
		tstrQ += ptszExtensionId;
		tstrQ += "\";";

		bool bOK = SQLITE_OK == sqlite3_exec (db, _t2utf8 (tstrQ.c_str ()).c_str (), _onAddonRecordEnum, this, NULL);

		sqlite3_close (db);

		return bOK;
	}

public:
	int m_iSoftDisabled, m_iAppDisabled, m_iUserDisabled;

protected:
	static int _onAddonRecordEnum (void *param, int coln, char **rows, char **colnm)
	{
		vmsFirefoxExtensionInfo *pthis = (vmsFirefoxExtensionInfo*) param;

		for (int i = 0; i < coln; i++)
		{
			if (!stricmp (colnm [i], "softDisabled"))
				pthis->m_iSoftDisabled = atoi (rows [i]);

			else if (!stricmp (colnm [i], "appDisabled"))
				pthis->m_iAppDisabled = atoi (rows [i]);

			else if (!stricmp (colnm [i], "userDisabled"))
				pthis->m_iUserDisabled = atoi (rows [i]);
		}

		return 0;
	}
};

