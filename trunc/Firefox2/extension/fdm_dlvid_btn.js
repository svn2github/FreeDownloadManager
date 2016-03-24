var pageMod = require("sdk/page-mod");

function FdmDownloadVideoButton (csReqProcessor)
{
	var pageModInfo = 
	{
		include: "*",
		contentStyleFile: "./fdm_tabs.css",
		contentScriptFile:
			["./dom_util.js", "./fdm_util.js", "./RequestsManager.js", 
			"./CsRequestsManager.js", "./FdmCsRequestsManager.js",
			"./dom_elem_btn.js", "./fdm_dom_elem_btn.js",
			"./fdm_dlvid_btn.js"],
		onAttach: function(worker) 
		{
			csReqProcessor.attachToPort (worker.port);
		}
	};
	
	pageMod.PageMod (pageModInfo);
}


exports.FdmDownloadVideoButton = FdmDownloadVideoButton;