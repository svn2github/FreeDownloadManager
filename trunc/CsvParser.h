/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef CSVPARSER_INCLUDED_FILE
#define CSVPARSER_INCLUDED_FILE

class CCsvParser
{
public:
	CCsvParser();
	virtual ~CCsvParser();
	void Init(const CStringA& sStr, const CStringA& sTextQulifier, const CStringA& sDelimiter);
	bool ParseNextRecord();
	bool GetNextField(CStringA& sValue);

private:
	void Sgets();
	int findDelimiter(bool& bIsQuote, int& nClosingQuotePos);
	int countQuote(int nStartChar, const CStringA& sValue);
	void procLastField(bool bIsQuote, int nClosingQuotePos, CStringA& sValue);
	void procLastButOneField(bool bIsQuote, int nClosingQuotePos, CStringA& sValue);
	void procField(int nDelimPos, bool bIsQuote, int nClosingQuotePos, CStringA& sValue);
	void decodeValue(bool bIsQuote, int nClosingQuotePos, CStringA& sValue);
	int findSingleQuote(int nStartChar, const CStringA& sValue);

	CStringA m_sCsvContent; 
	CStringA m_sRow; 
	CStringA m_sTextQulifier;
	CStringA m_sDelimiter;
	int m_nPos;
};

#endif
