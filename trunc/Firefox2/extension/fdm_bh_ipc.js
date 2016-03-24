var {SimpleJsonIpcItem, SimpleJsonIpcManager} = require ("./simple_json_ipc.js");
const misc = require ("./misc.js");
const ffproxy = require ("./ff_proxy.js");
var child_process = require("sdk/system/child_process");
const {RequestsManager} = require ("./data/RequestsManager.js");
const Constants = require("./constants.js");


function FdmBhIpcTask (id, type)
{
	this.id = id || 0;
	this.type = type || "";
	if (typeof this.id == "number")
		this.id = this.id.toString ();
}

function FdmBhHandshakeTask (id)
{
	FdmBhIpcTask.call (this, id, "handshake");
	this.handshake = new Object;
	this.handshake.api_version = "1";
	this.handshake.response_send_data_len_as_string = "1";
	this.handshake.base64_encoding_for_rcv = "1";
	this.handshake.base64_encoding_for_send = "1";
	this.handshake.browser = "Firefox";
}

function FdmBhUiStringsTask (id)
{
	FdmBhIpcTask.call (this, id, "ui_strings");
}

function FdmBhQuerySettingsTask (id)
{
	FdmBhIpcTask.call (this, id, "query_settings");
}

function FdmBhShutdownTask (id)
{
	FdmBhIpcTask.call (this, id, "shutdown");
}

function FdmBhCreateDownloadsTask (id)
{
	FdmBhIpcTask.call (this, id, "create_downloads");
	
	this.create_downloads = new Object;
	this.create_downloads.downloads = [];

	this.addDownload = function (download)
	{
		if (!download instanceof misc.DownloadInfo)
			throw "invalid type";
		this.create_downloads.downloads.push (download);
	};
	
	this.hasDownloads = function ()
	{
		return this.create_downloads.downloads.length != 0;
	};
}

function FdmBhBrowserProxyTask (id)
{
	FdmBhIpcTask.call (this, id, "browser_proxy");
	
	this.setProxy = function (proxy)
	{
		this.browser_proxy = new Object;
		this.browser_proxy.type = proxy.type.toString();
		this.browser_proxy.protocol = proxy.protocol;
		this.browser_proxy.addr = proxy.addr;
		this.browser_proxy.port = proxy.port.toString();
	};
}

function FdmBhWindowTask (id)
{
	FdmBhIpcTask.call (this, id, "window");
	this.window = new Object;
	
	this.showWindow = function (handle, show, needWait)
	{
		this.window.windowHandle = handle;
		this.window.action = show ? "show" : "hide";
		this.window.needWait = needWait ? "1" : "0";
	};
}

function FdmBhVideoSnifferTask (id)
{
	FdmBhIpcTask.call (this, id, "video_sniffer");
	
	this.setRequest = function (req)
	{
		this.video_sniffer = req;
	};
}

function FdmBhNetworkRequestNotification (id, type)
{
	FdmBhIpcTask.call (this, id, "network_request_notification");
	this.network_request_notification = new Object;
	this.network_request_notification.type = type;
}

function FdmBhNewNetworkRequestNotification (id)
{
	FdmBhNetworkRequestNotification.call (this, id, "new");
	
	this.setInfo = function (url, srcTabUrl)
	{
		this.network_request_notification.url = url;
		this.network_request_notification.srcTabUrl = srcTabUrl;
	};
}

function FdmBhNetworkRequestActivityNotification (id)
{
	FdmBhNetworkRequestNotification.call (this, id, "activity");
	
	this.setInfo = function (url)
	{
		this.network_request_notification.url = url;
	};
}

function FdmBhNetworkRequestResponseNotification (id)
{
	FdmBhNetworkRequestNotification.call (this, id, "response");
	
	this.setInfo = function (requestId, url, requestHeaders, responseHeaders)
	{
		this.network_request_notification.requestId = requestId;
		this.network_request_notification.url = url;
		this.network_request_notification.requestHeaders = requestHeaders;
		this.network_request_notification.responseHeaders = responseHeaders;
	};
}

function FdmBhNetworkRequestResponseDataNotification (id)
{
	FdmBhNetworkRequestNotification.call (this, id, "response_data");
	
	this.setInfo = function (requestId, data)
	{
		this.network_request_notification.requestId = requestId;
		this.network_request_notification.data = data;
	};
}

function FdmBhNetworkRequestResponseCompleteNotification (id)
{
	FdmBhNetworkRequestNotification.call (this, id, "response_complete");
	
	this.setInfo = function (requestId)
	{
		this.network_request_notification.requestId = requestId;
	};
}

