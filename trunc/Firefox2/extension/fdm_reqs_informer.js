const {Cc,Ci,Cu,Cr} = require("chrome");
var fdmbh = require ("./fdm_bh_ipc.js");
const {FdmNetworkRequestListener} = require ("./FdmNetworkRequestListener.js");
Cu.import("resource://gre/modules/Services.jsm");

function ExtractUrlFromHttpChannel (channel)
{
  return channel.URI.prePath + channel.URI.path;
}

function findSubjectWindow (aSubject)
{
	var oHttp = aSubject.QueryInterface(Ci.nsIHttpChannel);
	var notificationCallbacks;
	try 
	{
		notificationCallbacks = oHttp.notificationCallbacks ? 
			oHttp.notificationCallbacks : oHttp.loadGroup.notificationCallbacks;
	}
	catch  (e)
	{
		return null;
	}
	
	if (!notificationCallbacks)
		return null;
	
	var interfaceRequestor = notificationCallbacks.QueryInterface (
		Ci.nsIInterfaceRequestor);
		
	var loadContext;
	
	try 
	{
		loadContext = interfaceRequestor.getInterface (Ci.nsILoadContext);
	} 
	catch (ex) 
	{
		try 
		{
			loadContext = aSubject.loadGroup.notificationCallbacks.getInterface (Ci.nsILoadContext);
		} 
		catch (ex2) 
		{
			return null;
		}
	}
		
	try 
	{
		var contentWindow = loadContext.associatedWindow; //this is the HTML window of the page that just loaded
		return contentWindow;
	}
	catch(ex)
	{
		return null;
	}
	/*//aDOMWindow this is the firefox window holding the tab
    var aDOMWindow = contentWindow.top.QueryInterface(Ci.nsIInterfaceRequestor).getInterface(Ci.nsIWebNavigation).QueryInterface(Ci.nsIDocShellTreeItem).rootTreeItem.QueryInterface(Ci.nsIInterfaceRequestor).getInterface(Ci.nsIDOMWindow);
    var gBrowser = aDOMWindow.gBrowser; //this is the gBrowser object of the firefox window this tab is in
    var aTab = gBrowser._getTabForContentWindow(contentWindow.top); //this is the clickable tab xul element, the one found in the tab strip of the firefox window, aTab.linkedBrowser is same as browser var above //can stylize tab like aTab.style.backgroundColor = 'blue'; //can stylize the tab like aTab.style.fontColor = 'red';
    var browser = aTab.linkedBrowser; //this is the browser within the tab //this is what the example in the previous section gives*/
}


function FdmHttpRequestsListener (ipcmgr)
{
	this.nextRequestId = 1;
	this.ipcmgr = ipcmgr;
	
	this.observe = function (subject, topic, data) 
	{
		if (false == (subject instanceof Ci.nsIHttpChannel))
			return;

		var channel = subject.QueryInterface (Ci.nsIHttpChannel);
		var url = ExtractUrlFromHttpChannel (channel);

		if (topic == "http-on-modify-request")
		{
			{
				var task = new fdmbh.FdmBhNetworkRequestActivityNotification;
				task.setInfo (url);
				this.ipcmgr.sendTask (task);
			}
			
			var wndSrc = findSubjectWindow (subject);
			if (wndSrc)
			{
				wndSrc = wndSrc.top;
				if (wndSrc)
				{
					var task = new fdmbh.FdmBhNewNetworkRequestNotification;
					task.setInfo (url, wndSrc.location.href);
					this.ipcmgr.sendTask (task);
				}
			}
			
			return;
		}

		var newListener = new FdmNetworkRequestListener (
			channel, topic, url, this.nextRequestId++, this.ipcmgr);
	};
	
	this.QueryInterface = function (aIID) 
	{
		if (!aIID.equals (Ci.nsISupports) && 
			!aIID.equals (Ci.nsIObserver))
		{
			throw Cr.NS_ERROR_NO_INTERFACE;
		}
		return this;
	};
	
	this.register = function ()
	{
		Services.obs.addObserver (this, "http-on-modify-request", false);
		Services.obs.addObserver (this, "http-on-examine-response", false);
		Services.obs.addObserver (this, "http-on-examine-cached-response", false);
		Services.obs.addObserver (this, "http-on-examine-merged-response", false);
	};
	
	this.unregister = function ()
	{
		Services.obs.removeObserver (this, "http-on-modify-request");
		Services.obs.removeObserver (this, "http-on-examine-response");
		Services.obs.removeObserver (this, "http-on-examine-cached-response");
		Services.obs.removeObserver (this, "http-on-examine-merged-response");
	};
}



exports.FdmHttpRequestsListener = FdmHttpRequestsListener;