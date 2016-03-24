/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsHttpBuffer
{
private:
	
	
	vmsHttpBuffer (const vmsHttpBuffer&);
	const vmsHttpBuffer& operator= (const vmsHttpBuffer&);
public:
	enum ThrowReason
	{
		NeedMoreData,
		ParseError,
	};
	struct Position
	{
		const unsigned char* pb;
		size_t nSize;
	};
public:
	vmsHttpBuffer (const unsigned char *pb, size_t nSize) : m_pb (pb), m_nSize (nSize), 
		m_enDefaultThrowReason (NeedMoreData), m_enThrowReason (NeedMoreData) {}

	void SavePosition (Position& pos) const {pos.pb = m_pb; pos.nSize = m_nSize;}
	void RestorePosition (const Position& pos) {m_pb = pos.pb; m_nSize = pos.nSize;}

	ThrowReason getDefaultThrowReason () const {return m_enDefaultThrowReason;}
	void setDefaultThrowReason (ThrowReason en) {m_enDefaultThrowReason = en;}

	ThrowReason getThrowReason () const {return m_enThrowReason;}

	bool isEndOfBufferReached () const {return !m_nSize;}
	BYTE getByte (bool bPeek = false)
	{
		if (!m_nSize) 
			Throw ();
		if (bPeek)
			return *m_pb;
		m_nSize--;
		return *m_pb++;
	}
	void getString (size_t nLen, std::string &str)
	{
		if (m_nSize < nLen)
			Throw ();
		str.assign ((LPCSTR)m_pb, nLen);
		m_pb += nLen;
		m_nSize -= nLen;
	}
	bool SkeepBytes (size_t nAmount)
	{
		if (nAmount > m_nSize)
			Throw ();
		m_pb += nAmount;
		m_nSize -= nAmount;
		return true;
	}
	size_t getSizeAvail () const {return m_nSize;}
	const unsigned char* getPointer () const {return m_pb;}
	void Throw (ThrowReason en) {m_enThrowReason = en; throw 0;}
	void Throw () {Throw (m_enDefaultThrowReason);}
protected:
	const unsigned char* m_pb;
	size_t m_nSize;
	ThrowReason m_enDefaultThrowReason, m_enThrowReason;
};

class vmsHttpBufferStateAutoSaveAndRestore
{
public:
	enum StateItems
	{
		Position				= 1,
		DefaultThrowReason		= 1 << 1,
		All						= 0xffffffff
	};
public:
	vmsHttpBufferStateAutoSaveAndRestore (vmsHttpBuffer& buff, DWORD dwWhat = All)
	{
		m_dwWhat = dwWhat;
		m_pBuff = &buff;
		if (m_dwWhat & Position)
			buff.SavePosition (m_pos);
		if (m_dwWhat & DefaultThrowReason)
			m_enTR = buff.getDefaultThrowReason ();
	}

	~vmsHttpBufferStateAutoSaveAndRestore ()
	{
		if (m_dwWhat & Position)
			m_pBuff->RestorePosition (m_pos);
		if (m_dwWhat & DefaultThrowReason)
			m_pBuff->setDefaultThrowReason (m_enTR);
	}

protected:
	vmsHttpBuffer *m_pBuff;
	vmsHttpBuffer::Position m_pos;
	vmsHttpBuffer::ThrowReason m_enTR;
	DWORD m_dwWhat; 
};

#define HttpTspecials "()<>@,;:\\\"/[]?={} \t"

