/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

const char HEX2DEC [256] =
{
	
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,

	 -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

inline std::string PercentDecode (const std::string & sSrc)
{
	
	
	

	const unsigned char * pSrc = (const unsigned char *)sSrc.c_str ();
	const int SRC_LEN = (int)sSrc.length ();
	const unsigned char * const SRC_END = pSrc + SRC_LEN;
	const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   

	char * const pStart = new char [SRC_LEN];
	char * pEnd = pStart;

	while (pSrc < SRC_LAST_DEC)
	{
		if (*pSrc == '%')
		{
			char dec1, dec2;
			if (-1 != (dec1 = HEX2DEC [*(pSrc + 1)])
				&& -1 != (dec2 = HEX2DEC [*(pSrc + 2)]))
			{
				*pEnd++ = (dec1 << 4) + dec2;
				pSrc += 3;
				continue;
			}
		}

		*pEnd++ = *pSrc++;
	}

	
	while (pSrc < SRC_END)
		*pEnd++ = *pSrc++;

	std::string sResult (pStart, pEnd);
	delete [] pStart;
	return sResult;
}

inline std::string PercentEncode (const void *data, size_t SRC_LEN, const char SAFE [256])
{
	const char DEC2HEX [16 + 1] = "0123456789ABCDEF";
	const unsigned char * pSrc = (const unsigned char *)data;
	unsigned char * const pStart = new unsigned char [SRC_LEN * 3];
	unsigned char * pEnd = pStart;
	const unsigned char * const SRC_END = pSrc + SRC_LEN;

	for (; pSrc < SRC_END; ++pSrc)
	{
		if (SAFE [*pSrc])
			*pEnd++ = *pSrc;
		else
		{
			
			*pEnd++ = '%';
			*pEnd++ = DEC2HEX [*pSrc >> 4];
			*pEnd++ = DEC2HEX [*pSrc & 0x0F];
		}
	}

	std::string sResult ((char *)pStart, (char *)pEnd);
	delete [] pStart;
	return sResult;
}

inline std::string PercentEncode (const std::string & sSrc, const char SAFE [256])
{
	if (sSrc.empty ())
		return sSrc;
	return PercentEncode (&sSrc.front (), sSrc.length (), SAFE);
}