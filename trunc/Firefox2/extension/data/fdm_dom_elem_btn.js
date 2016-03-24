// requires dom_elem_btn.js
// requires dom_util.js
// requires FdmCsRequestsManager.js

/*
 * We took some parts of code from Adblock Plus.
 *
 * The Initial Developer of the Original Code is
 * Wladimir Palant.
 * Portions created by the Initial Developer are Copyright (C) 2006-2010
 * the Initial Developer. All Rights Reserved.

 The functions we took are: ProcessCSSData, LoadTabCss,
  onTabClick, ShowTab, HideTab, positionTab, intersectRect,
  getElementPosition. We did minor changes in some of these functions.
 *
 *
*/

function FdmDomElementButton (target, buttonText)
{
	DomElementButton.call (this, target);
	
	this.onClickEvent = function (ev)
	{
		if (!ev.isTrusted)
			return;
		if (ev.type != "click" || ev.button != 0)
			return;
		ev.preventDefault ();
		ev.stopPropagation ();
		this.onButtonClick ();
	};
	
	var onClickEventBinded = this.onClickEvent.bind (this);
	
	this.show = function (sh)
	{		
		if (sh)
		{
			if (this.button)
				return;
				
			var doc = this.target.ownerDocument.defaultView.top.document;
			
			this.button = doc.createElementNS ("http://www.w3.org/1999/xhtml", "a");
			this.button.setAttribute ("href", "");
			this.button.setAttribute ("class", "fdm_ffext_CLASSVISIBLETOP");
			this.button.style.setProperty ("opacity", "1", "important");
			this.button.addEventListener ("click", onClickEventBinded, true);
		
			var el2 = doc.createElementNS ("http://www.w3.org/1999/xhtml", "img");
			el2.setAttribute ("src", "data:image/png,%89PNG%0D%0A%1A%0A%00%00%00%0DIHDR%00%00%00%10%00%00%00%10%08%02%00%00%00%90%91h6%00%00%00%01sRGB%00%AE%CE%1C%E9%00%00%00%04gAMA%00%00%B1%8F%0B%FCa%05%00%00%00%20cHRM%00%00z%26%00%00%80%84%00%00%FA%00%00%00%80%E8%00%00u0%00%00%EA%60%00%00%3A%98%00%00%17p%9C%BAQ%3C%00%00%01%01IDAT8O%8D%911%0E%C20%0CE%CD-8%40N%C0%CC%8AT%0EQ1%23%85%0B0%B1%C0%DC%BDB%EA%80%188%00%20%B8%01C%87n%9C%82%AEL%E6%3BN%D3%40%D5%0A%CB%AA%12%E7%3F%7F%A7%1913E1%DF%D1%F5%7DG%C1%BCffL%97u%7C%E8%D6%004%92-%9B%15%DB%92-%CB%D7%9C%99N5*%A8%C7%E1%01%CA'%C7G-%D2%90%3D%8C%00%B4!Q%BB%DE%9B%C1p%F3%E7%13%00%A2.E%5DUUQ%1C%BE%86p%1B%14qd%AD%05%40%01%C0%3E%00P%84%EC%05h%99%FF%E7%F0%F4%26%01%40oJ%09%19%FB%B4%23MoD%8EA%A2%AA%01u%C6%19%BE%A1%E2%01%B9xN%9Ei0%81%1D%B0g%1B3r%E9%00(%93p%82%84N%13k1o%7C%3C%A0O%01%A0%9B%A2~%92a%A3L%0B%A8%8Fb%AD(R%EB%ED%BF%00%7D%2C%C5%F4%17%85%DE%8B%26D%D0%7DTO%A6~%92%1F%C1%10%00%93n%BB%0Fg%7D%C9%E5%DA%24_%06%00%00%00%00IEND%AEB%60%82");
			el2.style.setProperty ("float", "left", "important");
			el2.style.setProperty ("padding-right", "3px", "important");
			el2.setAttribute ("align", "absmiddle");
			el2.setAttribute ("border", "0");
			el2.setAttribute ("hspace", "0");
			el2.setAttribute ("vspace", "0");
		
			var el3 = doc.createElementNS ("http://www.w3.org/1999/xhtml", "span");
			el3.textContent = buttonText;
			el3.setAttribute ("class", "");
			el3.setAttribute ("style", "");
		
			this.button.appendChild (el2);  
			this.button.appendChild (el3);
		
			doc.documentElement.appendChild (this.button);
		
			this.positionButtonByTimer ();
		}
		else
		{
			if (!this.button)
				return;
			this.button.removeEventListener ("click", onClickEventBinded, true);
			this.button.parentNode.removeChild (this.button);
			this.button = null;
		}
	};
	
	this.onButtonClick = function ()
	{
		throw "pure function call";
	};
	
	this.positionButtonByTimer = function ()
	{
		if (!this.button)
			return;
		this.positionButton ();
		if (this.button)
			setTimeout (this.positionButtonByTimer.bind (this), 100);
	};
	
	this.positionButton = function ()
	{
		var objRect = this.getElementPosition (this.target); 
		var className = "fdm_ffext_CLASSVISIBLETOP";
		var left = objRect.left;
		var top = objRect.top - this.button.offsetHeight;
		if (top < 0)
		{
			top = objRect.bottom;
			className = "fdm_ffext_CLASSVISIBLEBOTTOM";
		}
  
		if (this.button.style.left != left + "px")
			this.button.style.setProperty ("left", left + "px", "important");
		if (this.button.style.top != top + "px")
			this.button.style.setProperty ("top", top + "px", "important");
		if (this.button.getAttribute ("class") != className)
			this.button.setAttribute ("class", className);
	};
	
	this.getElementPosition = function (elem)
	{
		var rect = elem.getBoundingClientRect ();
		let wnd = elem.ownerDocument.defaultView;
		var offsets = [0, 0, 0, 0];
		var style = wnd.getComputedStyle (elem, null);
		offsets [0] = parseFloat (style.borderLeftWidth) + parseFloat (style.paddingLeft);
		offsets [1] = parseFloat (style.borderTopWidth) + parseFloat (style.paddingTop);
		offsets [2] = parseFloat (style.borderRightWidth) + parseFloat (style.paddingRight);
		offsets [3] = parseFloat (style.borderBottomWidth) + parseFloat (style.paddingBottom);
		rect = {left: rect.left + offsets [0], top: rect.top + offsets [1], 
			right: rect.right - offsets [2], bottom: rect.bottom - offsets [3]};
		while (true)
		{
			this.intersectRect (rect, wnd);
			if (!wnd.frameElement)
				break;
			var frameElem = wnd.frameElement;
			wnd = frameElem.ownerDocument.defaultView;
			var frameRect = frameElem.getBoundingClientRect ();
			var frameStyle = wnd.getComputedStyle (frameElem, null);
			var relLeft = frameRect.left + parseFloat (frameStyle.borderLeftWidth) + parseFloat (frameStyle.paddingLeft);
			var relTop = frameRect.top + parseFloat (frameStyle.borderTopWidth) + parseFloat (frameStyle.paddingTop);
			rect.left += relLeft;
			rect.right += relLeft;
			rect.top += relTop;
			rect.bottom += relTop;
		}
		return rect;
	};
	
	this.intersectRect = function (rect, wnd)
	{
		var doc = wnd.document;
		var wndWidth = doc.documentElement.clientWidth;
		var wndHeight = doc.documentElement.clientHeight;
		if (doc.compatMode == "BackCompat")
			wndHeight = doc.documentElement.offsetHeight - wnd.scrollMaxY;
		rect.left = Math.max (rect.left, 0);
		rect.top = Math.max (rect.top, 0);
		rect.right = Math.min (rect.right, wndWidth);
		rect.bottom = Math.min (rect.bottom, wndHeight);
	};
}