class vmsHttpBufferElements
{
public:
	static bool isHEX (BYTE b) {return isdigit ((char)b) || (b >= 'A' && b <= 'F') || (b >= 'a' && b <= 'f');}
	static bool isChar (BYTE b) {return b < 127;}
	static bool isCTL (BYTE b) {return b < 32 || b == 127;} // 127 - DEL
	static bool isTspecialsChar (BYTE b) {return strchr (HttpTspecials, (char)b) != NULL;}
	static bool isTokenChar (BYTE b) {return isChar (b) && !isCTL (b) && !isTspecialsChar (b);}
	// LWS = [CRLF] 1*( SP | HT )
	static bool isLWS (vmsHttpBuffer& buff, size_t *pnLwsLen) 
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLwsLen = 0;
		size_t nLwsLen = 0;
		if (buff.getSizeAvail () >= 2)
		{
			if (!strncmp ((const char*)buff.getPointer (), "\r\n", 2))
			{
				buff.SkeepBytes (2);
				nLwsLen = 2;
			}
		}
		if (buff.isEndOfBufferReached ())
			buff.Throw ();
		while (buff.getSizeAvail ())
		{
			BYTE b = buff.getByte ();
			if (b != ' ' && b != '\t')
				break;
			nLwsLen++;
		}
		if (nLwsLen < 3)
			return false;
		*pnLwsLen = nLwsLen;
		return true;
	}
	// throws ParseError in case of a failure
	static void ExtractField (vmsHttpBuffer& buff, bool (*pfnIsField)(vmsHttpBuffer&, size_t*), std::string &str)
	{
		size_t nLen = 0;
		if (!pfnIsField (buff, &nLen))
			buff.Throw (buff.ParseError);
		if (nLen)
			buff.getString (nLen, str);
		else
			str = "";
	}
	static void ExtractLWS (vmsHttpBuffer& buff, std::string& str) {ExtractField (buff, isLWS, str);}
	// TEXT = <any OCTET except CTLs, but including LWS>
	static bool isTEXT (vmsHttpBuffer& buff, size_t *pnTextLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnTextLen = 0;
		BYTE b = buff.getByte (true);
		if (!isCTL (b))
		{
			*pnTextLen = 1;
			return true;
		}
		return isLWS (buff, pnTextLen);
	}
	static void ExtractTEXT (vmsHttpBuffer& buff, std::string& str) {ExtractField (buff, isTEXT, str);}
	static bool isQuotedString (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		if (buff.getByte () != '"')
			return false;		
		size_t nLen = 1;
		size_t nLen2 = 0;
		while (buff.getByte (true) != '"' && isTEXT (buff, &nLen2))
		{
			if (nLen2 == 1 && buff.getByte (true) == '\\')
				nLen2++; 
			nLen += nLen2;
			buff.SkeepBytes (nLen2);
			nLen2 = 0;
		}
		if (buff.getByte () != '"')
			return false;
		*pnLen = nLen + 1;
		return true;
	}
	static void ExtractQuotedString (vmsHttpBuffer& buff, std::string& str)
	{
		ExtractField (buff, isQuotedString, str);
		assert (str.length () >= 2);
		if (str.length () < 2)
			return buff.Throw (buff.ParseError);
		
		str.erase (0, 1);
		str.erase (str.length () - 1, 1);
		if (str.empty ())
			return;
		
		size_t nPos = 0;
		while (nPos < str.length () && 
					(nPos = str.find ('\\', nPos)) != std::string::npos)
			str.erase (nPos++, 1);
	}
	static bool isToken (vmsHttpBuffer& buff, size_t *pnLen)
	{
		if (buff.isEndOfBufferReached ())
			buff.Throw (buff.NeedMoreData);
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		while (!buff.isEndOfBufferReached () && isTokenChar (buff.getByte ()))
			(*pnLen)++;
		return *pnLen != 0;
	}
	static void ExtractToken (vmsHttpBuffer& buff, std::string& str) {ExtractField (buff, isToken, str);}
	
	static bool isHttpVersion (vmsHttpBuffer& buff, size_t *pnLen)
	{
		*pnLen = 0;
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		if (buff.getByte () != 'H' || buff.getByte () != 'T' || buff.getByte () != 'T' || buff.getByte () != 'P')
			return false;
		if (buff.getByte () != '/')
			return false;
		BYTE b;
		size_t nLen = 5;
		while (isdigit ((char)(b = buff.getByte ())))
			nLen++;
		if (nLen == 5)
			return false;
		if (b != '.')
			return false;
		nLen++;
		size_t nLenBefore = nLen;
		while (isdigit ((char)(b = buff.getByte ())))
			nLen++;
		if (nLen == nLenBefore)
			return false;
		*pnLen = nLen;
		return true;
	}
	static void ExtractHttpVersion (vmsHttpBuffer& buff, std::string& str) {ExtractField (buff, isHttpVersion, str);}
};

