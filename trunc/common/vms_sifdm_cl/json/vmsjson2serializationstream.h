/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsJson2SerializationInputStream : 
	public vmsSerializationInputStream,
	public vmsSerializationInputStreamBinding
{
public:
	typedef std::shared_ptr <vmsJson2SerializationInputStream> tSP;
public:
	virtual vmsSerializationInputStream::tSP SelectNode (LPCWSTR pwszName)
	{
		assert (m_node.is<picojson::object> ());
		auto&& obj = m_node.get <picojson::object> ();

		std::string strName (utf8FromWide (pwszName));

		auto it = obj.find (strName);
		if (it == obj.end ())
			return nullptr;

		tSP spResult = std::make_shared <vmsJson2SerializationInputStream> ();
		spResult->m_node = it->second;

		return spResult;
	}

	virtual std::vector <vmsSerializationInputStream::tSP> SelectNodes (const std::wstring& wstrName)
	{
		std::vector <vmsSerializationInputStream::tSP> vResult;
		
		assert (m_node.is<picojson::object> () || m_node.is<picojson::array> ());

		picojson::array *arr = nullptr;

		if (m_node.is<picojson::object> ())
		{
			auto&& obj = m_node.get <picojson::object> ();

			std::string strName (utf8FromWide (wstrName));

			auto it = obj.find (strName);
			if (it == obj.end ())
				return vResult;

			if (it->second.is<picojson::array> ())
			{
				arr = &it->second.get <picojson::array> ();
			}
			else
			{
				vResult.push_back (
					std::make_shared <vmsJson2SerializationInputStream> (it->second));
				return vResult;
			}

		}
		else if (m_node.is<picojson::array> ())
		{
			arr = &m_node.get <picojson::array> ();
		}

		if (arr)
		{
			for (const auto &val : *arr)
			{
				tSP spResult = std::make_shared <vmsJson2SerializationInputStream> (val);
				vResult.push_back (spResult);
			}
		}

		return vResult;
	}

	virtual bool ReadValue (LPCWSTR pwszName, std::wstring& val, bool bAttribute = false)
	{
		return getValueText (pwszName, val, bAttribute);
	}

	vmsJson2SerializationInputStream ()
	{

	}

	vmsJson2SerializationInputStream (picojson::value node) : m_node (node)
	{

	}

	virtual bool BindToStream (std::shared_ptr <std::istream> spStream)
	{
		m_node = picojson::value ();
		if (!vmsSerializationInputStreamBinding::BindToStream (spStream))
			return false;
		if (!m_spBoundStream)
			return true;
		return picojson::parse (m_node, *m_spBoundStream.get ()).empty ();
	}

protected:
	picojson::value m_node;

	bool getValueText (LPCWSTR pwszName, std::wstring &val, bool bAttribute)
	{
		assert (m_node.is<picojson::object> ());
		if (!m_node.is<picojson::object> ())
			return false;
		auto&& obj = m_node.get <picojson::object> ();

		std::string strName (utf8FromWide (pwszName));
		auto it = obj.find (strName);
		if (it == obj.end ())
			return false;

		assert (it->second.is<std::string> ());
		if (!it->second.is<std::string> ())
			return false;

		val = wideFromUtf8 (it->second.get<std::string>());
		return true;
	}
};

class vmsJson2SerializationOutputStream : 
	public vmsSerializationOutputStream,
	public vmsSerializationOutputStreamBinding
{
public:
	typedef std::shared_ptr <vmsJson2SerializationOutputStream> tSP;

public:
	vmsJson2SerializationOutputStream ()
	{
		clear ();
	}

	~vmsJson2SerializationOutputStream ()
	{
		if (!m_bFlushedToBoundStream)
			Flush ();
	}

	virtual vmsSerializationOutputStream::tSP CreateNode (LPCWSTR pwszName)
	{
		assert (m_node.is<picojson::object> ());
		if (!m_node.is<picojson::object> ())
			return nullptr;
		auto&& obj = m_node.get <picojson::object> ();

		auto it = obj.find (utf8FromWide (pwszName));
		assert (it == obj.end ());
		if (it != obj.end ())
			return nullptr;

		tSP spNode = std::make_shared <vmsJson2SerializationOutputStream> ();
		m_mChildNodes.insert (std::make_pair (std::wstring (pwszName), spNode));

		return spNode;
	}

	virtual bool WriteValue (LPCWSTR pwszName, const std::wstring& val, bool bAttribute = false)
	{
		return WriteValueText (pwszName, val, bAttribute);
	}

	virtual void clear () 
	{
		m_node = picojson::value (picojson::value::object ());
		m_mChildNodes.clear ();
	}

	virtual bool Flush ()
	{
		if (!m_spBoundStream)
			return false;
		if (m_bFlushedToBoundStream)
			return false;
		toStream (*m_spBoundStream);
		return m_bFlushedToBoundStream = true;
	}

protected:
	picojson::value m_node;
	std::multimap <std::wstring, tSP> m_mChildNodes;

	bool WriteValueText (LPCWSTR pwszName, const std::wstring &wstrText, bool bAttribute)
	{
		assert (m_node.is<picojson::object> ());
		if (!m_node.is<picojson::object> ())
			return nullptr;
		auto&& obj = m_node.get <picojson::object> ();
		obj [utf8FromWide (pwszName)] = picojson::value (utf8FromWide (wstrText));
		return true;
	}

	void toStream (std::ostream &os)
	{
		os << getResultingJsonValue ();
	}

	picojson::value getResultingJsonValue () const
	{
		picojson::value retVal = m_node;
		auto&& obj = retVal.get<picojson::object>();
		bool has_different_elemtns = !obj.empty ();
		for (auto it = m_mChildNodes.begin (); it != m_mChildNodes.end (); ++it)
		{
			picojson::value::array arr;
			auto itNext (it); ++itNext;
			for (auto it2 = itNext; it2 != m_mChildNodes.end () && it->first == it2->first; ++it2, ++it)
				arr.push_back (it->second->getResultingJsonValue ());
			itNext = it; ++itNext;
			if (itNext != m_mChildNodes.end ())
				has_different_elemtns = true;
			
			if (!arr.empty ())
			{
				arr.push_back (it->second->getResultingJsonValue ());
				if (!has_different_elemtns)
					return picojson::value (arr);
				else
					obj [utf8FromWide (it->first)] = picojson::value (arr);
			}
			else
			{
				obj [utf8FromWide (it->first)] = it->second->getResultingJsonValue ();
			}
		}
		return retVal;
	}
};