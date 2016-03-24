const {Cc,Ci,Cu} = require("chrome");
var {FdmAppInfo} = require ("./fdm_app_info.js");
var {DmContextMenus} = require ("./dm_ctx_menus.js");
var fdmbh = require ("./fdm_bh_ipc.js");
const {URL} = require ("sdk/url");
const Tools = require ("./tools.js");
const {FdmDownloadsInterceptor} = require ("./fdm_dlds_intercept.js");
Cu.import("resource://gre/modules/Services.jsm");
const misc = require ("./misc.js");
const fdmffproxy = require ("./fdm_ffproxy.js");
const {FdmDownloadVideoButton} = require ("./fdm_dlvid_btn.js");
const {FdmCsRequestsProcessor} = require ("./fdm_cs_requests.js");
const {HttpRedirectsWatcher} = require ("./http_redir_watcher.js");
const {FdmHttpRequestsListener} = require ("./fdm_reqs_informer.js");
const {FdmDownloadsObserver} = require ("./fdm_dlds_obs.js");


function FdmExtension ()
{
	this.fdmAppInfo = new FdmAppInfo;

	this.InitializeContextMenus ();
	
	this.ipcmgr = new fdmbh.FdmBhIpcManager (this.fdmAppInfo.brhlpr_path);
	this.ipcmgr.onBhExeUnexpectedShutdown = this.onFdmBhExeUnexpectedShutdown.bind (this);
	this.ipcmgr.onReadyToUse = this.onFdmBhReadyToUse.bind (this);
	
	this.httpRedirectsWatcher = new HttpRedirectsWatcher;
}

FdmExtension.prototype.register = function ()
{
	this.httpRedirectsWatcher.register ();
}

FdmExtension.prototype.unregister = function ()
{
	this.httpRedirectsWatcher.unregister ();
	this.dlCatcher.unregister ();
	this.ffproxyManager.unregister ();
	this.networkRequestsListener.unregister ();
	this.downloadsObserver.unregister ();
	this.ipcmgr.shutdown ();
}

FdmExtension.prototype.onFdmBhReadyToUse = function ()
{
	if (this.ipcmgr.bhExeLaunchCount > 1)
		return;
	this.ipcmgr.sendTask (new fdmbh.FdmBhQuerySettingsTask, function (resp)
	{
		this.onQuerySettingsResponse (resp);
	}.bind (this));
}

FdmExtension.prototype.onFdmBhExeUnexpectedShutdown = function ()
{
	var notifications = require("sdk/notifications");
	notifications.notify({
		title: "Free Download Manager",
		text: "Fatal error: browser helper exe unexpected shutdown. Please restart the browser to continue the use of Free Download Manager extension."
	});
}

FdmExtension.prototype.onUiStringsResponse = function (resp)
{
	this.uiStrings = resp.strings;
	this.onIpcInitResponsesPerformed ();
}

FdmExtension.prototype.onIpcInitResponsesPerformed = function ()
{
	this.initializeIpcRequiringObjects ();
	this.initializeUiObjects ();
}

FdmExtension.prototype.initializeIpcRequiringObjects = function ()
{
	////////////////////////////////////////////////////////////
	// objs requiring IPC can be initialized here
	////////////////////////////////////////////////////////////
	this.csRequestsProcessor = new FdmCsRequestsProcessor (
		this.ipcmgr, this.settings, this.uiStrings);
		
	this.dlCatcher = new FdmDownloadsInterceptor (
		this.ipcmgr, this.settings, this.httpRedirectsWatcher);
	this.dlCatcher.register (); // now it's ready to catch downloads
	
	this.ffproxyManager = new fdmffproxy.FdmFfProxyManager (
		this.ipcmgr);
	this.ffproxyManager.initialize ();
	this.ffproxyManager.register ();
	
	this.networkRequestsListener = new FdmHttpRequestsListener (
		this.ipcmgr);
	this.networkRequestsListener.register ();
	
	this.downloadsObserver = new FdmDownloadsObserver (
		this.ipcmgr);
	this.downloadsObserver.register ();
	/////////////////////////////////////////////////////////////
}