class vmsHttpGenericHeaderField
{
public:
	
	
	void ReadFromBuffer (vmsHttpBuffer& buff)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff, vmsHttpBufferStateAutoSaveAndRestore::DefaultThrowReason);
		buff.setDefaultThrowReason (buff.NeedMoreData);
		m_strName = m_strValue = "";
		if (buff.isEndOfBufferReached ())
			buff.Throw ();
		vmsHttpBufferElements::ExtractToken (buff, m_strName);
		if (buff.getByte () != ':')
			buff.Throw (buff.ParseError);
		while (buff.getByte (true) == ' ')
			buff.getByte ();
		ExtractFieldValue (buff, m_strValue);
		if (buff.getByte () != '\r' || buff.getByte () != '\n')
			buff.Throw (buff.ParseError);
	}

	void setName (LPCSTR psz) {m_strName = psz;}
	void setValue (LPCSTR psz) {m_strValue = psz;}
	const std::string& getName () const {return m_strName;}
	const std::string& getValue () const {return m_strValue;}
	std::string& getName () {return m_strName;}
	std::string& getValue () {return m_strValue;}
	void getValueWoExtraSpaces (std::string& str) const {RemoveExtraWhitespacesFromFieldValue (m_strValue.c_str (), str);}
	std::string getValueWoExtraSpaces () const {std::string str; getValueWoExtraSpaces (str); return str;}

	void toString (std::string &str) const
	{
		str = m_strName;
		str += ':';
		str += m_strValue;
		str += "\r\n";
	}

public:
	
	
	
	
	static bool isFieldValue (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		size_t nLen = 0;
		size_t nLenAdded = 1; 
		while (!buff.isEndOfBufferReached () && nLenAdded)
		{
			nLenAdded = 0;
			if (vmsHttpBufferElements::isTspecialsChar (buff.getByte (true)))
				nLenAdded = 1;
			if (!nLenAdded)
				vmsHttpBufferElements::isQuotedString (buff, &nLenAdded);
			if (!nLenAdded)
				vmsHttpBufferElements::isToken (buff, &nLenAdded);
			if (!nLenAdded)
				vmsHttpBufferElements::isTEXT (buff, &nLenAdded);
			if (nLenAdded)
			{
				nLen += nLenAdded;
				buff.SkeepBytes (nLenAdded);
			}
		} 
		*pnLen = nLen;
		return true;
	}
	static void ExtractFieldValue (vmsHttpBuffer& buff, std::string& str) {vmsHttpBufferElements::ExtractField (buff, isFieldValue, str);}

	static void RemoveExtraWhitespacesFromFieldValue (LPCSTR pszValue, std::string& strResult)
	{
		strResult = "";
		try{
		vmsHttpBuffer buff ((const unsigned char*)pszValue, strlen (pszValue));
		while (!buff.isEndOfBufferReached ())
		{
			size_t nLen = 0;
			if (vmsHttpBufferElements::isQuotedString (buff, &nLen))
			{
				std::string str;
				buff.getString (nLen, str);
				strResult += str;
				continue;
			}
			BYTE b = buff.getByte ();
			if (b != ' ' && b != '\t' && b != '\r' && b != '\n')
			{
				strResult += b;
				continue;
			}
			
			
			if (!strResult.empty () && strResult [strResult.length () - 1] != ' ')
				strResult += ' ';
		}
		}catch (...) {}
	}

protected:
	std::string m_strName;
	std::string m_strValue;
};

