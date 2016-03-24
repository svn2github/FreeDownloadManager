var fdmbh = require ("./fdm_bh_ipc.js");

function CsRequestsProcessor ()
{
	this.processors = new Array;
	
	this.addProcessor = function (name, fn)
	{
		var obj = new Object;
		obj.name = name;
		obj.fn = fn;
		this.processors.push (obj);
	};
	
	this.attachToPort = function (port)
	{
		this.processors.forEach (function (processor)
		{
			port.on (processor.name, function (req)
			{
				processor.fn (req, function (resp)
				{
					port.emit (processor.name, resp);
				});
			});
		});
	};
}

function FdmCsRequestsProcessor (ipcmgr, settings, uiStrings)
{
	CsRequestsProcessor.call (this);
	
	this.ipcmgr = ipcmgr;
	this.settings = settings;
	this.uiStrings = uiStrings;
	
	function create_resp (req)
	{
		var resp = new Object;
		resp.id = req.id;
		return resp;
	}
	
	this.sniffDllRequestProcessor = function (req, callback)
	{
		var task = new fdmbh.FdmBhVideoSnifferTask;
		task.setRequest (req);
		this.ipcmgr.sendTask (task, function (resp)
		{
			var obj = create_resp (req);
			obj.error = resp.error;
			obj.videoSniffer = resp.videoSniffer;
			callback (obj);
		});
	};
	
	this.addProcessor ("sniff_dll_request", 
		this.sniffDllRequestProcessor.bind (this));
		
		
	this.extensionRequestProcessor = function (req, callback)
	{
		var resp = create_resp (req);
		if (req.type == "ui_strings")
			resp.uiStrings = this.uiStrings;
		callback (resp);
	};
	
	this.addProcessor ("extension_request", 
		this.extensionRequestProcessor.bind (this));
}


exports.FdmCsRequestsProcessor = FdmCsRequestsProcessor;