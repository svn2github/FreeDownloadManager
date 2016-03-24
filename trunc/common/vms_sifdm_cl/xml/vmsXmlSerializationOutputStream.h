/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsXmlUtil.h"
class vmsXmlSerializationOutputStream : public vmsSerializationOutputStream,
	public vmsSerializationOutputStreamBinding
{
public:
	typedef std::shared_ptr <vmsXmlSerializationOutputStream> tSP;

	enum Flags
	{
		DoNotWriteXmlTag = 1,
		XmlSpacePreserve = 1 << 1,
	};

public:
	vmsXmlSerializationOutputStream (const std::wstring& wstrRootNodeName, unsigned uFlags = 0) :
		m_wstrNodeName (wstrRootNodeName),
		m_uFlags (uFlags)
	{

	}

	~vmsXmlSerializationOutputStream ()
	{
		if (!m_bFlushedToBoundStream)
			Flush ();
	}

	virtual vmsSerializationOutputStream::tSP CreateNode (LPCWSTR pwszName)
	{
		tSP spNode = std::make_shared <vmsXmlSerializationOutputStream> (pwszName);
		m_mChildNodes.insert (std::make_pair (std::wstring (pwszName), spNode));
		return spNode;
	}

	virtual bool WriteValue (LPCWSTR pwszName, const std::wstring& val, bool bAttribute = false)
	{
		return WriteValueText (pwszName, val, bAttribute);
	}

	virtual void clear ()
	{
		m_mAttributes.clear ();
		m_mValues.clear ();
		m_mChildNodes.clear ();
	}

	virtual bool Flush ()
	{
		if (!m_spBoundStream)
			return false;
		if (m_bFlushedToBoundStream)
			return false;
		if (!(m_uFlags & DoNotWriteXmlTag))
			(*m_spBoundStream) << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		toStream (*m_spBoundStream);
		return m_bFlushedToBoundStream = true;
	}

protected:
	unsigned m_uFlags;
	std::wstring m_wstrNodeName;
	std::map <std::wstring, std::wstring> m_mAttributes;
	std::map <std::wstring, std::wstring> m_mValues;
	std::multimap <std::wstring, tSP> m_mChildNodes;

	bool WriteValueText (LPCWSTR pwszName, const std::wstring &wstrText, bool bAttribute)
	{
		if (bAttribute)
			m_mAttributes [pwszName] = wstrText;
		else
			m_mValues [pwszName] = wstrText;
		return true;
	}

	void toStream (std::ostream &os)
	{
		os << "<" << vmsXmlUtil::toUtf8 (m_wstrNodeName);

		if (m_uFlags & XmlSpacePreserve)
			os << " xml:space=\"preserve\"";

		for (auto it = m_mAttributes.cbegin (); it != m_mAttributes.cend (); ++it)
			os << " " << vmsXmlUtil::toUtf8 (it->first) << "=\"" << vmsXmlUtil::toUtf8 (it->second) << "\"";

		if (m_mValues.empty () && m_mChildNodes.empty ())
		{
			os << " />";
			return;
		}

		os << ">";

		for (auto it = m_mValues.cbegin (); it != m_mValues.cend (); ++it)
			os << "<" << vmsXmlUtil::toUtf8 (it->first) << ">" << vmsXmlUtil::toUtf8 (it->second) << "</" << vmsXmlUtil::toUtf8 (it->first) << ">";

		for (auto it = m_mChildNodes.cbegin (); it != m_mChildNodes.cend (); ++it)
			it->second->toStream (os);

		os << "</" << vmsXmlUtil::toUtf8 (m_wstrNodeName) << ">";
	}
};