class vmsHttpHeaderParser  
{
public:
	const vmsHttpGenericHeaderField* FieldByName (LPCSTR pszName) const
	{
		for (size_t i = 0; i < m_vFields.size (); i++)
		{
			if (_stricmp (m_vFields [i].getName ().c_str (), pszName) == 0)
				return &m_vFields [i];
		}
		return NULL;
	}
	vmsHttpGenericHeaderField* FieldByName (LPCSTR pszName)
	{
		for (size_t i = 0; i < m_vFields.size (); i++)
		{
			if (_stricmp (m_vFields [i].getName ().c_str (), pszName) == 0)
				return &m_vFields [i];
		}
		return NULL;
	}
	void AppendHeader (const vmsHttpGenericHeaderField& hf) {m_vFields.push_back (hf);}
	virtual void ReadHeader (vmsHttpBuffer& buff)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff, vmsHttpBufferStateAutoSaveAndRestore::DefaultThrowReason);
		buff.setDefaultThrowReason (buff.NeedMoreData);

		m_vFields.clear ();

		ReadHeaderFirstLine (buff);

		for (;;)
		{
			if (buff.getSizeAvail () < 2)
				buff.Throw ();
			const unsigned char* pb = buff.getPointer ();
			if (pb [0] == '\r' && pb [1] == '\n')
				break;
			vmsHttpGenericHeaderField hdrf;
			hdrf.ReadFromBuffer (buff);
			m_vFields.push_back (hdrf);
		}
		
		
		buff.getByte ();
		buff.getByte ();
	}
	virtual bool ReadHeader (LPCSTR pszHeader)
	{
		vmsHttpBuffer buff ((const unsigned char*)pszHeader, strlen (pszHeader));
		try {ReadHeader (buff);}catch (...){return false;}
		return true;
	}
	void toString (std::string &str) const 
	{
		toStringFirstLine (str);
		std::string str2;
		for (size_t i = 0; i < m_vFields.size (); i++)
		{
			m_vFields [i].toString (str2);
			str += str2;
		}
		str += "\r\n";
	}

	vmsHttpHeaderParser () {}
	virtual ~vmsHttpHeaderParser () {}

protected:
	virtual void ReadHeaderFirstLine (vmsHttpBuffer& buff) = NULL; 
	virtual void toStringFirstLine (std::string& str) const = NULL;
	std::vector <vmsHttpGenericHeaderField> m_vFields;
};

class vmsHttpURI
{
public:
	
	static bool isURI (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		size_t nLen = 0;
		if (!isAbsoluteURI (buff, &nLen) && !isRelativeURI (buff, &nLen))
			return false;
		buff.SkeepBytes (nLen);
		if (!buff.isEndOfBufferReached ())
		{
			if (buff.getByte () == '#')
			{
				size_t nFragmentLen = 0;
				if (!isFragment (buff, &nFragmentLen))
					return false;
				nLen += nFragmentLen + 1;
			}
		}
		*pnLen = nLen;
		return true;
	}

	static void ExtractURI (vmsHttpBuffer& buff, std::string& str) {vmsHttpBufferElements::ExtractField (buff, isURI, str);}

	
	static bool isAbsoluteURI (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		size_t nLen = 0;
		if (!isScheme (buff, &nLen))
			return false;
		buff.SkeepBytes (nLen);
		if (buff.getByte () != ':')
			return false;
		nLen++;
		size_t nLen2 = 0;
		if (!isAsteriskUcharOrReserved (buff, &nLen2))
			return false;
		nLen += nLen2;
		*pnLen = nLen;
		return true;
	}

	
	static bool isRelativeURI (vmsHttpBuffer& buff, size_t *pnLen)
	{
		return isNetPath (buff, pnLen) || isAbsPath (buff, pnLen) || isRelPath (buff, pnLen);
	}

	
	static bool isAsteriskUcharOrReserved (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		size_t nLen = 0;
		while (!buff.isEndOfBufferReached ())
		{
			size_t nLen2 = 0;
			if (isUchar (buff, &nLen2))
			{
				buff.SkeepBytes (nLen2);
				nLen += nLen2;
				continue;
			}
			
			if (isReserved (buff.getByte ()))
			{
				nLen++;
				continue;
			}
			break;			
		}
		*pnLen = nLen;
		return true;
	}

