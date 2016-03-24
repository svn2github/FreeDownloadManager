/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
	BYTE	bWidth;               
	BYTE	bHeight;              
	BYTE	bColorCount;          
	BYTE	bReserved;            
	WORD	wPlanes;              
	WORD	wBitCount;            
	DWORD	dwBytesInRes;         
	WORD	nID;                  
} MEMICONDIRENTRY, *LPMEMICONDIRENTRY;
typedef struct
{
	WORD			idReserved;   
	WORD			idType;       
	WORD			idCount;      
	MEMICONDIRENTRY	idEntries [1]; 
} MEMICONDIR, *LPMEMICONDIR;
#pragma pack( pop )

typedef struct
{
	UINT			Width, Height, Colors; 
	LPBYTE			lpBits;                
	DWORD			dwNumBytes;            
	LPBITMAPINFO	lpbi;                  
	LPBYTE			lpXOR;                 
	LPBYTE			lpAND;                 
} ICONIMAGE, *LPICONIMAGE;
typedef struct
{
	BOOL		bHasChanged;                     
	UINT		nNumImages;                      
	ICONIMAGE	IconImages [1];                   
} ICONRESOURCE, *LPICONRESOURCE;

typedef struct
{
	BYTE	bWidth;               
	BYTE	bHeight;              
	BYTE	bColorCount;          
	BYTE	bReserved;            
	WORD	wPlanes;              
	WORD	wBitCount;            
	DWORD	dwBytesInRes;         
	DWORD	dwImageOffset;        
} ICONDIRENTRY, *LPICONDIRENTRY;
typedef struct
{
	WORD			idReserved;   
	WORD			idType;       
	WORD			idCount;      
	ICONDIRENTRY	idEntries [1]; 
} ICONDIR, *LPICONDIR;

#define ICON_EXCTRACTOR_WIDTHBYTES(bits)      ((((bits) + 31)>>5)<<2)

class CIconExtractor
{

public:

	

	DWORD ExtractIconFromExe (LPCTSTR SourceEXE,
		LPCTSTR TargetICON,
		int IconIndex)
	{
		LPICONRESOURCE      lpIR = NULL;
		HINSTANCE        	hLibrary;
		DWORD               ret;

		

		
		if ((hLibrary = LoadLibraryEx (SourceEXE, NULL, LOAD_LIBRARY_AS_DATAFILE)) == NULL)
			return GetLastError ();

		HRSRC        	hRsrc = NULL;
		HGLOBAL        	hGlobal = NULL;
		LPMEMICONDIR    lpIcon = NULL;
		

		m_vIconsInfo.clear ();
		ret = EnumResourceNames (hLibrary, RT_GROUP_ICON, EnumResNameProc, (LONG_PTR)this);

		if (!ret)
		{
			ret = GetLastError ();
			FreeLibrary (hLibrary);
			return ret;
		}

		if (IconIndex < 0)
		{
			IconIndex *= -1;

			size_t i = 0;
			for (; i < m_vIconsInfo.size (); i++)
			{
				if (m_vIconsInfo [i].strIconName.empty () && m_vIconsInfo [i].uIconID == IconIndex)
					break;
			}
			if (i != m_vIconsInfo.size ())
				IconIndex = (int)i;
			else
				IconIndex = 0;
		}
		else
		{
			assert (IconIndex < (int)m_vIconsInfo.size ());
			if (IconIndex >= (int)m_vIconsInfo.size ())
				IconIndex = 0;
		}

		if ((int)m_vIconsInfo.size () > IconIndex)
		{
			
			

			if (m_vIconsInfo [IconIndex].strIconName.empty ())
				hRsrc = FindResource (hLibrary, MAKEINTRESOURCE (m_vIconsInfo [IconIndex].uIconID), RT_GROUP_ICON);
			else
				hRsrc = FindResource (hLibrary, m_vIconsInfo [IconIndex].strIconName.c_str (), RT_GROUP_ICON);
		}

		if (hRsrc == NULL)
		{
			ret = GetLastError ();
			FreeLibrary (hLibrary);
			return ret;
		}

		if (SizeofResource (hLibrary, hRsrc) < sizeof (MEMICONDIR))
		{
			FreeLibrary (hLibrary);
			return E_UNEXPECTED;
		}

		if ((hGlobal = LoadResource (hLibrary, hRsrc)) == NULL)
		{
			ret = GetLastError ();
			FreeLibrary (hLibrary);
			return ret;
		}
		if ((lpIcon = (LPMEMICONDIR)LockResource (hGlobal)) == NULL)
		{
			ret = GetLastError ();
			FreeLibrary (hLibrary);
			return ret;
		}

		if (IsBadReadPtr (lpIcon, sizeof (MEMICONDIR)))
		{
			FreeLibrary (hLibrary);
			return E_UNEXPECTED;
		}

		
		if ((lpIR = (LPICONRESOURCE)malloc (sizeof (ICONRESOURCE) + ((lpIcon->idCount - 1) * sizeof (ICONIMAGE)))) == NULL)
		{
			ret = GetLastError ();
			FreeLibrary (hLibrary);
			return ret;
		}

		
		lpIR->nNumImages = lpIcon->idCount;

		
		for (UINT i = 0; i < lpIR->nNumImages; i++)
		{
			
			if ((hRsrc = FindResource (hLibrary, MAKEINTRESOURCE (lpIcon->idEntries [i].nID), RT_ICON)) == NULL)
			{
				ret = GetLastError ();
				FreeLibrary (hLibrary);
				free (lpIR);
				return ret;
			}
			if ((hGlobal = LoadResource (hLibrary, hRsrc)) == NULL)
			{
				ret = GetLastError ();
				FreeLibrary (hLibrary);
				free (lpIR);
				return ret;
			}
			
			lpIR->IconImages [i].dwNumBytes = SizeofResource (hLibrary, hRsrc);

			lpIR->IconImages [i].lpBits = (LPBYTE)malloc (lpIR->IconImages [i].dwNumBytes);
			if (lpIR->IconImages [i].lpBits == NULL)
			{
				ret = GetLastError ();
				free (lpIR);
				return ret;
			}

			memcpy (lpIR->IconImages [i].lpBits, LockResource (hGlobal), lpIR->IconImages [i].dwNumBytes);

			
			if (!AdjustIconImagePointers (&(lpIR->IconImages [i])))
			{
				ret = GetLastError ();
				FreeLibrary (hLibrary);
				free (lpIR);
				return ret;
			}
		}

		FreeLibrary (hLibrary);
		ret = WriteIconToICOFile (lpIR, TargetICON);

		free (lpIR);
		return (ret);
	}

