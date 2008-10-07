var fdm_FDM = Components.classes["@freedownloadmanager.org/FDMForFirefox;1"].createInstance ();
fdm_FDM = fdm_FDM.QueryInterface (Components.interfaces.IFDMForFirefox);

var fdm_hoverElement = null;
var fdm_lastClickCaptureTime = 0;

function fdm_gatherCookieForHost (_host, _cookieWeHave)
{
   var fdm_cookieManager =
      Components.classes["@mozilla.org/cookiemanager;1"].getService(Components.interfaces.nsICookieManager);
   var _cookie = _cookieWeHave;

   for (var iter = fdm_cookieManager.enumerator; iter.hasMoreElements();) 
   {
      if ((objCookie = iter.getNext()) instanceof Components.interfaces.nsICookie) 
      {
        var cookieHost = objCookie.host;
        if (cookieHost.charAt(0) == ".")
          cookieHost = cookieHost.substring (1);

        if (cookieHost == _host || _host.indexOf ("." + cookieHost) != -1)
        {
	        if (_cookie.indexOf (objCookie.name + "=" + objCookie.value) == -1)
	        {
		        if (_cookie.length)
		        	_cookie += "; ";
	        	_cookie += objCookie.name + "=" + objCookie.value;
	        }
	}
      }
   }

   return _cookie;
}


function fdm_dlURL (strUrl)
{
  var url = Components.classes["@freedownloadmanager.org/FDMUrl;1"].createInstance ();
  url = url.QueryInterface (Components.interfaces.IFDMUrl);
  url.Url = strUrl;
  url.Referer = document.commandDispatcher.focusedWindow.document.URL;
  url.Cookies = fdm_gatherCookieForHost (document.commandDispatcher.focusedWindow.document.location.hostname, document.commandDispatcher.focusedWindow.document.cookie);

  var urlRcvr = Components.classes["@freedownloadmanager.org/FDMUrlReceiver;1"].createInstance ();
  urlRcvr = urlRcvr.QueryInterface (Components.interfaces.IFDMUrlReceiver);
  urlRcvr.SetUrl (url);
  urlRcvr.ShowAddDownloadDialog ();
}

function fdm_dlHoverElem()
{
  var strUrl = fdm_getURL (fdm_hoverElement);
  if (strUrl == null)
    return false;
  fdm_dlURL (strUrl);
  return true;
}

// remember the current hover element here
window.addEventListener("mouseover", function(ev) 
{
      fdm_hoverElement = ev.target;

}, false);

window.addEventListener("mouseup", function(ev) 
{
  if(ev.altKey && !(ev.ctrlKey || ev.metaKey || ev.shiftKey) && 
   	fdm_FDM.IsALTShouldBePressed()) 
  {
     if(fdm_dlHoverElem()) 
     {
       ev.preventDefault();
       ev.stopPropagation();
       fdm_lastClickCaptureTime = new Date().getTime();
       return;
     }
  }

}, false);

window.addEventListener("click",function(ev) 
{
      if(ev.altKey && new Date().getTime()-fdm_lastClickCaptureTime < 100) 
      {
        // catch click on a link
        ev.preventDefault();
        ev.stopPropagation();
      }

}, true);

var fdm_LastPost = null;

var fdm_Observer = { 
 observe: function (channel, topic, data) 
 {
   if(channel instanceof Components.interfaces.nsIUploadChannel) {
      var fdm_Ext = Components.classes["@freedownloadmanager.org/FDMFirefoxExtension;1"].createInstance ();
      fdm_Ext = fdm_Ext.QueryInterface (Components.interfaces.IFDMFirefoxExtension);
      fdm_LastPost = channel;
      fdm_Ext.LastPost = fdm_LastPost;
   }
 },

 QueryInterface: function (aIID) 
 {
   if (!aIID.equals (Components.interfaces.nsISupports) && !aIID.equals (Components.interfaces.nsIObserver))
      throw Components.result.NS_ERROR_NO_INTERFACE; 
   return this;
 }
};


const fdm_FFObsrv = Components.classes['@mozilla.org/observer-service;1'].getService(
    Components.interfaces.nsIObserverService);
fdm_FFObsrv.addObserver (fdm_Observer, "http-on-modify-request", false);

window.addEventListener("load", fdm_initOverlay, false);

function fdm_initOverlay ()
{
  var menu = document.getElementById("contentAreaContextMenu");
  menu.addEventListener("popupshowing", fdm_contextPopupShowing, false);
}

function fdm_checkHasVideo ()
{
  if (document.commandDispatcher.focusedWindow.document.domain == null)
  	return false;
  return fdm_FDM.IsDomainSupportedForVideoDownloads (document.commandDispatcher.focusedWindow.document.domain);
}

