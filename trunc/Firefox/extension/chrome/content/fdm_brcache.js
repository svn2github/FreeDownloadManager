var fdm_CacheMon = Components.classes["@freedownloadmanager.org/FDMFfCacheMonitor;1"].createInstance ();
fdm_CacheMon.QueryInterface (Components.interfaces.IFDMFfCacheMonitor);

var fdm_FDM1 = Components.classes["@freedownloadmanager.org/FDMForFirefox;1"].createInstance ();
fdm_FDM1.QueryInterface (Components.interfaces.IFDMForFirefox);


function fdm_TracingListener() {

}

fdm_TracingListener.prototype =
{
    originalListener: null,
    httpDlgUID : 0,

    onStartRequest: function(request, context) {
        this.originalListener.onStartRequest(request, context);
    },

    onDataAvailable: function(request, context, inputStream, offset, count)
    {
        //fix for firebug error
                if (typeof Cc == "undefined") {
                var Cc = Components.classes;
                }
                if (typeof Ci == "undefined") {
        var Ci = Components.interfaces;
                }
                if (typeof CCIN == "undefined") {
                function CCIN(cName, ifaceName){
                return Cc[cName].createInstance(Ci[ifaceName]);
                }
                }
                if (typeof CCSV == "undefined") {
                function CCSV(cName, ifaceName){
                if (Cc[cName])
                        // if fbs fails to load, the error can be _CC[cName] has no properties
                        return Cc[cName].getService(Ci[ifaceName]);
                else
                        dumpError("CCSV fails for cName:" + cName);
                }
                }

        var binaryInputStream = CCIN("@mozilla.org/binaryinputstream;1",
                "nsIBinaryInputStream");
        var storageStream = CCIN("@mozilla.org/storagestream;1", "nsIStorageStream");
        var binaryOutputStream = CCIN("@mozilla.org/binaryoutputstream;1","nsIBinaryOutputStream");

        binaryInputStream.setInputStream(inputStream);
        storageStream.init(8192, count, null);
        binaryOutputStream.setOutputStream(storageStream.getOutputStream(0));

        // Copy received data as they come.
        var data = binaryInputStream.readByteArray (count);
        
	fdm_CacheMon.OnDataReceived (this.httpDlgUID, count, data);

        binaryOutputStream.writeByteArray(data, count);

        this.originalListener.onDataAvailable (request, context,
            storageStream.newInputStream(0), offset, count);
    },

    onStopRequest: function(request, context, statusCode)
    {
        fdm_CacheMon.OnDialogClosed (this.httpDlgUID);
        this.originalListener.onStopRequest(request, context, statusCode);
    },

    QueryInterface: function (aIID) {
        if (aIID.equals(Components.interfaces.nsIStreamListener) || aIID.equals(Components.interfaces.nsISupports))
        {
            return this;
        }
        throw Components.results.NS_NOINTERFACE;
    }
};

function fdm_httpHeaders () {}
fdm_httpHeaders.prototype =
{
    m_Headers       : "",

    visitHeader: function(name, value)
    {
      this.m_Headers += name + ": " + value + "\r\n"; 
    },

    QueryInterface: function (aIID) {
        if (aIID.equals(Components.interfaces.nsIHttpHeaderVisitor) || aIID.equals(Components.interfaces.nsISupports))
            return this;
        throw Components.results.NS_NOINTERFACE;
    }
};

function fdm_ExtractUrlFromHttpChannel (channel)
{
  return channel.URI.prePath + channel.URI.path;
}

function fdm_ExtractHttpHeadersFromHttpChannel (channel)
{
  var reqH = channel.requestMethod + " " + channel.URI.path + " HTTP/1.1" + "\r\n";
  var hdrs = new fdm_httpHeaders ();
  channel.visitRequestHeaders (hdrs);
  reqH += hdrs.m_Headers;
  delete hdrs;
  var respH = "HTTP/1.0 " + channel.responseStatus + " " + channel.responseStatusText + "\r\n";
  hdrs = new fdm_httpHeaders;
  channel.visitResponseHeaders (hdrs);
  respH += hdrs.m_Headers;
  delete hdrs;
  return {reqH : reqH, respH : respH};
}

function fdm_findChannelWindow (aChannel)
{
  try {
   var notificationCallbacks = aChannel.notificationCallbacks ? 
     aChannel.notificationCallbacks : aChannel.loadGroup.notificationCallbacks;
 
   if (!notificationCallbacks)
      return null;
 
    return notificationCallbacks.getInterface(Components.interfaces.nsIDOMWindow);
  }
  catch (e) {return null;}
}

var fdm_brCacheListener = { 
 observe: function (subject, topic, data) {
   if (false == (subject instanceof Components.interfaces.nsIHttpChannel))
     return;
   subject.QueryInterface(Components.interfaces.nsIHttpChannel);
   var url = fdm_ExtractUrlFromHttpChannel (subject);

   if (topic == "http-on-modify-request")
   {
     var wndSrc = fdm_findChannelWindow (subject);
     if (wndSrc)
     {
       wndSrc = wndSrc.top;
       if (wndSrc)
         fdm_FDM1.OnNewHttpRequest (url, wndSrc.location.href);
     }
     return;
   }
   
   var hdrs = fdm_ExtractHttpHeadersFromHttpChannel (subject);
   var newListener = new fdm_TracingListener();
   newListener.httpDlgUID = fdm_CacheMon.OnNewHttpDialog (url, hdrs.reqH, hdrs.respH);
   subject.QueryInterface(Components.interfaces.nsITraceableChannel);
   newListener.originalListener = subject.setNewListener(newListener);
 },

 QueryInterface: function (aIID) {
   if (!aIID.equals (Components.interfaces.nsISupports) && !aIID.equals (Components.interfaces.nsIObserver))
      throw Components.result.NS_ERROR_NO_INTERFACE; 
   return this;
 }
};

function fdm_brCacheRegisterObserver ()
{
  var observerService = Components.classes["@mozilla.org/observer-service;1"]
              .getService(Components.interfaces.nsIObserverService);
  observerService.addObserver(fdm_brCacheListener, "http-on-modify-request", false);
  observerService.addObserver(fdm_brCacheListener, "http-on-examine-cached-response", false);
}

fdm_brCacheRegisterObserver ();