	static bool isFragment (vmsHttpBuffer& buff, size_t *pnLen)
	{
		return isAsteriskUcharOrReserved (buff, pnLen);
	}

	
	static bool isScheme (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		while (!buff.isEndOfBufferReached () || *pnLen == 0)
		{
			char ch = (char)buff.getByte ();
			if (isalpha (ch) || isdigit (ch) || ch == '+' || ch == '-' || ch == '.')
				(*pnLen)++;
			else
				break;
		}
		return *pnLen != 0;
	}

	
	static bool isNetPath (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		if (buff.getByte () != '/' || buff.getByte () != '/')
			return false;
		size_t nLen = 2;
		size_t nLen2 = 0;
		if (!isNetLoc (buff, &nLen2))
			return false;
		buff.SkeepBytes (nLen2);
		nLen += nLen2;
		if (isAbsPath (buff, &nLen2))
			nLen += nLen2;
		*pnLen = nLen;
		return true;
	}

	
	static bool isNetLoc (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		while (!buff.isEndOfBufferReached ())
		{
			size_t nLen = 0;
			if (isPchar (buff, &nLen))
			{
				(*pnLen) += nLen;
				buff.SkeepBytes (nLen);
				continue;
			}
			BYTE b = buff.getByte ();
			if (b == ';' || b == '?')
			{
				(*pnLen)++;
				continue;
			}
			break;
		}
		return true;
	}

	
	static bool isAbsPath (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		if (buff.getByte () != '/')
			return false;
		if (!isRelPath (buff, pnLen))
			return false;
		(*pnLen)++;
		return true;
	}

	
	static bool isRelPath (vmsHttpBuffer& buff, size_t *pnLen)
	{
		*pnLen = 0;
		if (buff.isEndOfBufferReached ())
			return true;
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		size_t nLen = 0;
		if (isPath (buff, &nLen))
		{
			*pnLen += nLen;
			buff.SkeepBytes (nLen);
		}
		BYTE b = buff.getByte ();
		if (b == ';' && isParams (buff, &nLen))
		{
			*pnLen += nLen + 1;
			buff.SkeepBytes (nLen);
			b = buff.getByte ();
		}
		if (b == '?' && isQuery (buff, &nLen))
			*pnLen += nLen + 1;
		return true;
	}

	
	static bool isPath (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		if (!isFsegment (buff, pnLen))
			return false;
		buff.SkeepBytes (*pnLen);
		while (!buff.isEndOfBufferReached ())
		{
			size_t nLen = 0;
			if (buff.getByte () == '/' && isSegment (buff, &nLen))
			{
				buff.SkeepBytes (nLen);
				*pnLen += nLen + 1;
			}
			else
			{
				break;
			}
		}
		return true;
	}

	
	static bool isFsegment (vmsHttpBuffer& buff, size_t *pnLen)
	{
		if (!isPchar (buff, pnLen))
			return false;
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		buff.SkeepBytes (*pnLen);
		size_t nLen = 0;
		while (!buff.isEndOfBufferReached () && isPchar (buff, &nLen))
		{
			(*pnLen) += nLen;
			buff.SkeepBytes (nLen);
		}
		return true;
	}

	
	static bool isSegment (vmsHttpBuffer& buff, size_t *pnLen)
	{
		*pnLen = 0;
		if (buff.isEndOfBufferReached ())
			return true;
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		size_t nLen = 0;
		while (!buff.isEndOfBufferReached () && isPchar (buff, &nLen))
		{
			(*pnLen) += nLen;
			buff.SkeepBytes (nLen);
		}
		return true;
	}

	
	static bool isUchar (vmsHttpBuffer& buff, size_t *pnLen)
	{
		if (isUnreserved (buff.getByte (true)))
		{
			*pnLen = 1;
			return true;
		}
		return isEscape (buff, pnLen);
	}

	
	static bool isUnreserved (BYTE b)
	{
		return isalpha ((char)b) || isdigit ((char)b) || isSafe (b) || isExtra (b) || isNational (b);
	}