	CIconExtractor ()
	{
	}

protected:
	BOOL AddResource (LPCTSTR lpszType, LPTSTR lpszName)
	{
		if (lpszType == RT_GROUP_ICON)
		{
			__inc_IconInfo icon;

			if ((ULONG)lpszName < 65536)
				icon.uIconID = (ULONG)lpszName;
			else 
				icon.strIconName = lpszName;

			m_vIconsInfo.push_back (icon);
		}

		return TRUE;
	}

private:
	DWORD WriteIconToICOFile (LPICONRESOURCE lpIR, LPCTSTR szFileName)
	{
		HANDLE    	hFile;
		UINT        i;
		DWORD    	dwBytesWritten;
		DWORD		ret = 0;

		
		if ((hFile = CreateFile (szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
			return GetLastError ();

		
		if (WriteICOHeader (hFile, lpIR->nNumImages))
		{
			ret = GetLastError ();
			CloseHandle (hFile);
			return ret;
		}
		
		for (i = 0; i < lpIR->nNumImages; i++)
		{
			ICONDIRENTRY    ide;

			
			ide.bWidth = lpIR->IconImages [i].Width;
			ide.bHeight = lpIR->IconImages [i].Height;
			ide.bReserved = 0;
			ide.wPlanes = lpIR->IconImages [i].lpbi->bmiHeader.biPlanes;
			ide.wBitCount = lpIR->IconImages [i].lpbi->bmiHeader.biBitCount;
			if ((ide.wPlanes * ide.wBitCount) >= 8)
				ide.bColorCount = 0;
			else
				ide.bColorCount = 1 << (ide.wPlanes * ide.wBitCount);
			ide.dwBytesInRes = lpIR->IconImages [i].dwNumBytes;
			ide.dwImageOffset = CalculateImageOffset (lpIR, i);

			
			if (!WriteFile (hFile, &ide, sizeof (ICONDIRENTRY), &dwBytesWritten, NULL))
				return GetLastError ();

			
			if (dwBytesWritten != sizeof (ICONDIRENTRY))
				return GetLastError ();
		}
		
		for (i = 0; i < lpIR->nNumImages; i++)
		{
			DWORD dwTemp = lpIR->IconImages [i].lpbi->bmiHeader.biSizeImage;

			
			lpIR->IconImages [i].lpbi->bmiHeader.biSizeImage = 0;
			
			if (!WriteFile (hFile, lpIR->IconImages [i].lpBits, lpIR->IconImages [i].dwNumBytes, &dwBytesWritten, NULL))
				return GetLastError ();

			if (dwBytesWritten != lpIR->IconImages [i].dwNumBytes)
				return GetLastError ();

			
			lpIR->IconImages [i].lpbi->bmiHeader.biSizeImage = dwTemp;
		}
		CloseHandle (hFile);
		return NO_ERROR;
	}

	BOOL AdjustIconImagePointers (LPICONIMAGE lpImage)
	{
		
		if (lpImage == NULL)
			return FALSE;
		
		lpImage->lpbi = (LPBITMAPINFO)lpImage->lpBits;
		
		lpImage->Width = lpImage->lpbi->bmiHeader.biWidth;
		
		lpImage->Height = (lpImage->lpbi->bmiHeader.biHeight) / 2;
		
		lpImage->Colors = lpImage->lpbi->bmiHeader.biPlanes * lpImage->lpbi->bmiHeader.biBitCount;
		
		lpImage->lpXOR = (PBYTE)FindDIBBits ((LPSTR)lpImage->lpbi);
		
		lpImage->lpAND = lpImage->lpXOR + (lpImage->Height*BytesPerLine ((LPBITMAPINFOHEADER)(lpImage->lpbi)));
		return TRUE;
	}

	LPSTR FindDIBBits (LPSTR lpbi)
	{
		return ( lpbi + *(LPDWORD)lpbi + PaletteSize( lpbi ) );
	}

	WORD DIBNumColors (LPSTR lpbi)
	{
		WORD wBitCount;
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;

		if (dwClrUsed)
			return (WORD)dwClrUsed;

		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;

		switch (wBitCount)
		{
		case 1: return 2;
		case 4: return 16;
		case 8:	return 256;
		default:return 0;
		}
		return 0;
	}

	WORD PaletteSize (LPSTR lpbi)
	{
		return ( DIBNumColors( lpbi ) * sizeof( RGBQUAD ) );
	}

	DWORD BytesPerLine (LPBITMAPINFOHEADER lpBMIH)
	{
		return ICON_EXCTRACTOR_WIDTHBYTES(lpBMIH->biWidth * lpBMIH->biPlanes * lpBMIH->biBitCount);
	}

	DWORD WriteICOHeader (HANDLE hFile, UINT nNumEntries)
	{
		WORD    Output;
		DWORD	dwBytesWritten;

		
		Output = 0;
		if (!WriteFile (hFile, &Output, sizeof (WORD), &dwBytesWritten, NULL))
			return GetLastError ();
		
		if (dwBytesWritten != sizeof (WORD))
			return GetLastError ();
		
		Output = 1;
		if (!WriteFile (hFile, &Output, sizeof (WORD), &dwBytesWritten, NULL))
			return GetLastError ();
		
		if (dwBytesWritten != sizeof (WORD))
			return GetLastError ();
		
		Output = (WORD)nNumEntries;
		if (!WriteFile (hFile, &Output, sizeof (WORD), &dwBytesWritten, NULL))
			return GetLastError ();
		
		if (dwBytesWritten != sizeof (WORD))
			return GetLastError ();

		return NO_ERROR;
	}

	DWORD CalculateImageOffset (LPICONRESOURCE lpIR, UINT nIndex)
	{
		DWORD	dwSize;
		UINT    i;

		
		dwSize = 3 * sizeof (WORD);
		
		dwSize += lpIR->nNumImages * sizeof (ICONDIRENTRY);
		
		for (i = 0; i < nIndex; i++)
			dwSize += lpIR->IconImages [i].dwNumBytes;
		
		return dwSize;
	}

	struct __inc_IconInfo {
		ULONG uIconID; 
		std::wstring strIconName;
	};
	std::vector <__inc_IconInfo> m_vIconsInfo;

	BOOL static CALLBACK EnumResNameProc (
		HMODULE hModule,   
		LPCTSTR lpszType,  
		LPTSTR lpszName,   
		LONG_PTR lParam)    
	{
		CIconExtractor *ptr = (CIconExtractor*)lParam;
		return ptr->AddResource (lpszType, lpszName);
	}
};
