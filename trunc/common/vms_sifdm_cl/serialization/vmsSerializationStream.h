/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsSerializationInputStream
{
public:
	typedef std::shared_ptr <vmsSerializationInputStream> tSP;
public:
	virtual tSP SelectNode (LPCWSTR pwszName)	 = NULL;
	virtual std::vector <tSP> SelectNodes (const std::wstring& wstrName) = NULL;
	
	virtual bool ReadValue (LPCWSTR pwszName, std::wstring& val, bool bAttribute = true) = NULL;
	
	
	template <class TValue>
	bool ReadValueS (LPCWSTR pwszName, TValue& val, bool bAttribute = true)
	{
		std::wstring wstr;
		if (!ReadValue (pwszName, wstr, bAttribute))
			return false;
		std::wstringstream wss;
		wss  << wstr;
		wss >> val;
		return !wss.fail ();
	}

	
	virtual void clear () 
	{

	}
};

class vmsSerializationOutputStream
{
public:
	typedef std::shared_ptr <vmsSerializationOutputStream> tSP;
public:
	virtual tSP CreateNode (LPCWSTR pwszName) = NULL;
	
	virtual bool WriteValue (LPCWSTR pwszName, const std::wstring& val, bool bAttribute = true) = NULL;
	
	
	template <class TValue>
	bool WriteValueS (LPCWSTR pwszName, const TValue& val, bool bAttribute = true)
	{
		std::wstringstream wss;
		wss << val;
		if (wss.fail ())
			return false;
		return WriteValue (pwszName, wss.str (), bAttribute);
	}

	
	virtual void clear () 
	{

	}
};

class vmsSerializationIoStream
{
public:
	typedef std::shared_ptr <vmsSerializationIoStream> tSP;
public:
	vmsSerializationIoStream (vmsSerializationInputStream* pStm) : 
	  m_pIS (pStm), m_pOS (nullptr)
	{
		assert (m_pIS);
	}

	vmsSerializationIoStream (std::shared_ptr <vmsSerializationInputStream> spStm) : 
		m_spIS (spStm), m_pIS (spStm.get ()), m_pOS (nullptr)
	{
		assert (m_pIS);
	}

	vmsSerializationIoStream (vmsSerializationOutputStream* pStm) :
		m_pOS (pStm), m_pIS (nullptr)
	{
		assert (m_pOS);
	}

	vmsSerializationIoStream (std::shared_ptr <vmsSerializationOutputStream> spStm) :
		m_spOS (spStm), m_pOS (spStm.get ()), m_pIS (nullptr)
	{
		assert (m_pOS);
	}

public:
	tSP SelectOrCreateNode (LPCWSTR pwszName)
	{
		if (m_pIS)
		{
			auto node = m_pIS->SelectNode (pwszName);
			if (!node)
				return nullptr;
			return std::make_shared <vmsSerializationIoStream> (node);
		}
		else
		{
			auto node = m_pOS->CreateNode (pwszName);
			if (!node)
				return nullptr;
			return std::make_shared <vmsSerializationIoStream> (node);
		}
	}

	std::vector <tSP> SelectNodes (const std::wstring& wstrName)
	{
		assert (m_pIS);
		if (!m_pIS)
			return std::vector <tSP> ();
		auto nodes = m_pIS->SelectNodes (wstrName);
		std::vector <tSP> vResult;
		for (auto it = nodes.begin (); it != nodes.end (); ++it)
			vResult.push_back (std::make_shared <vmsSerializationIoStream> (*it));
		return vResult;
	}

	
	bool SerializeValue (LPCWSTR pwszName, std::wstring& val, bool bAttribute = true)
	{
		if (m_pIS)
			return m_pIS->ReadValue (pwszName, val, bAttribute);
		else
			return m_pOS->WriteValue (pwszName, val, bAttribute);
	}

	
	template <class TValue>
	bool SerializeValueS (LPCWSTR pwszName, TValue& val, bool bFailOnReadFail = true, bool bAttribute = true)
	{
		if (m_pIS)
			return m_pIS->ReadValueS (pwszName, val, bAttribute) ? true : !bFailOnReadFail;
		else 
			return m_pOS->WriteValueS (pwszName, val, bAttribute);
	}

	
	void clear () 
	{
		if (m_pIS)
			m_pIS->clear ();
		else
			m_pOS->clear ();
	}

	bool isInputStream () const 
	{
		return m_pIS != nullptr;
	}

	bool isOutputStream () const 
	{
		return m_pOS != nullptr;
	}

	vmsSerializationInputStream* getInputStream ()
	{
		return m_pIS;
	}

	vmsSerializationOutputStream* getOutputStream ()
	{
		return m_pOS;
	}

protected:
	vmsSerializationInputStream* m_pIS;
	std::shared_ptr <vmsSerializationInputStream> m_spIS;
	vmsSerializationOutputStream* m_pOS;
	std::shared_ptr <vmsSerializationOutputStream> m_spOS;
};