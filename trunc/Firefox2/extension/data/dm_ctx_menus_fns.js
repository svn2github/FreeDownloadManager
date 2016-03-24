function extractNodeUrl (node)
{  
  if (node instanceof HTMLImageElement &&
	  	!((node.parentNode instanceof HTMLAnchorElement) || (node.parentNode instanceof HTMLAreaElement)))
  	return node.src;  

  if ((node instanceof HTMLAnchorElement) || (node instanceof HTMLAreaElement))
  	return node.href;

  return node.parentNode ? extractNodeUrl (node.parentNode) : null;
}

function gatherElements (collection, bSelectedOnly)
{
  var result = [];
  const selection = window.getSelection();
  if (bSelectedOnly && selection.isCollapsed)
  	return;
  for (var i = 0; i < collection.length; i++)
  {
    const elem = collection.item (i);
    if (bSelectedOnly == false || selection.containsNode (elem, true))
    {
        var url = extractNodeUrl (elem);
		if (url && url.length)
			result.push (url);
    }
  }
  return result;
}

function gatherAllElements (bSelectedOnly)
{
	var urls = gatherElements (document.links, bSelectedOnly);
	var urls2 = gatherElements (document.images, bSelectedOnly);
	Array.prototype.push.apply (urls, urls2);
	return urls;
}

function DlLinkContextMenuDomProcessor (node)
{
	var r = new Object;
	r.urls = new Array;
	r.urls.push (extractNodeUrl (node));
	r.pageUrl = window.location.href;
	r.userAgent = window.navigator.userAgent;
	return r;
}

function DlAllContextMenuDomProcessor (node)
{
	var r = new Object;
	r.urls = gatherAllElements (false);
	r.pageUrl = window.location.href;
	r.userAgent = window.navigator.userAgent;
	return r;
}

function DlSelectedContextMenuDomProcessor (node)
{
	var r = new Object;
	r.urls = gatherAllElements (true);
	r.pageUrl = window.location.href;
	r.userAgent = window.navigator.userAgent;
	return r;
}

function DlVideoContextMenuDomProcessor (node)
{
	var r = new Object;
	r.webPageUrl = window.location.href;
	return r;
}

function ctxmenuOnClick (resultFn)
{
	self.on ("click", function (CtxMenuNode, CtxMenuData) 
	{
		var result = resultFn (CtxMenuNode, CtxMenuData);
        self.postMessage (result);
	});
}