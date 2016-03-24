function RequestsManager ()
{
	this.nextReqId = 1;
	this.requestsInProgress = new Map;
	
	this.assignRequestId = function (req)
	{
		req.id = this.nextReqId.toString ();
		this.nextReqId++;
	};
	
	this.performRequest = function (req, callback)
	{
		if (!req.id || req.id == "0")
			this.assignRequestId (req);
			
		if (callback)
			this.requestsInProgress.set (req.id, callback);
			
		this.sendRequest (req);
	};
	
	this.sendRequest = function (req)
	{
		throw "pure function call";
	};
	
	this.onRequestResponse = function (resp)
	{
		var callback = this.requestsInProgress.get (resp.id);
		this.requestsInProgress.delete (resp.id);
		if (callback)
			callback (resp);
	};
	
	this.closeRequestsInProgress = function (callback)
	{
		this.requestsInProgress.forEach (function (val, key, map)
		{
			map.delete (key);
			if (callback)
				callback (key, val);
		});
	};
}



try
{
	exports.RequestsManager = RequestsManager;
}
catch (e) {}