// requires RequestsManager

function CsRequestsManager (name)
{
	RequestsManager.call (this);
	this.name = name;
	
	this.sendRequest = function (req)
	{
		self.port.emit (this.name, req);
	};
	
	self.port.on (this.name, 
		this.onRequestResponse.bind (this));
}


function CsUniformRequest (name, request)
{
	this.name = name;
	this.request = request;
}

function CsUniformRequestsManager ()
{
	this.managers = new Map;
	
	this.addManager = function (manager)
	{
		this.managers.set (manager.name, manager);
	};
	
	this.performRequest = function (unifReq, callback)
	{
		this.managers.get (unifReq.name).performRequest (
			unifReq.request, callback);
	};
}