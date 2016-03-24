/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsMsXmlUtil
{
public:
	static inline HRESULT CreateXmlDocumentInstance (IXMLDOMDocumentPtr &spXML, bool bSetAllowAsyncLoadTo = false)
	{
		HRESULT hr = spXML.CreateInstance (__uuidof (DOMDocument));
		if (!spXML)
			return hr;
		spXML->put_async (bSetAllowAsyncLoadTo ? VARIANT_TRUE : VARIANT_FALSE);
		return S_OK;
	}

	static HRESULT LoadXmlFromStream (IXMLDOMDocument *pDoc, std::istream *pStm)
	{
		IStreamPtr spStream;
		HRESULT hr = CreateStreamOnHGlobal (NULL, TRUE, &spStream);
		if (!spStream)
			return hr;

		std::vector <char> vbTmp (16*1024);
		while (pStm->good ())
		{
			pStm->read (&vbTmp.front (), vbTmp.size ());
			size_t nRead = (size_t) pStm->gcount ();
			hr = spStream->Write (&vbTmp.front (), (ULONG)nRead, NULL);
			if (FAILED (hr))
				return hr;
		}

		LARGE_INTEGER li = {0};
		hr = spStream->Seek (li, STREAM_SEEK_SET, NULL);
		if (FAILED (hr))
			return hr;

		return LoadXmlFromStream (pDoc, spStream);
	}

	static HRESULT LoadXmlFromStream (IXMLDOMDocument *pDoc, IStream *pStm)
	{
		IPersistStreamInitPtr spPSI;
		HRESULT hr = pDoc->QueryInterface (IID_IPersistStreamInit, (void**)&spPSI);
		if (!spPSI)
			return hr;
		return spPSI->Load (pStm);
	}

	static inline HRESULT isDocumentLoadComplete (IXMLDOMDocument *pDoc, bool &bComplete)
	{
		assert (pDoc != NULL);
		long lReady = 0;
		HRESULT hr = pDoc->get_readyState (&lReady);
		if (SUCCEEDED (hr))
			return bComplete = lReady == READYSTATE_COMPLETE;
		return hr;
	}

	static inline HRESULT getDocumentLoadResult (IXMLDOMDocument *pDoc)
	{
		IXMLDOMParseErrorPtr  spError;
		HRESULT hr = pDoc->get_parseError (&spError);
		if (!spError)
			return hr;
		LONG lCode = E_FAIL;
		spError->get_errorCode (&lCode);
		return lCode;
	}
};