	static bool isSafe (BYTE b)
	{
		return b == '$' || b == '-' || b == '_' || b == '.';
	}

	static bool isExtra (BYTE b)
	{
		return b == '!' || b == '*' || b == '\'' || b == '(' || b == ')' || b == ',';
	}

	static bool isNational (BYTE b)
	{
		return !isalpha ((char)b) && !isdigit ((char)b) && !isReserved (b) && !isExtra (b) && !isSafe (b) && !isUnsafe (b);
	}

	static bool isUnsafe (BYTE b)
	{
		return vmsHttpBufferElements::isCTL (b) || b == ' ' || b == '"' || b == '#' || b == '%' || b == '<' || b == '>';
	}

	static bool isReserved (BYTE b)
	{
		return b == ';' || b == '/' || b == '?' || b == ':' || b == '@' || b == '&' || b == '=' || b == '+';
	}

	
	static bool isEscape (vmsHttpBuffer& buff, size_t *pnLen)
	{
		*pnLen = 0;
		if (buff.getByte (true) != '%')
			return false;
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		buff.SkeepBytes (1);
		if (vmsHttpBufferElements::isHEX (buff.getByte ()) && vmsHttpBufferElements::isHEX (buff.getByte ()))
			*pnLen = 3;
		return *pnLen != 0;
	}

	
	static bool isPchar (vmsHttpBuffer& buff, size_t *pnLen)
	{
		BYTE b = buff.getByte (true);
		if (b == ':' || b == '@' || b == '&' || b == '=' || b == '+')
		{
			*pnLen = 1;
			return true;
		}
		return isUchar (buff, pnLen);
	}

	
	static bool isQuery (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);

		*pnLen = 0;

		while (!buff.isEndOfBufferReached ())
		{
			size_t nLen = 0;
			if (isUchar (buff, &nLen))
			{
				buff.SkeepBytes (nLen);
				(*pnLen) += nLen;
				continue;
			}
			if (isReserved (buff.getByte ()))
			{
				(*pnLen)++;
				continue;
			}
			break;
		}

		return true;
	}

	
	static bool isParams (vmsHttpBuffer& buff, size_t *pnLen)
	{
		if (!isParam (buff, pnLen))
			return false;
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		buff.SkeepBytes (*pnLen);
		while (!buff.isEndOfBufferReached ())
		{
			if (buff.getByte () != ';')
				break;
			size_t nLen = 0;
			if (!isParam (buff, &nLen))
				break;
			buff.SkeepBytes (nLen);
			(*pnLen) += nLen + 1;
		}
		return true;
	}

	
	static bool isParam (vmsHttpBuffer& buff, size_t *pnLen)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;
		while (!buff.isEndOfBufferReached ())
		{
			size_t nLen = 0;
			if (isPchar (buff, &nLen))
			{
				(*pnLen) += nLen;
				buff.SkeepBytes (nLen);
				continue;
			}
			if (buff.getByte () == '/')
			{
				(*pnLen)++;
				continue;
			}
			break;
		}
		return true;
	}
};

class vmsHttpRequestParser : public vmsHttpHeaderParser  
{
public:
	const std::string& getMethod () const {return m_strMethod;}
	const std::string& getURI () const {return m_strURI;}
	void setURI (LPCSTR psz) {m_strURI = psz;}

protected:
	