function FdmDomElementButtonsManager (window)
{
	DomElementButtonsManager.call (this, window);
}


function FdmDomElementVideoButtonsManager (window, csReqMgr)
{
	FdmDomElementButtonsManager.call (this, window);
	this.csReqMgr = csReqMgr;
	this.uiStrings = null;
	
	csReqMgr.performRequest (
		new FdmExtensionUiStringsRequest,
		function (resp)
		{
			this.uiStrings = resp.uiStrings;
		}.bind (this));
	
	this.createButton = function (target)
	{
		var button = new FdmDomElementButton (
			target, this.uiStrings.dlbyfdm);
		
		button.onButtonClick = function ()
		{
			button.show (false);
			var params = this.getSniffDllParams (target);
			var req = new FdmSniffDllCreateVideoDownloadFromUrlRequest (
				params.webPageUrl, params.frameUrl, 
				params.swfSrc, params.flashVars, params.objOtherSwfUrls.OtherSwfUrls, 
				params.objOtherSwfUrls.OtherFlashVars);
			this.csReqMgr.performRequest (req, function (resp)
			{
				var resultMsg = VideoSnifferRequestResultToString (
					resp.videoSniffer, this.uiStrings);
				if (resultMsg)
					alert (resultMsg);
			}.bind (this));
		}.bind (this);
		
		return button;
	};
	
	this.queryElementType = function (elem, callback)
	{
		function notTargetTag (elem)
		{
			return elem.tagName &&
				elem.tagName.toLowerCase() != "embed" && 
				elem.tagName.toLowerCase() != "object" && 
				elem.tagName.toLowerCase() != "video";
		}
		
		if (notTargetTag (elem) ||
			!elem.parentNode ||
			!isFlashObject (elem))
		{
			callback (false);
			return;
		}
		
		if (isYouTubeHTML5Video (elem))
		{
			callback (true, elem.parentNode.parentNode);
			return;
		}
		
		this.queryElementTypeFromFlvSniffer (elem, callback);
	};
	
	this.queryElementTypeFromFlvSniffer = function (elem, callback)
	{
		var params = this.getSniffDllParams (elem);
		var req = new FdmSniffDllIsVideoFlashRequest (params.webPageUrl, params.frameUrl, 
			params.swfSrc, params.flashVars, params.objOtherSwfUrls.OtherSwfUrls, 
			params.objOtherSwfUrls.OtherFlashVars);
		this.csReqMgr.performRequest (req, function (resp)
		{
			var isTarget = resp.videoSniffer.result && resp.videoSniffer.result != "0";
			callback (isTarget);
		});
	};
	
	this.getSniffDllParams = function (elem)
	{
		var obj = new Object;
		obj.swfSrc = getFlashElementSwfUrl (elem);
		obj.flashVars = getElementParam (elem, "flashvars");
		obj.frameUrl = elem.ownerDocument.location.href;
		obj.webPageUrl = this.window.document.location.href;
		if (obj.frameUrl == obj.webPageUrl)
			obj.frameUrl = "";
		obj.objOtherSwfUrls = getOtherSwfUrls (elem);
		return obj;
	};
}
