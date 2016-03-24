const {Cc,Ci,Cu} = require("chrome");
Cu.import("resource://gre/modules/Downloads.jsm");
Cu.import("resource://gre/modules/Task.jsm");
var fdmbh = require ("./fdm_bh_ipc.js");

function FdmDownloadsObserver (ipcmgr)
{
	this.ipcmgr = ipcmgr;
	
	this.onDownloadAdded = function (download)
	{
		var task = new fdmbh.FdmBhBrowserDownloadStateReport;
		task.setInfo (download.source.url, "created");	
		this.ipcmgr.sendTask (task);
	};
	
	this.onDownloadChanged = function (download)
	{
		var state;
		if (download.succeeded)
			state = "succeeded";
		else if (download.canceled)
			state = "cancelled";
		else if (download.error)
			state = "failed";
		else
			return;
		var task = new fdmbh.FdmBhBrowserDownloadStateReport;
		task.setInfo (download.source.url, state);	
		this.ipcmgr.sendTask (task);
	};
	
	this.onDownloadRemoved = function (download)
	{
	};
	
	this.register = function ()
	{
		Task.spawn (function () 
		{
			let list = yield Downloads.getList(Downloads.ALL);
			yield list.addView (this);
		}.bind (this));
	};
	
	this.unregister = function ()
	{
		Task.spawn (function () 
		{
			let list = yield Downloads.getList(Downloads.ALL);
			yield list.removeView (this);
		}.bind (this));
	};
}


exports.FdmDownloadsObserver = FdmDownloadsObserver;