// handler. will be called by browser
function fdm_contextPopupShowing ()
{
  var menuitem = document.getElementById("fdm-context-item-dllink");
  menuitem.label  = fdm_FDM.GetLngString ("dllink");
  menuitem.hidden = fdm_FDM.IsContextMenuItemShouldBeHidden ("dllink") || 
  	!fdm_checkURL (fdm_getURL (document.popupNode));

  menuitem = document.getElementById("fdm-context-item-dlall");
  menuitem.label  = fdm_FDM.GetLngString ("dlall");
  menuitem.hidden = fdm_FDM.IsContextMenuItemShouldBeHidden ("dlall");

  menuitem = document.getElementById("fdm-context-item-dlselected");
  menuitem.label  = fdm_FDM.GetLngString ("dlselected");
  menuitem.hidden = fdm_FDM.IsContextMenuItemShouldBeHidden ("dlselected") ||
  	document.commandDispatcher.focusedWindow.getSelection().isCollapsed;

  menuitem = document.getElementById("fdm-context-item-dlvideo");
  menuitem.label  = fdm_FDM.GetLngString ("dlvideo");
  menuitem.hidden = fdm_FDM.IsContextMenuItemShouldBeHidden ("dlvideo") ||
  	fdm_checkHasVideo () == false;

  menuitem = document.getElementById("fdm-context-item-separator");
  menuitem.hidden = fdm_FDM.IsContextMenuItemShouldBeHidden ("separator1");
}

function fdm_getURL (node)
{
  if (node instanceof HTMLImageElement &&
	  	!((node.parentNode instanceof HTMLAnchorElement) || (node.parentNode instanceof HTMLAreaElement)))
  	return node.src;  

  if ((node instanceof HTMLAnchorElement) || (node instanceof HTMLAreaElement))
  	return node.href;

  return node.parentNode ? fdm_getURL (node.parentNode) : null;
}

function fdm_checkURL (url)
{
   return url ? true : false;
}

function fdm_dllink ()
{
  fdm_dlURL (fdm_getURL (document.popupNode));
}

function fdm_dlvideo ()
{
  /*fdm_FDM.ProcessVideoDocument (document.commandDispatcher.focusedWindow.document.domain, 
  	"", document.commandDispatcher.focusedWindow.document.documentElement.innerHTML);*/

  fdm_FDM.CreateVideoDownloadFromUrl (document.commandDispatcher.focusedWindow.document.URL);
}

// add urls of all or selected only elements of current web page to fdm
function fdm_gatherElements (url, urlListRcvr, elements, bSelectedOnly)
{
  const selection = document.commandDispatcher.focusedWindow.getSelection();
  if (bSelectedOnly && selection.isCollapsed)
  	return;
  for (var i = 0; i < elements.length; i++)
  {
    const elem = elements.item (i);
    if (bSelectedOnly == false || selection.containsNode (elem, true))
    {
        var strUrl = fdm_getURL (elem);;
        if (fdm_checkURL (strUrl))
        {
	  url.Url = strUrl;
	  urlListRcvr.AddUrl (url);
	}
    }
  }
}

function fdm_gatherAllElements (bSelectedOnly, cookie)
{
  var url = Components.classes["@freedownloadmanager.org/FDMUrl;1"].createInstance ();
  url = url.QueryInterface (Components.interfaces.IFDMUrl);
  url.Referer = document.commandDispatcher.focusedWindow.document.URL;
  url.Cookies = cookie;

  var urlListRcvr = Components.classes["@freedownloadmanager.org/FDMUrlListReceiver;1"].createInstance ();
  urlListRcvr = urlListRcvr.QueryInterface (Components.interfaces.IFDMUrlListReceiver);

  fdm_gatherElements (url, urlListRcvr, 
      document.commandDispatcher.focusedWindow.document.links, bSelectedOnly);

  fdm_gatherElements (url, urlListRcvr, 
      document.commandDispatcher.focusedWindow.document.images, bSelectedOnly);

  urlListRcvr.ShowAddUrlListDialog ();
}

function fdm_dlall ()
{
  fdm_gatherAllElements (false, fdm_gatherCookieForHost (document.commandDispatcher.focusedWindow.document.location.hostname, document.commandDispatcher.focusedWindow.document.cookie));
}

function fdm_dlselected ()
{
  fdm_gatherAllElements (true, fdm_gatherCookieForHost (document.commandDispatcher.focusedWindow.document.location.hostname, document.commandDispatcher.focusedWindow.document.cookie));
}

