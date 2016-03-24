function VideoSnifferRequestResultToString (resp, uiStrings)
{
	var msg;
	
	if (!resp.error)
	{
		if (resp.result == "flv_not_found")
			msg = uiStrings.noFlashVideoFound;
	}
	else
	{
		if (resp.error == "sniffer_not_loaded")
			msg = uiStrings.sniffModuleNotLoaded;
			
		else if (resp.error == "url_not_found")
			msg = uiStrings.flvWebPageNotFound;
			
		else if (resp.error == "fdm_call_failed")
			msg = uiStrings.fdmCallFailed.replace ("%x", resp.error2);
		
		else
			msg = "Unknown error: " + resp.error;
	}
			
	return msg;
};


try
{
	exports.VideoSnifferRequestResultToString = VideoSnifferRequestResultToString;
}
catch (e) {}