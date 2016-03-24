const {Cc,Ci,Cu} = require("chrome");
var prefs = Cc["@mozilla.org/preferences;1"].getService (Ci.nsIPrefBranch);

function ProxyInfo ()
{
	this.query = function (protocol)
	{
		this.type = prefs.getIntPref ("network.proxy.type");
		this.protocol = protocol;
		this.addr = prefs.getCharPref ("network.proxy." + protocol);
		this.port = prefs.getIntPref ("network.proxy." + protocol + "_port");
	};
}

function ProxiesInfo ()
{
	this.query = function ()
	{
		this.proxies = new Array;
		var protocols = ["http", "ssl", "ftp"];
		protocols.forEach (function (protocol)
		{
			var proxy = new ProxyInfo;
			proxy.query (protocol);
			this.proxies.push (proxy);
		}.bind (this));
	}
}

function ProxyChangeObserver ()
{
	this.observe = function () {};

	this.register = function ()
	{
		prefs.addObserver ("network.proxy", this, false);
	};
	
	this.unregister = function ()
	{
		prefs.removeObserver ("network.proxy", this, false);
	};
}


exports.ProxyInfo = ProxyInfo;
exports.ProxiesInfo = ProxiesInfo;
exports.ProxyChangeObserver = ProxyChangeObserver;