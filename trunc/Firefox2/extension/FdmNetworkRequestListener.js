const {Cc,Ci,Cu,Cr} = require("chrome");
var fdmbh = require ("./fdm_bh_ipc.js");
const {NetworkRequestListener} = require ("./NetworkRequestListener.js");
var encodings = require("./encodings.js");

function httpHeaders ()
{
}
httpHeaders.prototype =
{
    m_Headers       : "",

    visitHeader: function(name, value)
    {
      this.m_Headers += name + ": " + value + "\r\n"; 
    },

    QueryInterface: function (aIID) 
	{
        if (aIID.equals(Ci.nsIHttpHeaderVisitor) || 
			aIID.equals(Ci.nsISupports))
		{
            return this;
		}
        throw Cr.NS_NOINTERFACE;
    }
};

function ExtractHttpHeadersFromHttpChannel (channel)
{
  var reqH = channel.requestMethod + " " + channel.URI.path + " HTTP/1.1" + "\r\n";
  var hdrs = new httpHeaders ();
  channel.visitRequestHeaders (hdrs);
  reqH += hdrs.m_Headers;
  delete hdrs;
  var respH = "HTTP/1.0 " + channel.responseStatus + " " + channel.responseStatusText + "\r\n";
  hdrs = new httpHeaders;
  channel.visitResponseHeaders (hdrs);
  respH += hdrs.m_Headers;
  delete hdrs;
  return {reqH : reqH, respH : respH};
}


function FdmNetworkRequestListener (channel, topic, url, id, ipcmgr)
{
	NetworkRequestListener.call (this);
	
	this.dataCountSent = 0;
	this.url = url;
	this.requestId = id;
	this.ipcmgr = ipcmgr;
	
	// temporarily disabled due to addon sdk bugs
	/*if (topic == "http-on-examine-cached-response")
	{
		this.dontExtractData = false;
		this.responseNotifications = true;
	}*/
		
	if (!this.responseNotifications)
	{
		var task = new fdmbh.FdmBhNetworkRequestActivityNotification;
		task.setInfo (url);
		this.ipcmgr.sendTask (task);
	}
	else
	{
		var hdrs = ExtractHttpHeadersFromHttpChannel (channel);
		var task = new fdmbh.FdmBhNetworkRequestResponseNotification;
		task.setInfo (this.requestId, this.url, hdrs.reqH, hdrs.respH);
		this.ipcmgr.sendTask (task);
	}
	
	channel.QueryInterface (Ci.nsITraceableChannel);
	this.originalListener = channel.setNewListener (this);
}

FdmNetworkRequestListener.prototype = Object.create (NetworkRequestListener.prototype);

FdmNetworkRequestListener.prototype.responseNotifications = false;

FdmNetworkRequestListener.prototype.onNetworkRequestDataReceived = 
	function (count, data)
{
	if (!data)
	{
		var task = new fdmbh.FdmBhNetworkRequestActivityNotification;
		task.setInfo (this.url);
		this.ipcmgr.sendTask (task);
	}
	else
	{
		const MAX_SIZE = 1000; // addon sdk bug workaround
		const MAX_TOTAL_SIZE = 15000;
		for (var i = 0; i < count && this.dataCountSent < MAX_TOTAL_SIZE;)
		{
			var dataStr = "";
			for (var j = 0; 
				j < MAX_SIZE && i < count && this.dataCountSent < MAX_TOTAL_SIZE; 
				j++, i++, this.dataCountSent++)
			{
				dataStr += String.fromCharCode (data[i]);
			}
			var dataEncoded = encodings.to_base64 (dataStr);
			var task = new fdmbh.FdmBhNetworkRequestResponseDataNotification;
			task.setInfo (this.requestId, dataEncoded);
			this.ipcmgr.sendTask (task);
		}
		if (this.dataCountSent >= MAX_TOTAL_SIZE)
			this.dontExtractData = true;
	}
}

FdmNetworkRequestListener.prototype.onNetworkRequestClosed = 
	function ()
{
	if (this.responseNotifications)
	{
		var task = new fdmbh.FdmBhNetworkRequestResponseCompleteNotification;
		task.setInfo (this.requestId);
		this.ipcmgr.sendTask (task);
	}
}


exports.FdmNetworkRequestListener = FdmNetworkRequestListener;