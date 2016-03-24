/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsXmlUtil.h"
#include "vmsMsXmlUtil.h"
#include "vmsXmlSerializationOutputStream.h"

class vmsMsXmlSerializationInputStream : public vmsSerializationInputStream,
	public vmsSerializationInputStreamBinding
{
public:
	typedef std::shared_ptr <vmsMsXmlSerializationInputStream> tSP;
public:
	virtual vmsSerializationInputStream::tSP SelectNode (LPCWSTR pwszName)
	{
		assert (m_spNode != NULL);
		IXMLDOMNodePtr spNode;
		m_spNode->selectSingleNode ((BSTR)pwszName, &spNode);
		if (!spNode)
			return NULL;

		tSP spResult = std::make_shared <vmsMsXmlSerializationInputStream> ();
		spResult->m_spNode = spNode;
		return spResult;
	}

	virtual std::vector <vmsSerializationInputStream::tSP> SelectNodes (const std::wstring& wstrName)
	{
		assert (m_spNode != NULL);
		std::vector <vmsSerializationInputStream::tSP> vResult;
		IXMLDOMNodeListPtr spList;
		m_spNode->selectNodes ((BSTR)wstrName.c_str (), &spList);
		if (!spList)
			return vResult;
		IXMLDOMNodePtr spNode;
		while (SUCCEEDED (spList->nextNode (&spNode)) && spNode != nullptr)
		{
			vResult.push_back (std::make_shared <vmsMsXmlSerializationInputStream> (spNode));
			spNode = NULL;
		}
		return vResult;
	}

	virtual bool ReadValue (LPCWSTR pwszName, std::wstring& val, bool bAttribute = false)
	{
		return getValueText (pwszName, val, bAttribute);
	}

	vmsMsXmlSerializationInputStream ()
	{

	}

	vmsMsXmlSerializationInputStream (IXMLDOMNode *pRootNode) : m_spNode (pRootNode)
	{

	}

	virtual bool BindToStream (std::shared_ptr <std::istream> spStream)
	{
		if (spStream)
		{
			IXMLDOMDocumentPtr spDoc;
			vmsMsXmlUtil::CreateXmlDocumentInstance (spDoc);
			if (!spDoc)
				return false;
			if (FAILED (vmsMsXmlUtil::LoadXmlFromStream (spDoc, spStream.get ())))
				return false;
			IXMLDOMElementPtr spEl;
			spDoc->get_documentElement (&spEl);
			if (!spEl)
				return false;
			m_spNode = spEl;
			assert (m_spNode);
			if (!m_spNode)
				return false;
		}
		else
		{
			m_spNode = nullptr;
		}

		return vmsSerializationInputStreamBinding::BindToStream (spStream);		
	}

protected:
	IXMLDOMNodePtr m_spNode;

	bool getValueText (LPCWSTR pwszName, std::wstring &val, bool bAttribute)
	{
		assert (m_spNode != NULL);
		if (!m_spNode)
			return false;

		if (!bAttribute)
		{
			IXMLDOMNodePtr spItem;
			m_spNode->selectSingleNode ((BSTR)pwszName, &spItem);
			if (!spItem)
				return false;
			CComBSTR bstr;
			spItem->get_text (&bstr);
			val = bstr;
			return true;
		}

		IXMLDOMNamedNodeMapPtr spAttrs;
		m_spNode->get_attributes (&spAttrs);
		if (!spAttrs)
			return false;

		IXMLDOMNodePtr spItem;
		spAttrs->getNamedItem ((BSTR)pwszName, &spItem);
		if (!spItem)
			return false;

		CComVariant vt;
		spItem->get_nodeValue (&vt);
		assert (vt.vt = VT_BSTR);
		if (vt.vt != VT_BSTR)
			return false;

		val = vt.bstrVal;
		return true;
	}
};
