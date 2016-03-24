// current implementation uses the technique described here:
// http://stackoverflow.com/questions/26175475/add-an-option-to-the-download-file-dialog

// another way is to use Downloads.jsm
// https://developer.mozilla.org/en-US/docs/Mozilla/JavaScript_code_modules/Downloads.jsm#Observing_downloads
// currently (13.07.2015, Firefox 39) is not quite a good way due to several UI problems.

const {Cc,Ci,Cu} = require("chrome");
const {XulWindowListener} = require ("./wnd_listener.js");
const {URL} = require ("sdk/url");
const Tools = require ("./tools.js");
var { setTimeout } = require ("sdk/timers");
const misc = require ("./misc.js");
Cu.import("resource://gre/modules/Services.jsm");


function extractPostData(channel, url)
{
	if(channel instanceof Ci.nsIUploadChannel &&
       channel.uploadStream instanceof Ci.nsISeekableStream) 
	{
      try 
	  {
        channel.uploadStream.seek(0,0);
        const sis = Cc ['@mozilla.org/scriptableinputstream;1'].createInstance(
          Ci.nsIScriptableInputStream);
        sis.init(channel.uploadStream);
        var index;
        var postData=sis.read(sis.available());
        if (postData.indexOf ("application/x-www-form-urlencoded") >= 0 &&
            url && (index=url.indexOf ("?")) >= 0) 
		{
          // For Content-Type: application/x-www-form-urlencoded
          // get the parameters from the URL after the question mark.
            return url.substring (index+1);
        } 
		else 
		{
          postData=postData.replace(/\s$/,'').split(/[\r\n]/);
          return postData[postData.length-1];
        }
      }
	  catch(ex) 
	  {
	  } 
	  finally 
	  {
        try {
			sis.close();
		}catch(e){}
      }
    }
    return null;
}

function extractCookie(channel, _cookieWeHave)
{
    var _cookie = _cookieWeHave || "";

    if(channel instanceof Ci.nsIUploadChannel &&
       channel.uploadStream instanceof Ci.nsISeekableStream) 
	{
      var cookies = channel.getRequestHeader("Cookie");
      if (cookies) 
	  {
        cookies = cookies.split(/\s*;\s*/);
        if (cookies.length > 0) 
		{
          for (var i = 0, l = cookies.length; i < l; i++) 
		  {
            if (_cookie.indexOf (cookies[i]) == -1)
            {
              if (_cookie.length)
                _cookie += "; ";
              _cookie += cookies[i];
            }
          }
        }
      }
    }

    return _cookie;
}


function LastHttpRequestTracker ()
{
	this.observe = function (subj, topic, data)
	{
		try 
		{
			subj.QueryInterface (Ci.nsIHttpChannel);
			this.channel = subj;
		}
		catch (e)
		{
		}
	};
	
	this.register = function ()
	{
		Services.obs.addObserver (this, "http-on-modify-request", false);
	};
	
	this.unregister = function ()
	{
		Services.obs.removeObserver (this, "http-on-modify-request");
	};
}


function DownloadsInterceptor (httpRedirectsWatcher)
{
	XulWindowListener.call (this, 
		"chrome://mozapps/content/downloads/unknownContentType.xul");
		
	this.httpRedirectsWatcher = httpRedirectsWatcher;
		
	this.lastRequest = new LastHttpRequestTracker;
	
	this.downloadsToSkip = new Set;
	this.processingWindows = new Map;
	
	this.DownloadsInterceptorOrigRegister = this.register;
	this.DownloadsInterceptorOrigUnregister = this.unregister;
	
	this.register = function ()
	{
		this.lastRequest.register ();
		this.DownloadsInterceptorOrigRegister ();
	};
	
	this.unregister = function ()
	{
		this.lastRequest.unregister ();
		this.DownloadsInterceptorOrigUnregister ();
	};
		
	this.loadIntoXulWindow = function (window) 
	{
		var url = window.dialog.mLauncher.source.spec;
		if (this.downloadsToSkip.has (url))
		{
			this.downloadsToSkip.delete (url);
			return;
		}
		
		var info = new misc.DownloadInfo;
		info.url = url;
		
		try {
			info.fileSize = window.dialog.mLauncher.contentLength.toString();
		} catch (err) {}
		
		if (this.httpRedirectsWatcher)
		{
			var reqInfo = this.httpRedirectsWatcher.requests.get (url);
			if (reqInfo)
				info.originalUrl = reqInfo.originalUrl;
		}
		
		try {
			info.userAgent = window.navigator.userAgent;
		} catch (err) {
			info.userAgent = "";
		}
		
		var doc;
		try {
			doc = window.dialog.mContext.QueryInterface(Ci.nsIInterfaceRequestor)
				.getInterface(Ci.nsIDOMWindow).document;
		} catch(err) {
			doc = window.top.opener && window.top.opener.content && window.top.opener.content.document || null;
		}
		
		info.httpCookies = "";
		
		try {
			if (doc.URL && doc.URL != "about:blank")
				info.httpReferer = doc.URL;
			else if (doc.baseURI && doc.baseURI != "about:blank")
				info.httpReferer = doc.baseURI;
		} catch(err) {}
		
		try {
			info.httpCookies = Tools.getCookiesFromHost (URL (url).host);
		} catch (err) {}
		
		if (this.lastRequest.channel)
		{
			if (info.url == this.lastRequest.channel.URI.spec)
			{
				var postData = extractPostData (this.lastRequest.channel, info.url);
				if (postData)
					info.httpPostData = postData;
				try {
					info.httpCookies = extractCookie (this.lastRequest.channel, info.httpCookies);
				} catch (err) {}
			}
		}
		
		info.suggestedName = window.dialog.mLauncher.suggestedFileName;
		
		this.processingWindows.set (info, window);
			
		this.showDownloadWindow (window, false);
		
		this.onNewDownload (info, function (info, added) 
		{
			if (!added)
				this.proceedDownloadWithBrowser (info);
			else
				this.cancelDownloadWithBrowser (info);
		}.bind (this));
	};
	
	// return true to abort the download in the browser
	this.onNewDownload = function (info, callback) {callback (info, false /*download was not added*/);};
	
	this.proceedDownloadWithBrowser = function (info)
	{
		this.showDownloadWindow (
			this.processingWindows.get (info), true);
		this.processingWindows.delete (info);
	};
	
	this.cancelDownloadWithBrowser = function (info)
	{
		this.processingWindows.get (info).document.documentElement.cancelDialog();
		this.processingWindows.delete (info);
	};
	
	this.showDownloadWindow = function (window, show)
	{
		throw "not implemented";
	};
}


exports.DownloadsInterceptor = DownloadsInterceptor;