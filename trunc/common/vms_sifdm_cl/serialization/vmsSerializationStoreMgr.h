/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsSerializationStoreMgr
{
public:
	vmsSerializationStoreMgr ()
	{

	}

	virtual ~vmsSerializationStoreMgr ()
	{

	}

	

	virtual std::shared_ptr <std::ostream> getSecondaryStreamReadyToSaveObject () = NULL;

	
	
	virtual bool MoveSecondaryStreamToPrimary (std::shared_ptr <std::ostream> spStm)
	{
		
		
		return true;
	}

	

	virtual std::shared_ptr <std::istream> getPrimaryStreamReadyToLoadObject () = NULL;

	
	virtual std::shared_ptr <std::istream> getSecondaryStreamReadyToLoadObject ()
	{
		
		return NULL;
	}
};

class vmsSerializationFileStoreMgr : public vmsSerializationStoreMgr
{
public:

	vmsSerializationFileStoreMgr (const std::wstring& wstrSerializationFile = L"") : m_wstrSerializationFile (wstrSerializationFile)
	{

	}

	virtual ~vmsSerializationFileStoreMgr ()
	{

	}

	void setSerializationFile (const std::wstring& wstrFile)
	{
		m_wstrSerializationFile = wstrFile;
	}

	virtual std::shared_ptr <std::ostream> getSecondaryStreamReadyToSaveObject ()
	{
		if (m_wstrSerializationFile.empty ())
			return NULL;
		return std::make_shared <std::ofstream> (getSecondarySerializationFile ().c_str (), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	}

	virtual bool MoveSecondaryStreamToPrimary (std::shared_ptr <std::ostream> spStm)
	{
		std::shared_ptr <std::ofstream> spFstm = std::dynamic_pointer_cast <std::ofstream> (spStm);
		assert (spFstm != NULL);
		if (spFstm->is_open ())
			spFstm->close ();
		_wremove (m_wstrSerializationFile.c_str ());
		return !_wrename (getSecondarySerializationFile ().c_str (), m_wstrSerializationFile.c_str ());
	}
	
	virtual std::shared_ptr <std::istream> getPrimaryStreamReadyToLoadObject ()
	{
		auto spStm = std::make_shared <std::ifstream> (m_wstrSerializationFile, std::ios_base::in | std::ios_base::binary);
		if (!spStm->good ())
			return NULL;
		return spStm;
	}

	virtual std::shared_ptr <std::istream> getSecondaryStreamReadyToLoadObject ()
	{
		auto spStm = std::make_shared <std::ifstream> (getSecondarySerializationFile ().c_str (), std::ios_base::in | std::ios_base::binary);
		if (!spStm->good ())
			return NULL;
		return spStm;
	}

protected:
	std::wstring m_wstrSerializationFile;

	std::wstring getSecondarySerializationFile () const 
	{
		return m_wstrSerializationFile + L".tmp";
	}
};