function FdmBhBrowserDownloadStateReport (id)
{
	FdmBhIpcTask.call (this, id, "browser_download_state_report");
	this.browser_download_state_report = new Object;
	
	this.setInfo = function (url, state)
	{
		this.browser_download_state_report.url = url;
		this.browser_download_state_report.state = state;
	};
}

function FdmBhIpcManager (bhpath)
{
	this.shuttingDown = false;
	this.bhExeLaunchCount = 0;
	this.scheduledRequests = new Array;
	
	this.reqsMan = new RequestsManager;
	this.reqsMan.sendRequest = function (req)
	{
		this.sendItem (new SimpleJsonIpcItem (req));
	}.bind (this);
	
	this.launchBhExe = function (constructing)
	{
		this.bhExeLaunchCount++;
		this.bhExeLaunchTime = new Date;
		this.brhlpr_process = child_process.spawn (bhpath, Constants.BrowserHelperArgs);
		this.ready = false;
		
		if (constructing)
		{
			SimpleJsonIpcManager.call (this, this.brhlpr_process.stdin, this.brhlpr_process.stdout);
			this.onJsonItemRcvd = function (item)
			{
				this.reqsMan.onRequestResponse (item.object);
			};
		}
		else
		{
			this.reqsMan.closeRequestsInProgress (function (id, callback)
			{
				if (callback)
				{
					var resp = new Object;
					resp.id = id;
					resp.error = "ipc failure";
					callback (resp);
				}				
			});
			this.resetState (this.brhlpr_process.stdin, this.brhlpr_process.stdout);
		}
		
		this.brhlpr_process.on ('close', function ()
		{
			if (!this.shuttingDown)
			{
				var minSeconds = this.bhExeLaunchCount > 3 ? 10 : this.bhExeLaunchCount*3;
				if ((new Date).getTime () - this.bhExeLaunchTime.getTime () > minSeconds*1000)
					this.launchBhExe (false);
				else
					this.onBhExeUnexpectedShutdown ();
			}
		}.bind (this));
		
		this.itemSender.useBase64 = false;
		this.reqsMan.performRequest (new FdmBhHandshakeTask, this.onGotHandshake.bind (this));
	};
		
	this.sendTask = function (task, callback)
	{
		if (this.ready)
		{
			this.reqsMan.performRequest (task, callback);
		}
		else
		{
			var o = new Object;
			o.task = task;
			o.callback = callback;
			this.scheduledRequests.push (o);
		}
	};
		
	this.onGotHandshake = function (resp)
	{
		this.itemSender.useBase64 = true;
		this.ready = true;
		this.onReadyToUse ();
		for (var i = 0; i < this.scheduledRequests.length; i++)
			this.sendTask (this.scheduledRequests [i].task, this.scheduledRequests [i].callback);
		this.scheduledRequests = [];
	};
	
	// called by manager when it's ready to accept tasks
	this.onReadyToUse = function () {};
	
	this.shutdown = function ()
	{
		this.shuttingDown = true;
		this.sendTask (new FdmBhShutdownTask);
	};
	
	this.onBhExeUnexpectedShutdown = function () {};
	
	this.launchBhExe (true);
}


exports.FdmBhIpcTask = FdmBhIpcTask;
exports.FdmBhHandshakeTask = FdmBhHandshakeTask;
exports.FdmBhUiStringsTask = FdmBhUiStringsTask;
exports.FdmBhQuerySettingsTask = FdmBhQuerySettingsTask;
exports.FdmBhCreateDownloadsTask = FdmBhCreateDownloadsTask;
exports.FdmBhBrowserProxyTask = FdmBhBrowserProxyTask;
exports.FdmBhShutdownTask = FdmBhShutdownTask;
exports.FdmBhWindowTask = FdmBhWindowTask;
exports.FdmBhVideoSnifferTask = FdmBhVideoSnifferTask;
exports.FdmBhNewNetworkRequestNotification = FdmBhNewNetworkRequestNotification;
exports.FdmBhNetworkRequestActivityNotification = FdmBhNetworkRequestActivityNotification;
exports.FdmBhNetworkRequestResponseNotification = FdmBhNetworkRequestResponseNotification;
exports.FdmBhNetworkRequestResponseDataNotification = FdmBhNetworkRequestResponseDataNotification;
exports.FdmBhNetworkRequestResponseCompleteNotification = FdmBhNetworkRequestResponseCompleteNotification;
exports.FdmBhBrowserDownloadStateReport = FdmBhBrowserDownloadStateReport;
exports.FdmBhIpcManager = FdmBhIpcManager;