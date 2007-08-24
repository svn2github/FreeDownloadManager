/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
      

#include <numeric>

#include "libtorrent/stat.hpp"
#include "libtorrent/invariant_check.hpp"
#include <algorithm>

#if defined _MSC_VER && _MSC_VER <= 1200
#define for if (false) {} else for
#endif

using namespace libtorrent;

void libtorrent::stat::second_tick(float tick_interval)
{
	INVARIANT_CHECK;

	for (int i = history - 2; i >= 0; --i)
	{
		m_download_rate_history[i + 1] = m_download_rate_history[i];
		m_upload_rate_history[i + 1] = m_upload_rate_history[i];
		m_download_payload_rate_history[i + 1] = m_download_payload_rate_history[i];
		m_upload_payload_rate_history[i + 1] = m_upload_payload_rate_history[i];
	}

	m_download_rate_history[0] = (m_downloaded_payload + m_downloaded_protocol)
		/ tick_interval;
	m_upload_rate_history[0] = (m_uploaded_payload + m_uploaded_protocol)
		/ tick_interval;
	m_download_payload_rate_history[0] = m_downloaded_payload / tick_interval;
	m_upload_payload_rate_history[0] = m_uploaded_payload / tick_interval;

	m_downloaded_payload = 0;
	m_uploaded_payload = 0;
	m_downloaded_protocol = 0;
	m_uploaded_protocol = 0;

	m_mean_download_rate = 0;
	m_mean_upload_rate = 0;
	m_mean_download_payload_rate = 0;
	m_mean_upload_payload_rate = 0;

	for (int i = 0; i < history; ++i)
	{
		m_mean_download_rate += m_download_rate_history[i];
		m_mean_upload_rate += m_upload_rate_history[i];
		m_mean_download_payload_rate += m_download_payload_rate_history[i];
		m_mean_upload_payload_rate += m_upload_payload_rate_history[i];
	}

	m_mean_download_rate /= history;
	m_mean_upload_rate /= history;
	m_mean_download_payload_rate /= history;
	m_mean_upload_payload_rate /= history;
}