	void ReadHeaderFirstLine (vmsHttpBuffer& buff)
	{
		m_strMethod = m_strURI = m_strHttpVersion = "";

		vmsHttpBufferElements::ExtractToken (buff, m_strMethod);

		if (buff.getByte () != ' ')
			buff.Throw (buff.ParseError);

		vmsHttpURI::ExtractURI (buff, m_strURI);

		if (buff.getByte () != ' ')
			buff.Throw (buff.ParseError);

		vmsHttpBufferElements::ExtractHttpVersion (buff, m_strHttpVersion);

		if (buff.getByte () != '\r' || buff.getByte () != '\n')
			buff.Throw (buff.ParseError);
	}

public:
	vmsHttpRequestParser (LPCSTR pszRequest = NULL) 
	{
		if (pszRequest)
			ReadHeader (pszRequest);
	}
	
protected:
	void toStringFirstLine (std::string& str) const
	{
		str = m_strMethod;
		str += ' ';
		str += m_strURI;
		str += ' ';
		str += m_strHttpVersion;
		str += "\r\n";
	}

	std::string m_strMethod; 
	std::string m_strURI; 
	std::string m_strHttpVersion;
};

class vmsHttpResponseParser : public vmsHttpHeaderParser  
{
public:
	UINT getStatusCode () const {return m_nStatusCode;}
	const std::string& getResponseResult () const {return m_strResponseResult;}

protected:
	
	void ReadHeaderFirstLine (vmsHttpBuffer& buff)
	{
		m_strHttpVersion = m_strResponseResult = "";
		m_nStatusCode = 0;
		
		vmsHttpBufferElements::ExtractHttpVersion (buff, m_strHttpVersion);

		if (buff.getByte () != ' ')
			buff.Throw (buff.ParseError);

		ExtractStatusCode (buff);

		if (buff.getByte () != ' ')
			buff.Throw (buff.ParseError);

		ExtractReasonPhrase (buff);

		if (buff.getByte () != '\r' || buff.getByte () != '\n')
			buff.Throw (buff.ParseError);
	}

	
	void ExtractStatusCode (vmsHttpBuffer& buff)
	{
		std::string strCode;
		for (size_t i = 0; i < 3; i++)
		{
			char ch = (char)buff.getByte ();
			if (!isdigit (ch))
				buff.Throw (buff.ParseError);
			strCode += ch;
		}
		m_nStatusCode = atoi (strCode.c_str ());
	}

	
	void ExtractReasonPhrase (vmsHttpBuffer& buff)
	{
		m_strResponseResult = "";
		while (!buff.isEndOfBufferReached ())
		{
			BYTE b = buff.getByte (true);
			if (vmsHttpBufferElements::isCTL (b) && b != '\t')
				return;
			m_strResponseResult += buff.getByte ();
		}
	}
	
public:
	vmsHttpResponseParser (LPCSTR pszResponse = NULL) : m_nStatusCode (0) 
	{
		if (pszResponse)
			ReadHeader (pszResponse);
	}

protected:
	void toStringFirstLine (std::string& str) const
	{
		str = m_strHttpVersion;
		str += ' ';
		char sz [10] = "";
		str += _itoa (m_nStatusCode, sz, 10);
		str += ' ';
		str += m_strResponseResult;
		str += "\r\n";
	}

	std::string m_strHttpVersion;
	std::string m_strResponseResult; 
	UINT m_nStatusCode;
};

class vmsHttpContentRangeHeaderField
{
public:
	vmsHttpContentRangeHeaderField (const vmsHttpGenericHeaderField *pfield) : 
		m_uFirstBytePos (0), m_uLastBytePos (0), m_uEntityLength (0)
	{
		
		std::string strValue; pfield->getValueWoExtraSpaces (strValue);
		LPCSTR psz = strValue.c_str ();
		while (*psz && !isdigit (*psz))
			psz++;
		if (!*psz)
			return;
		m_uFirstBytePos = _atoi64 (psz);
		while (isdigit (*psz))
			psz++;
		if (*psz++ != '-')
			return;
		if (!isdigit (*psz))
			return;
		m_uLastBytePos = _atoi64 (psz);
		while (isdigit (*psz))
			psz++;
		if (*psz++ != '/')
			return;
		m_uEntityLength = _atoi64 (psz);		
	}
	UINT64 m_uFirstBytePos;
	UINT64 m_uLastBytePos;
	UINT64 m_uEntityLength;
};

