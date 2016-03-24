var fdmbh = require ("./fdm_bh_ipc.js");
const ffproxy = require ("./ff_proxy.js");

function FdmFfProxyManager (ipcmgr)
{
	this.ipcmgr = ipcmgr;
	
	this.observer = new ffproxy.ProxyChangeObserver;
	this.observer.observe = function ()
	{
		this.sendProxySettings ();
	}.bind (this);
	
	this.sendProxySettings = function ()
	{
		var proxies = new ffproxy.ProxiesInfo;
		proxies.query ();
		proxies.proxies.forEach (function (proxy)
		{
			var task = new fdmbh.FdmBhBrowserProxyTask;
			task.setProxy (proxy);
			this.ipcmgr.sendTask (task);
		}.bind (this));
	};
	
	this.initialize = function ()
	{
		this.sendProxySettings ();
	};
	
	this.register = function ()
	{
		this.observer.register ();
	}
	
	this.unregister = function ()
	{
		this.observer.unregister ();
	}
}


exports.FdmFfProxyManager = FdmFfProxyManager;