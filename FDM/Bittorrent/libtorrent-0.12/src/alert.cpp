/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include "libtorrent/alert.hpp"

namespace libtorrent {

	alert::alert(severity_t severity, const std::string& msg)
		: m_msg(msg)
		, m_severity(severity)
		, m_timestamp(boost::posix_time::second_clock::universal_time())
	{
	}

	alert::~alert()
	{
	}

	boost::posix_time::ptime alert::timestamp() const
	{
		return m_timestamp;
	}

	const std::string& alert::msg() const
	{
		return m_msg;
	}

	alert::severity_t alert::severity() const
	{
		return m_severity;
	}  

	alert_manager::alert_manager()
		: m_severity(alert::none)
	{}

	alert_manager::~alert_manager()
	{
		while (!m_alerts.empty())
		{
			delete m_alerts.front();
			m_alerts.pop();
		}
	}

	void alert_manager::post_alert(const alert& alert_)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		if (m_severity > alert_.severity()) return;

		
		if (m_alerts.size() == 100)
		{
			alert* result = m_alerts.front();
			m_alerts.pop();
			delete result;
		}
		m_alerts.push(alert_.clone().release());
	}

	std::auto_ptr<alert> alert_manager::get()
	{
		boost::mutex::scoped_lock lock(m_mutex);
		
		assert(!m_alerts.empty());

		alert* result = m_alerts.front();
		m_alerts.pop();
		return std::auto_ptr<alert>(result);
	}

	bool alert_manager::pending() const
	{
		boost::mutex::scoped_lock lock(m_mutex);
		
		return !m_alerts.empty();
	}

	void alert_manager::set_severity(alert::severity_t severity)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		
		m_severity = severity;
	}
	
	bool alert_manager::should_post(alert::severity_t severity) const
	{
		return severity >= m_severity;
	}

} 

