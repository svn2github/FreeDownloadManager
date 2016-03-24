/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFatalExceptionProvider
{
public:
	using callback_fn_t = std::function <ULONG(PEXCEPTION_POINTERS)>;
public:
	virtual void set_callback (callback_fn_t) = 0;
	virtual ~vmsFatalExceptionProvider () {};
};