/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

struct vmsAppVersionPart
{
	DWORD dwVal;
	int cPreZeros; 

	vmsAppVersionPart (LPCTSTR ptsz) 
	{
		fromString (ptsz);
	}

	void fromString (LPCTSTR ptsz)
	{
		if (ptsz)
		{
			dwVal = _ttoi (ptsz);
			cPreZeros = 0;
			if (dwVal)
			{
				while (*ptsz++ == '0')
					cPreZeros++;
			}
		}
		else
		{
			dwVal = 0; 
			cPreZeros = 0;
		}
	}

	tstring toString () const 
	{
		tstring tstr;
		for (int i = 0; i < cPreZeros; i++)
			tstr += '0';
		TCHAR tsz [100] = _T ("");
		tstr += _itot (dwVal, tsz, 10);
		return tstr;
	}

	vmsAppVersionPart (DWORD dw = 0) 
	{
		dwVal = dw;
		cPreZeros = 0;
	}

	BOOL operator < (const vmsAppVersionPart &o) const {
		return cPreZeros != o.cPreZeros && dwVal != 0 && o.dwVal != 0 ? 
			cPreZeros > o.cPreZeros : dwVal < o.dwVal;
	}

	BOOL operator == (const vmsAppVersionPart &o) const {
		return dwVal == o.dwVal && cPreZeros == o.cPreZeros;
	}

	BOOL operator != (const vmsAppVersionPart &o) const {return !operator == (o);}

	BOOL operator != (DWORD dw) const { return dwVal != dw || cPreZeros != 0;}
};

enum vmsAppVersionReleaseType
{
	RT_RELEASE,
	RT_ALPHA,
	RT_BETA,
	RC_RC,
	RC_UNKNOWN,
};

class vmsAppVersion : public std::vector <vmsAppVersionPart>
{
public:
	enum CompareType 
	{
		CT_INTEGER,  
		CT_FLOATING, 
	};

public:
	void toFloat (float *pfMajor , float *pfMinor ) const
	{
		if (pfMajor)
			*pfMajor = 0;
		if (pfMinor)
			*pfMinor = 0;

		if (empty ())
			return;

		

		if (size () == 1)
		{
			if (pfMajor)
				*pfMajor = (float)at (0).dwVal;
			return;
		}
		else
		{
			if (pfMajor)
			{
				tstringstream tss;
				tss.imbue (std::locale ("C"));
				tss << at (0).toString () << _T(".") << at (1).toString ();
				tss >> *pfMajor;
			}
		}

		

		if (!pfMinor)
			return;

		if (size () == 3)
		{
			*pfMinor = (float)at (2).dwVal;
			return;
		}
		else if (size () > 3)
		{
			tstringstream tss;
			tss.imbue (std::locale ("C"));
			tss << at (2).toString () << _T (".") << at (3).toString ();
			tss >> *pfMinor;
		}
	}

	int CompareBaseVersions (const vmsAppVersion &ver, CompareType enCT) const
	{
		if (empty () && ver.empty ())
			return 0;
		if (empty () && ver.empty () == false)
			return -1;
		if (empty () == false && ver.empty ())
			return 1;

		if (enCT == CT_INTEGER)
		{
			int c = size () > 1 && ver.size () > 1 ? 2 : 1;
			for (int i = 0; i < c; i++)
			{
				if (ver [i] < at (i))
					return 1;
				else if (at (i) < ver [i])
					return -1;
			}
			return 0;
		}

		assert (enCT == CT_FLOATING);

		float f0mj, f1mj;
		toFloat (&f0mj, NULL);
		ver.toFloat (&f1mj, NULL);

		if (f0mj < f1mj)
			return -1;
		if (f0mj == f1mj)
			return 0;
		return 1;
	}

