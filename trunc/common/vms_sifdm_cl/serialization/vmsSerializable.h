/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsSerializationStream.h"
#include "../threadsafe/vmsLockable.h"
#include "../vmsThreadSafe.h"
#include "../stream/enums.h"

class vmsSerializable : public vmsLockable
{
public:
	typedef std::shared_ptr <vmsSerializable> tSP;

	vmsSerializable () : m_bDirty (false)
	{

	}

	virtual ~vmsSerializable ()
	{

	}

	
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0)
	{
		assert (!"implemented");
		return false;
	}

	virtual bool isDirty () const 
	{
		vmsLockableScope;
		return m_bDirty;
	}

	virtual void setDirty (bool bSet = true) 
	{
		vmsLockableScope;
		m_bDirty = bSet;
	}

	
	
	virtual bool Serialize (vmsSerializationOutputStream *pStm) 
	{
		vmsSerializationIoStream stm (pStm);
		return Serialize (&stm);
	}
	virtual bool Serialize (vmsSerializationInputStream *pStm) 
	{
		vmsSerializationIoStream stm (pStm);
		return Serialize (&stm);
	}
	

protected:
	bool m_bDirty; 

public:
	
	

	template <class TObject>
	static bool SerializeObject (vmsSerializationOutputStream *pStm, TObject *pObject, const std::wstring& wstrNodeName)
	{
		if (!pObject)
			return true;
		vmsSerializable *pS = dynamic_cast <vmsSerializable*> (pObject);
		if (!pS)
			return true;
		vmsSerializationOutputStream::tSP spStm = pStm->CreateNode (wstrNodeName.c_str ());
		assert (spStm != NULL);
		if (!spStm)
			return false;
		return pS->Serialize (spStm.get ());
	}

	template <class TObject>
	static bool SerializeObject (vmsSerializationInputStream *pStm, TObject *pObject, const std::wstring& wstrNodeName, bool bOptional = false)
	{
		if (!pObject)
			return true;
		vmsSerializable *pS = dynamic_cast <vmsSerializable*> (pObject);
		if (!pS)
			return true;
		vmsSerializationInputStream::tSP spStm = pStm->SelectNode (wstrNodeName.c_str ());
		assert (bOptional || spStm != NULL);
		if (!spStm)
			return bOptional ? true : false;
		return pS->Serialize (spStm.get ());
	}

	template <class TObject>
	static bool SerializeObject (vmsSerializationIoStream *pStm, TObject *pObject, const std::wstring& wstrNodeName, bool bOptional = false, unsigned flags = 0)
	{
		if (!pObject)
			return true;
		vmsSerializable *pS = dynamic_cast <vmsSerializable*> (pObject);
		if (!pS)
			return true;
		vmsSerializationIoStream::tSP spStm = pStm->SelectOrCreateNode (wstrNodeName.c_str ());
		assert (bOptional || spStm != NULL);
		if (!spStm)
			return bOptional ? true : false;
		return pS->Serialize (spStm.get (), flags);
	}

	template <class TObject>
	static bool isDirtyObject (TObject *pObject)
	{
		if (!pObject)
			return false;
		vmsSerializable *pS = dynamic_cast <vmsSerializable*> (pObject);
		if (!pS)
			return false;
		return pS->isDirty ();
	}

	template <class TObject>
	static void setDirtyObject (TObject *pObject, bool bSet = true)
	{
		if (!pObject)
			return;
		vmsSerializable *pS = dynamic_cast <vmsSerializable*> (pObject);
		if (!pS)
			return;
		pS->setDirty (bSet);
	}

	
	template <class TEnum>
	static bool SerializeEnum (vmsSerializationIoStream *pStm, const std::wstring& name, TEnum &enVal, bool bOptional = false)
	{
		int i = (int)enVal;
		if (!pStm->SerializeValueS (name.c_str (), i, !bOptional))
			return false;
		if (pStm->isInputStream ())
			enVal = (TEnum)i;
		return true;
	}

	template <class TObject>
	static bool SerializeObjectVector (vmsSerializationIoStream *pStm, const std::wstring& objName, std::vector <std::shared_ptr <TObject>>& vObjects, unsigned flags = 0)
	{
		if (pStm->isInputStream ())
		{
			vObjects.clear ();
			bool bFailed = false;
			auto items = pStm->SelectNodes (objName);
			for (auto it = items.begin (); it != items.end (); ++it)
			{
				auto obj = std::make_shared <TObject> ();
				if (!obj->Serialize ((*it).get (), flags))
				{
					bFailed = true;
					continue;
				}
				vObjects.push_back (obj);
			}
			return !bFailed;
		}
		else
		{
			for (auto it = vObjects.begin (); it != vObjects.end (); ++it)
			{
				if (!SerializeObject (pStm, (*it).get (), objName))
					return false;
			}
			return true;
		}
	}

protected:
	
	bool SerializeValue (vmsSerializationIoStream *pStm, LPCWSTR pwszName, std::wstring& val, bool bMergeMode, bool bAttribute = true)
	{
		if (bMergeMode && pStm->isInputStream ())
		{
			std::wstring valNew;
			if (pStm->SerializeValue (pwszName, valNew, bAttribute) && 
				valNew != val)
			{
				val = valNew;
				setDirty ();
			}
			return true;
		}
		
		return pStm->SerializeValue (pwszName, val, bAttribute);
	}

	
	template <class TValue>
	bool SerializeValueS (LPCWSTR pwszName, TValue& val, bool bMergeMode, bool bFailOnReadFail = true, bool bAttribute = true)
	{
		if (bMergeMode && pStm->isInputStream ())
		{
			TValue valNew;
			if (pStm->SerializeValueS (pwszName, valNew, true, bAttribute) && 
				valNew != val)
			{
				val = valNew;
				setDirty ();
			}
			return true;
		}

		return pStm->SerializeValueS (pwszName, val, bFailOnReadFail, bAttribute);
	}
};

class vmsSerializableTs : public vmsSerializable,
	public vmsThreadSafe
{
public:
	typedef std::shared_ptr <vmsSerializableTs> tSP;

public:
	virtual void Lock () const
	{
		EnterCriticalSection (m_csObject);
	}

	virtual void Unlock () const
	{
		LeaveCriticalSection (m_csObject);
	}
};

#include <map>

template <class TSerializable>
class vmsSerializableWithChildren : public TSerializable
{
public: 
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		for (auto it = m_mSerializableChildren.begin (); it != m_mSerializableChildren.end (); ++it)
		{
			if (!SerializeObject (pStm, it->second, it->first, true, flags))
				return false;
		}
		return true;
	}

	virtual bool isDirty () const  override
	{
		if (vmsSerializable::isDirty ())
			return true;
		for (auto it = m_mSerializableChildren.begin (); it != m_mSerializableChildren.end (); ++it)
		{
			if (it->second->isDirty ())
				return true;
		}
		return false;
	}

	virtual void setDirty (bool bSet = true) override
	{
		vmsSerializable::setDirty (bSet);
		if (!bSet)
		{
			for (auto it = m_mSerializableChildren.begin (); it != m_mSerializableChildren.end (); ++it)
				it->second->setDirty (false);
		}
	}

protected:
	std::map <std::wstring, vmsSerializable*> m_mSerializableChildren;
};