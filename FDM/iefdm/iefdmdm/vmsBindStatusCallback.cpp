/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsBindStatusCallback.h"        

vmsBindStatusCallback::vmsBindStatusCallback()
{
	m_cRef = 0;
	m_grfBINDF = 0;
	m_pBindInfo = NULL;
	m_dwBindVerb = 0;
}

vmsBindStatusCallback::~vmsBindStatusCallback()
{

}