	int CompareVersions (const vmsAppVersion &ver, CompareType enCT) const
	{
		if (empty () && ver.empty ())
			return 0;
		if (empty () && ver.empty () == false)
			return -1;
		if (empty () == false && ver.empty ())
			return 1;

		if (enCT == CT_INTEGER)
		{
			if (*((vector<vmsAppVersionPart>*)this) == *(vector<vmsAppVersionPart>*)&ver)
				return 0;

			
			if (size () != ver.size ())
			{
				const vmsAppVersion &verMinSize = size () < ver.size () ? *this : ver;
				const vmsAppVersion &verMaxSize = size () > ver.size () ? *this : ver;
				size_t i;
				for (i = verMinSize.size (); i < verMaxSize.size (); i++)
				{
					if (verMaxSize [i].dwVal)
						break;
				}
				if (verMaxSize.size () == i)
				{
					for (i = 0; i < verMinSize.size (); i++)
					{
						if (verMinSize [i] != verMaxSize [i])
							break;
					}
					if (i == verMinSize.size ())
						return 0;
				}
			}

			if (*((vector<vmsAppVersionPart>*)this) < *(vector<vmsAppVersionPart>*)&ver)
				return -1;

			return 1;
		}

		assert (enCT == CT_FLOATING);

		float f0mj = 0, f0mn = 0, f1mj = 0, f1mn = 0;
		toFloat (&f0mj, &f0mn);
		ver.toFloat (&f1mj, &f1mn);

		if (f0mj < f1mj)
			return -1;
		if (f0mj > f1mj)
			return 1;

		if (f0mn < f1mn)
			return -1;
		if (f0mn > f1mn)
			return 1;		

		return 0;
	}

	void clear()
	{
		vector <vmsAppVersionPart>::clear ();
		m_tstrPostVersion = _T ("");
	}

	tstring ToString() const
	{
		tstring tstrVer;

		size_t s = sizeExcludingUselessZeroes ();

		for (size_t i = 0; i < s; i++)
		{
			if (!tstrVer.empty ())
				tstrVer += _T (".");
			tstrVer += at (i).toString ();
		}
		tstrVer += m_tstrPostVersion;
		return tstrVer;
	}

	BOOL FromString (LPCTSTR ptszVersion)
	{
		clear ();

		if (ptszVersion == NULL || *ptszVersion == 0)
			return FALSE;

		while (*ptszVersion == ' ')
			ptszVersion++;

		skipDateBeforeVersion (ptszVersion);

		if (*ptszVersion == 'v' || *ptszVersion == 'V')
			ptszVersion++;

		while (*ptszVersion)
		{
			while (*ptszVersion == ' ')
				ptszVersion++;
			tstring tstrVer;
			while (_istdigit (*ptszVersion))
				tstrVer += *ptszVersion++;
			if (tstrVer.empty ())
				break;
			while (*ptszVersion == ' ')
				ptszVersion++;
			if (*ptszVersion == '.' || *ptszVersion == ',')
				ptszVersion++;
			push_back (vmsAppVersionPart (tstrVer.c_str ()));
		}

		if (size () && *ptszVersion)
		{
			while (ptszVersion [-1] == ' ')
				ptszVersion--;
			m_tstrPostVersion = ptszVersion;
		}

		RemoveUselessZeroes ();

		return size () != 0;
	}

	void RemoveUselessZeroes ()
	{
		resize (sizeExcludingUselessZeroes ());
	}

	size_t sizeExcludingUselessZeroes () const
	{
		size_t s = size ();
		while (s > 2 && !at (s-1).dwVal)
			s--;
		return s;
	}

	tstring m_tstrPostVersion; 

	vmsAppVersion()
	{

	}

	virtual ~vmsAppVersion()
	{

	}

protected:
	static void skipDateBeforeVersion (LPCTSTR &ptsz)
	{
		LPCTSTR ptsz2 = ptsz;
		while (_istdigit (*ptsz2))
			ptsz2++;
		if (*ptsz2++ != '/')
			return;
		while (_istdigit (*ptsz2))
			ptsz2++;
		if (*ptsz2++ != '/')
			return;
		while (_istdigit (*ptsz2))
			ptsz2++;
		while (*ptsz2 == ' ')
			ptsz2++;
		ptsz = ptsz2;
	}

private:
	bool operator < (const vmsAppVersion&);
	bool operator > (const vmsAppVersion&);
};

