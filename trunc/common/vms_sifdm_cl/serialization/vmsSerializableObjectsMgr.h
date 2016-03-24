/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsSerializer.h"
#include "../vmsThreadSafe.h"

class vmsSerializableObjectsMgr : public vmsThreadSafe,
	public vmsCreatesThreads
{
public:
	typedef std::shared_ptr <vmsSerializableObjectsMgr> tSP;

	struct ObjectSerializationData
	{
		 std::shared_ptr <vmsSerializationStoreMgr> spSerializationStore;
		 vmsSerializationInputStream::tSP spSerializationInputFormatter;
		 vmsSerializationOutputStream::tSP spSerializationOutputFormatter;
		 enum Flags {DisableAutosave = 1};
		 unsigned flags;
		 ObjectSerializationData () : flags (0) {}
	};
public:
	vmsSerializableObjectsMgr () : m_uAutosaveIntervalMs (3*60*1000)
	{

	}

	virtual ~vmsSerializableObjectsMgr ()
	{
		Shutdown (true);
	}

	void addObject (vmsSerializable::tSP spObject, ObjectSerializationData &osd, bool bLoad = false)
	{
		vmsTHREAD_SAFE_SCOPE;
		m_mObjects [spObject] = osd;
		if (bLoad)
			LoadObject (spObject);
	}

	bool LoadObject (vmsSerializable::tSP spObject)
	{
		vmsTHREAD_SAFE_SCOPE;
		auto it = m_mObjects.find (spObject);
		assert (it != m_mObjects.end ());
		if (it == m_mObjects.end ())
			return false;
		it->second.spSerializationInputFormatter->clear ();
		return vmsSerializer::Serialize (spObject.get (), it->second.spSerializationStore.get (), it->second.spSerializationInputFormatter.get ());
	}

	bool SaveObject (vmsSerializable::tSP spObject, bool bCheckIfDirty = false)
	{
		if (bCheckIfDirty && !spObject->isDirty ())
			return true;

		vmsTHREAD_SAFE_SCOPE;

		auto it = m_mObjects.find (spObject);
		assert (it != m_mObjects.end ());
		if (it == m_mObjects.end ())
			return false;

		it->second.spSerializationOutputFormatter->clear ();

		return vmsSerializer::Serialize (spObject.get (), it->second.spSerializationStore.get (), it->second.spSerializationOutputFormatter.get ());
	}

	
	bool SaveAllObjects (bool bCheckIfDirty = false, unsigned skipObjectsWithFlags = 0)
	{
		vmsTHREAD_SAFE_SCOPE;

		bool bErrorOccured = false;

		std::for_each (m_mObjects.begin (), m_mObjects.end (), [&](std::pair <vmsSerializable::tSP, ObjectSerializationData> item)
		{
			if (item.second.flags & skipObjectsWithFlags)
				return;
			if (!SaveObject (item.first, bCheckIfDirty))
				bErrorOccured = true;
		});

		return !bErrorOccured;
	}

	bool LoadAllObjects ()
	{
		vmsTHREAD_SAFE_SCOPE;

		bool bErrorOccured = false;

		std::for_each (m_mObjects.begin (), m_mObjects.end (), [&](std::pair <vmsSerializable::tSP, ObjectSerializationData> item)
		{
			if (!LoadObject (item.first))
				bErrorOccured = true;
		});

		return !bErrorOccured;
	}

	void StartAutoSaveThread ()
	{
		if (m_spThreadAutosave)
			return;
		m_spThreadAutosave = CreateThread (_threadAutosave);
	}

	void setAutoSaveInterval (unsigned uIntervalMs)
	{
		m_uAutosaveIntervalMs = uIntervalMs;
	}

protected:
	std::map <vmsSerializable::tSP, ObjectSerializationData> m_mObjects;
	vmsWinHandle::tSP m_spThreadAutosave;
	unsigned m_uAutosaveIntervalMs; 

	static unsigned __stdcall _threadAutosave(vmsCreatesThreads *pthis2)
	{
		vmsSerializableObjectsMgr *pthis = dynamic_cast <vmsSerializableObjectsMgr*> (pthis2);
		assert (pthis != NULL);
		while (!pthis->WaitForShutdownEvent (pthis->m_uAutosaveIntervalMs))
			pthis->SaveAllObjects (true, ObjectSerializationData::DisableAutosave);

		
		pthis->SaveAllObjects(true, ObjectSerializationData::DisableAutosave);
		return 0;
	}
};