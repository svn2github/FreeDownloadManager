/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTask.h"
#include "vmsFdmBhIpcTaskResponse.h"
class vmsFdmBhIpcTaskProcessor
{
public:
	using process_callback_fn_t = std::function <void(std::shared_ptr <vmsFdmBhIpcTask>, std::shared_ptr <vmsFdmBhIpcTaskResponse>)>;

public:
	virtual void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback) = 0;
	virtual ~vmsFdmBhIpcTaskProcessor () {}

public:
	
	template <class TResponse = vmsFdmBhIpcTaskResponse>
	static std::shared_ptr <TResponse> construct_response (
		const std::shared_ptr <vmsFdmBhIpcTask> &task, 
		const std::wstring &error = L"")
	{
		auto result = std::make_shared <TResponse> ();
		initialize_response (task, *result);
		result->m_error = error;
		return result;
	}
	static void initialize_response (
		const std::shared_ptr <vmsFdmBhIpcTask> &task,
		vmsFdmBhIpcTaskResponse &resp)
	{
		resp.m_id = task->m_id;
		resp.m_type = task->m_type;
	}
};

