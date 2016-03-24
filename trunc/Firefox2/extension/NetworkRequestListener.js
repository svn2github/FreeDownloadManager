const {Cc,Ci,Cu,Cr} = require("chrome");

function NetworkRequestListener (url)
{
	this.originalListener = null;
	this.url = url;
}

NetworkRequestListener.prototype.dontExtractData = true;
NetworkRequestListener.prototype.dontCallOriginalListener = false;

NetworkRequestListener.prototype.onStartRequest = 
	function (request, context)
{
	try
	{
       	this.originalListener.onStartRequest (request, context);
    }
	catch(e)
	{
		this.dontCallOriginalListener = true;
	}
}

NetworkRequestListener.prototype.onDataAvailable = 
	function (request, context, inputStream, offset, count)
{
	if (!this.dontExtractData)
	{
		var binaryInputStream = Cc ["@mozilla.org/binaryinputstream;1"].
			createInstance (Ci.nsIBinaryInputStream);
		var storageStream = Cc ["@mozilla.org/storagestream;1"].
			createInstance (Ci.nsIStorageStream);
		var binaryOutputStream = Cc ["@mozilla.org/binaryoutputstream;1"].
			createInstance (Ci.nsIBinaryOutputStream);

		binaryInputStream.setInputStream(inputStream);
		storageStream.init(8192, count, null);
		binaryOutputStream.setOutputStream(storageStream.getOutputStream(0));

		// Copy received data as they come.
		var data = binaryInputStream.readByteArray (count);
        
		this.onNetworkRequestDataReceived (count, data);

		binaryOutputStream.writeByteArray(data, count);

		if (!this.dontCallOriginalListener)
		{
			this.originalListener.onDataAvailable (request, context,
				storageStream.newInputStream(0), offset, count);
		}
	}
	else
	{
		try 
		{
			this.onNetworkRequestDataReceived ();
			if (!this.dontCallOriginalListener)
				this.originalListener.onDataAvailable (request, context, inputStream, offset, count);
		}
		catch(e)
		{
			this.dontCallOriginalListener = true;
		}
	}
}

NetworkRequestListener.prototype.onStopRequest = 
	function (request, context, statusCode)
{
	this.onNetworkRequestClosed ();
    this.originalListener.onStopRequest(request, context, statusCode);
}

NetworkRequestListener.prototype.QueryInterface = 
	function (aIID) 
{
	if (aIID.equals(Ci.nsIStreamListener) || 
		aIID.equals(Ci.nsISupports))
	{
		return this;
	}
	throw Cr.NS_NOINTERFACE;
}

NetworkRequestListener.prototype.onNetworkRequestDataReceived = 
	function (count, data)
{
}

NetworkRequestListener.prototype.onNetworkRequestClosed = 
	function ()
{
}


exports.NetworkRequestListener = NetworkRequestListener;