class vmsHttpChunkedTransferEncoding
{
public:
	struct Chunk
	{
		UINT64 uSize;
		const unsigned char* pbData;
		Chunk () : uSize (0), pbData (NULL) {}
	};

	vmsHttpChunkedTransferEncoding () : m_bZeroChunkReached (false) {}

	
	
	
	
	
	void Read (vmsHttpBuffer &buff)
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff, vmsHttpBufferStateAutoSaveAndRestore::DefaultThrowReason);
		buff.setDefaultThrowReason (buff.NeedMoreData);

		m_vChunks.clear ();
		m_lastNotCompletelyProcessedChunk = Chunk ();
		m_bZeroChunkReached = false;

		while (buff.getByte (true) != '0')
		{
			Chunk item;
			try 
			{
				ReadChunk (buff, item);
			}
			catch (...) 
			{
				m_lastNotCompletelyProcessedChunk = item;
				buff.Throw (buff.getThrowReason ());
			}
			m_vChunks.push_back (item);
		}

		m_bZeroChunkReached = true;

		buff.getByte (); 
		if (buff.getByte () != '\r' || buff.getByte () != '\n')
			buff.Throw (buff.ParseError);

		while (buff.getByte (true) != '\r')
		{
			vmsHttpGenericHeaderField hf;
			hf.ReadFromBuffer (buff);
		}

		if (buff.getByte () != '\r' || buff.getByte () != '\n')
			buff.Throw (buff.ParseError);
	}

	size_t getChunkCount () const {return m_vChunks.size ();}
	const Chunk* getChunk (size_t nIndex) const {return &m_vChunks [nIndex];}

	const Chunk& getLastNotCompletelyProcessedChunk () const {return m_lastNotCompletelyProcessedChunk;}
	bool isZeroChunkReached () const {return m_bZeroChunkReached;}

protected:
	
	
	
	
	static bool ReadChunk (vmsHttpBuffer &buff, Chunk& item)
	{
		std::string str;
		while (vmsHttpBufferElements::isHEX (buff.getByte (true)))
		{
			assert (!str.empty () || buff.getByte (true) != '0');
			str += buff.getByte ();
		}
		
		if (str.empty ())
			buff.Throw (buff.ParseError);
		
		sscanf (str.c_str (), "%I64x", &item.uSize);
		if (!item.uSize)
			return false;

		size_t nLen = 0;
		if (isChunkExt (buff, &nLen))
			buff.SkeepBytes (nLen);

		if (buff.getByte () != '\r' || buff.getByte () != '\n')
			buff.Throw (buff.ParseError);

		item.pbData = buff.getPointer ();
		buff.SkeepBytes ((size_t)item.uSize);

		if (buff.getByte () != '\r' || buff.getByte () != '\n')
			buff.Throw (buff.ParseError);

		return true;
	}

	
	
	
	static bool isChunkExt (vmsHttpBuffer& buff, size_t *pnLen) 
	{
		vmsHttpBufferStateAutoSaveAndRestore bs (buff);
		buff.setDefaultThrowReason (vmsHttpBuffer::NeedMoreData);
		*pnLen = 0;

		while (!buff.isEndOfBufferReached ())
		{
			if (buff.getByte () != ';')
				break;

			size_t nLen1 = 0;
			if (!vmsHttpBufferElements::isToken (buff, &nLen1))
				break;

			buff.SkeepBytes (nLen1);

			if (buff.getByte () != '=')
				break;

			size_t nLen2 = 0;
			if (!vmsHttpBufferElements::isToken (buff, &nLen1))
			{
				if (!vmsHttpBufferElements::isQuotedString (buff, &nLen1))
					break;
			}

			buff.SkeepBytes (nLen2);

			*pnLen += 1 + nLen1 + 1 + nLen2;
		}

		return true;
	}

protected:
	std::vector <Chunk> m_vChunks;
	Chunk m_lastNotCompletelyProcessedChunk;
	bool m_bZeroChunkReached;
};