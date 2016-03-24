// requires CsUniformRequestsManager

const FLVSNIFF_REQUEST_NAME = "sniff_dll_request";
const EXTENSION_REQUEST_NAME = "extension_request";

function FdmCsFlvSniffRequest (req)
{
	CsUniformRequest.call (this, 
		FLVSNIFF_REQUEST_NAME, req);
}

function FdmSniffDllIsVideoFlashRequest (
	webPageUrl, frameUrl, swfUrl, 
	flashVars, otherSwfUrls, otherFlashVars)
{
	var req = new Object;
	req.name = "IsVideoFlash";
	req.webPageUrl = webPageUrl;
	req.frameUrl = frameUrl;
	req.swfUrl = swfUrl;
	req.flashVars = flashVars;
	req.otherSwfUrls = otherSwfUrls;
	req.otherFlashVars = otherFlashVars;
	FdmCsFlvSniffRequest.call (this, req);
}

function FdmSniffDllCreateVideoDownloadFromUrlRequest (
	webPageUrl, frameUrl, swfUrl, 
	flashVars, otherSwfUrls, otherFlashVars)
{
	var req = new Object;
	req.name = "CreateVideoDownloadFromUrl";
	req.webPageUrl = webPageUrl;
	req.frameUrl = frameUrl;
	req.swfUrl = swfUrl;
	req.flashVars = flashVars;
	req.otherSwfUrls = otherSwfUrls;
	req.otherFlashVars = otherFlashVars;
	FdmCsFlvSniffRequest.call (this, req);
}

function FdmExtensionRequest (req)
{
	CsUniformRequest.call (this, 
		EXTENSION_REQUEST_NAME, req);
}

function FdmExtensionUiStringsRequest ()
{
	var req = new Object;
	req.type = "ui_strings";
	FdmExtensionRequest.call (this, req);
}

function FdmCsRequestsManager ()
{
	CsUniformRequestsManager.call (this);
	
	this.addManager (new CsRequestsManager (FLVSNIFF_REQUEST_NAME));
	this.addManager (new CsRequestsManager (EXTENSION_REQUEST_NAME));
}