FdmExtension.prototype.initializeUiObjects = function ()
{
	this.ctxMenus.dlLink.menuInfo.label = this.uiStrings.dllink;
	this.ctxMenus.dlAll.menuInfo.label = this.uiStrings.dlall;
	this.ctxMenus.dlSelected.menuInfo.label = this.uiStrings.dlselected;
	this.ctxMenus.dlVideo.menuInfo.label = this.uiStrings.dlvideo;
	
	if (this.settings.browser.menu.dllink != "0")
		this.ctxMenus.dlLink.registerMenu ();
	if (this.settings.browser.menu.dlall != "0")
		this.ctxMenus.dlAll.registerMenu ();
	if (this.settings.browser.menu.dlselected != "0")
		this.ctxMenus.dlSelected.registerMenu ();
	if (this.settings.browser.menu.dlvideo != "0")
		this.ctxMenus.dlVideo.registerMenu ();
		
	if (!this.settings.browser.monitor.hasOwnProperty ("video") ||
	    !this.settings.browser.monitor.video.hasOwnProperty ("showButton") ||
	    this.settings.browser.monitor.video.showButton != "0")
	{
		this.dlvidBtn = new FdmDownloadVideoButton (
			this.csRequestsProcessor);
	}
}

FdmExtension.prototype.onQuerySettingsResponse = function (resp)
{
	this.settings = resp.settings;
	
	this.ipcmgr.sendTask (new fdmbh.FdmBhUiStringsTask, function (resp)
	{
		this.onUiStringsResponse (resp);
	}.bind (this));
}

FdmExtension.prototype.InitializeContextMenus = function ()
{
	this.ctxMenus = new DmContextMenus ();
	
	this.createDownloads = function (info)
	{
		var cookies = Tools.getCookiesFromHost (
			URL (info.pageUrl).host);
		
		var task = new fdmbh.FdmBhCreateDownloadsTask;
		
		info.urls.forEach (function (url)
		{
			if (!url)
				return;
			var parsedURL = new URL (url);
			var dlinfo = new misc.DownloadInfo (url);
			dlinfo.httpReferer = info.pageUrl;
			dlinfo.userAgent = info.userAgent;
			if (parsedURL.host)
				dlinfo.httpCookies = Tools.getCookiesFromHost (parsedURL.host);
			task.addDownload (dlinfo);
		});
		
		if (task.hasDownloads())
			this.ipcmgr.sendTask (task);
	};
	
	this.createVideoDownload = function (info)
	{
		var task = new fdmbh.FdmBhVideoSnifferTask;
		var req = new Object;
		req.name = "CreateVideoDownloadFromUrl";
		req.webPageUrl = info.webPageUrl;
		task.setRequest (req);
		this.ipcmgr.sendTask (task, function (resp)
		{
			var fdmUtil = require ("./data/fdm_util.js");
			var resultMsg = fdmUtil.VideoSnifferRequestResultToString (
				resp.videoSniffer, this.uiStrings);
			if (resultMsg)
			{
				var notifications = require("sdk/notifications");
				notifications.notify({
					title: "Free Download Manager",
					text: resultMsg
				});
			}
		}.bind (this));
	};
	
	this.ctxMenus.dlLink.menuInfo.setOnClickFn (
		this.createDownloads.bind (this));
	
	this.ctxMenus.dlAll.menuInfo.setOnClickFn (
		this.createDownloads.bind (this));
		
	this.ctxMenus.dlSelected.menuInfo.setOnClickFn (
		this.createDownloads.bind (this));
		
	this.ctxMenus.dlVideo.menuInfo.setOnClickFn (
		this.createVideoDownload.bind (this));
}


exports.FdmExtension = FdmExtension;