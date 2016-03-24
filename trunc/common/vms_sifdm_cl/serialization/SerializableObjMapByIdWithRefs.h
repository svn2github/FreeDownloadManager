/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "SerializableObjMapById.h"
template <typename TObj>
struct SerializableObjMapByIdWithRefsItem :
	public vmsSerializable
{
	uint32_t refs = 0;
	std::shared_ptr <TObj> obj;
	virtual bool Serialize (vmsSerializationIoStream *pStm,
		unsigned flags ) override
	{
		assert (obj);
		return pStm->SerializeValueS (L"item-refs", refs) &&
			obj->Serialize (pStm, flags);
	}
};
template <typename TObj, typename TKey = uint32_t>
class SerializableObjMapByIdWithRefs :
	public SerializableObjMapById <SerializableObjMapByIdWithRefsItem <TObj>, TKey>
{
protected:
	using base_t = SerializableObjMapById <SerializableObjMapByIdWithRefsItem <TObj>, TKey> ;
	using item_t = SerializableObjMapByIdWithRefsItem < TObj > ;

public:
	virtual TKey add_item (const std::shared_ptr <TObj> &obj)
	{
		auto item = std::make_shared <item_t> ();
		item->refs = 1;
		item->obj = obj;
		return base_t::add_item (item);
	}

	virtual void remove_item (TKey id)
	{
		{
			vmsLockableScope;
			auto it = m_items.find (id);
			if (it != m_items.end () && it->second->refs > 1)
			{
				--it->second->refs;
				vmsLockableScopeUnlock;
				setDirty ();
				return;
			}
		}

		base_t::remove_item (id);
	}

protected:
	virtual std::shared_ptr <TObj> create_object (vmsSerializationIoStream *pStm) = 0;

	virtual std::shared_ptr <item_t> create_obj (vmsSerializationIoStream *pStm) override
	{
		auto object = create_object (pStm);
		if (!object)
			return nullptr;
		auto obj = std::make_shared <item_t> ();
		obj->refs = 1;
		obj->obj = object;
		return obj;
	}
};