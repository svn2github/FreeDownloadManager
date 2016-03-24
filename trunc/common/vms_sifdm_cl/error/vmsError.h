/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <system_error>
#include "../stringh.h"
#include "common_error.h"
#include "vmsErrorSource.h"
#include "vmsErrorDetails.h"
#include "inter_module_err_helper.h"

class vmsError
{
public:
	std::error_code error;
	std::shared_ptr <vmsErrorSource> source;
	std::shared_ptr <vmsErrorDetails> details;

	vmsError () {}
	vmsError (const std::error_code& ec) : error (ec) {}

	void clear ()
	{
		error.clear ();
		source = nullptr;
		details = nullptr;
	}

	operator bool () const
	{
		return (bool)error;
	}

	vmsError& operator = (const std::error_code& ec)
	{
		error = ec;
		source = nullptr;
		details = nullptr;
		return *this;
	}

	void normalize (const inter_module_err_helper& normalizer = 
		inter_module_err_helper::instance ())
	{
		error = normalizer.normalize_error_code (error);
	}

	bool normalized (const inter_module_err_helper& normalizer =
		inter_module_err_helper::instance ()) const
	{
		return normalizer.normalized (error);
	}
};

class vmsErrorSerializer : public vmsSerializable
{
public:
	vmsErrorSerializer (const vmsError& err) : 
		m_err (err) {}

protected:
	const vmsError& m_err;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		assert (pStm && pStm->isOutputStream ());
		if (!pStm || !pStm->isOutputStream ())
			return false;
		return Serialize (pStm->getOutputStream (), flags);
	}

	bool Serialize (vmsSerializationOutputStream *pStm, unsigned flags = 0)
	{
		assert (pStm);
		if (!pStm->WriteValueS (L"code", m_err.error.value ()) || 
			!pStm->WriteValue (L"category", wideFromUtf8 (m_err.error.category ().name ())))
			return false;
		if (m_err.source && !vmsSerializable::SerializeObject (pStm, m_err.source.get (), L"source"))
			return false;
		if (m_err.details && !vmsSerializable::SerializeObject (pStm, m_err.details.get (), L"details"))
			return false;
		return true;
	}
};
