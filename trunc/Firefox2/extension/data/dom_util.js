function getElementParam (elem, paramName)
{
	var vars;

	try {
		vars = elem.getAttribute (paramName);
	} catch (e) {}

	if ((vars == null || vars == "") && elem.hasChildNodes ())
	{
		try 
		{
			for (var i = 0; i < elem.childNodes.length; i++) 
			{
				try{
					if (elem.childNodes[i].getAttribute("name").toLowerCase() == paramName)
					{
						vars = elem.childNodes[i].getAttribute("value");
						break;
					}
				}catch(e){}
			}
		}
		catch(e){}
	}
			
	if ((vars == null || vars == "") && elem.parentNode && elem.parentNode.hasChildNodes ())
	{
		try 
		{
			for (var i = 0; i < elem.parentNode.childNodes.length; i++) 
			{
				try{
					if (elem.parentNode.childNodes[i].getAttribute("name").toLowerCase() == paramName)
					{
						vars = elem.parentNode.childNodes[i].getAttribute("value");
						break;
					}
				}
				catch(e){}
			}
		}
		catch(e){}
	}

	return vars ? vars : "";
}
		
function getFlashElementSwfUrl (elem)
{
	var swfSrc;
	try {swfSrc = elem.data;}catch (e){}
	if (swfSrc == null || swfSrc == "")
	{
		try {swfSrc = elem.src;}catch (e){}
	}
	if (swfSrc == null || swfSrc == "")
		swfSrc = getElementParam (elem, "src");
	if (swfSrc == null || swfSrc == "")
		swfSrc = getElementParam (elem, "movie");
	return swfSrc;
}
		
function isFlashObject (elem)
{
	return /x-shockwave-flash/i.test (getElementParam (elem, "type")) ||
		getFlashElementSwfUrl (elem) != "";
}

function isYouTubeHTML5Video (elem)
{
	return elem && 
		(elem.id == 'movie_player' ||
			elem.parentNode && elem.parentNode.parentNode && elem.parentNode.parentNode.id == 'movie_player');
}

function getOtherSwfUrls (elem)
{
	var obj = new Object;
	obj.OtherSwfUrls = "";
	obj.OtherFlashVars = "";
	
	function getOtherSwfUrls_2 (coll, elemExcept)
	{
		for (var i = 0; i < coll.length; i++)
		{
			var el = coll [i];
			if (!el)
				continue;
			if (el != elemExcept && isFlashObject (el))
			{
				obj.OtherSwfUrls += getFlashElementSwfUrl (el);
				obj.OtherSwfUrls += "\n";
				obj.OtherFlashVars += getElementParam (el, "flashvars");
				obj.OtherFlashVars += "\n";
			}
		}
	}

	function getOtherSwfUrls_impl (doc, elem)
	{
		getOtherSwfUrls_2 (doc.embeds, elem);
		getOtherSwfUrls_2 (doc.getElementsByTagName ("object"), elem);
		var frames = doc.defaultView.frames;
		for (var i = 0; i < frames.length; i++)
		{
			try {
				getOtherSwfUrls_impl (frames [i].document, elem);
			}
			catch (e) {}
		}
	}
	
	getOtherSwfUrls_impl (window.content.document, elem);
	
	return obj;
}