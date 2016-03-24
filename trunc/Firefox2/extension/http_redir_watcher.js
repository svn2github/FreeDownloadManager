const {Cc,Ci,Cu} = require("chrome");
Cu.import("resource://gre/modules/Services.jsm");

function HttpRedirectsWatcher ()
{
	this.requests = new Map;
	
	this.observe = function (subj, topic, data)
	{
		try 
		{
			subj.QueryInterface (Ci.nsIHttpChannel);
			
			if (subj.URI.spec != subj.originalURI.spec)
			{
				this.removeTooOldRequests ();
				
				var reqInfo = new Object;
				reqInfo.originalUrl = subj.originalURI.spec;
				reqInfo.time = new Date;
			
				this.requests.set (subj.URI.spec, reqInfo);
			}
		}
		catch (err)
		{
		}	
	};
	
	this.removeTooOldRequests = function ()
	{
		var currentTime = (new Date).getTime ();
		
		this.requests.forEach (function (elem, key, map)
		{
			if ((currentTime - elem.time.getTime ()) >= 3*60*1000)
				map.delete (key);
		});
	};
	
	this.register = function ()
	{
		Services.obs.addObserver (this, "http-on-examine-response", false);
	};
	
	this.unregister = function ()
	{
		Services.obs.removeObserver (this, "http-on-examine-response");
	};
}


exports.HttpRedirectsWatcher = HttpRedirectsWatcher;