const {Cc,Cu, Ci} = require("chrome");
//Cu.import ("resource://gre/modules/Services.jsm");

function getCookiesFromHost (host)
{
	// this code does not work due to some unknown reason:
	/*var cookieService = Services.cookies;
	return cookieService.getCookiesFromHost (host);*/
	
	var cookieManager = Cc ["@mozilla.org/cookiemanager;1"].getService(Ci.nsICookieManager);
	var result = "";

	for (var iter = cookieManager.enumerator; iter.hasMoreElements();) 
	{
		if ((objCookie = iter.getNext()) instanceof Ci.nsICookie) 
		{
			var cookieHost = objCookie.host;
			if (cookieHost.charAt(0) == ".")
				cookieHost = cookieHost.substring (1);

			if (cookieHost == host || host.indexOf ("." + cookieHost) != -1)
			{
				if (result.indexOf (objCookie.name + "=" + objCookie.value) == -1)
				{
					if (result.length)
						result += "; ";
					result += objCookie.name + "=" + objCookie.value;
				}
			}
		}
	}

	return result;
}


exports.getCookiesFromHost = getCookiesFromHost;