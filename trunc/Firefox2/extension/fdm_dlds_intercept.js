const {Cc,Ci,Cu} = require("chrome");
const {DownloadsInterceptor} = require ("./dlds_intercept.js");
var fdmbh = require ("./fdm_bh_ipc.js");
const System = require("./system.js");

function FdmDownloadsInterceptor (ipcmgr, settings, httpRedirectsWatcher)
{
	DownloadsInterceptor.call (this, httpRedirectsWatcher);
	this.ipcmgr = ipcmgr;
	this.settings = settings;
	
	this.onNewDownload = function (info, callback)
	{
		var task = new fdmbh.FdmBhCreateDownloadsTask;
		task.addDownload (info);
		
		task.create_downloads.catchedDownloads = "1";
		task.create_downloads.waitResponse = "1";
		
		this.ipcmgr.sendTask (task, function (resp)
		{
			var eaten = true;
			
			if (resp.error)
			{
				eaten = false;
			}
			else
			{
				if (resp.result == "0" &&  // download was not added to FDM?
					this.settings.browser.monitor.allowDownload != "0")
				{
					eaten = false;
				}
			}
		
			callback (info, eaten);
			
		}.bind (this));
	};
	
	this.showDownloadWindow = function (window, show)
	{
		var baseWindow = window.QueryInterface(Ci.nsIInterfaceRequestor).
			getInterface(Ci.nsIWebNavigation).
            QueryInterface(Ci.nsIDocShellTreeItem).
			treeOwner.
			QueryInterface(Ci.nsIInterfaceRequestor).
			getInterface(Ci.nsIBaseWindow);
			
		try
		{
			System.showWindow2(baseWindow, show, 
				show ? false : true);
		}
		catch(e)
		{
			var task = new fdmbh.FdmBhWindowTask;
			task.showWindow (baseWindow.nativeHandle, show, show ? false : true);
		
			this.ipcmgr.sendTask (task);
		}
	};
}
FdmDownloadsInterceptor.prototype = Object.create (DownloadsInterceptor.prototype);


exports.FdmDownloadsInterceptor = FdmDownloadsInterceptor;