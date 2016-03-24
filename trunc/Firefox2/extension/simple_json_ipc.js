var {SimpleIpcItem, SimpleIpcManager} = require ("./simple_ipc.js");

function SimpleJsonIpcItem (json)
{
	SimpleIpcItem.call (this);
	
	if (typeof (json) == "string")
		this.object = JSON.parse (json);
	else
		this.content = JSON.stringify (json);
}
//SimpleJsonIpcItem.prototype = new SimpleIpcItem;

function SimpleJsonIpcManager (stdin, stdout)
{
	SimpleIpcManager.call (this, stdin, stdout);

	this.onItemRcvd = function (item)
	{
		item = new SimpleJsonIpcItem (item.content);
		this.onJsonItemRcvd (item);
	};
	
	this.onJsonItemRcvd = function () {};
}
//SimpleJsonIpcManager.prototype = SimpleIpcManager.prototype;


exports.SimpleJsonIpcItem = SimpleJsonIpcItem;
exports.SimpleJsonIpcManager = SimpleJsonIpcManager;
