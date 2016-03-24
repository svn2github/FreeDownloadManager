var sdkEmit = require('sdk/event/core').emit;
var encodings = require("./encodings.js");

function SimpleIpcItem ()
{
  this.content = "";
}

function SimpleIpcItemRcvr ()
{
  this.clear = function () {
	this.item = new SimpleIpcItem ();
	this.expectingLength = 0;
	this.complete = false;	
  };
  
  this.clear ();
  
  this.useUriDecode = false;
  this.useBase64 = true;
  
  this.onDataRcvd = function (data) 
  {
	if (this.complete)
	  throw "unexpected";
    if (!this.expectingLength)
	  data = this.extractExpectingLength (data);
	if (!this.expectingLength)
	{
	  this.complete = true;
	  return data;
	}
	var lenReq = this.expectingLength - this.item.content.length;
	if (lenReq > data.length)
	{
	  this.item.content += data;
	  return "";
	}
	this.item.content += data.substring (0, lenReq);
	if (this.useBase64)
		this.item.content = encodings.from_utf8 (encodings.from_base64 (this.item.content));
	else if (this.useUriDecode)
		this.item.content = decodeURIComponent (this.item.content);
	this.complete = true;
	data = data.slice (lenReq);
	if ((data.length == 1 && data == "\n") ||
		(data.length == 2 && data == "\r\n"))
	{
	  data = "";
	}
	return data;
  };
  
  this.extractExpectingLength = function (data)
  {
    /*this.expectingLength = 
		(data.charCodeAt(0)) + 
		(data.charCodeAt(1) << 8) +
		(data.charCodeAt(2) << 16) +
		(data.charCodeAt(3) << 24);
	return data.slice (4);*/
	var len = "";
	for (var i = 0; i < data.length; i++)
	{
		if (data [i] == ' ')
		{
			this.expectingLength = parseInt (len);
			return data.slice (i+1);
		}
		len += data [i];
	}
	throw "input format error";
  };
}

function IpcItemLenToString (len)
{
  return String.fromCharCode (
	(len & 0xFF),
	(len >> 8) & 0xFF,
	(len >> 16) & 0xFF,
	(len >> 24) & 0xFF
  );
}

function SimpleIpcItemSender ()
{
	this.useUriEncode = false;
	this.useBase64 = true;
	
	this.sendItem = function (out, item)
	{
		var content;
		if (this.useBase64)
			content = encodings.to_base64 (encodings.to_utf8 (item.content));
		else if (this.useUriEncode)
			content = encodeURIComponent (item.content);
		else
			content = item.content;
		sdkEmit (out, 'data', IpcItemLenToString (content.length));
		sdkEmit (out, 'data', content);
	}
}

function SimpleIpcManager (stdin, stdout)
{
  this.resetState = function (stdin, stdout)
  {
	this.stdin = stdin;
	this.stdout = stdout;
	this.itemRcvr = new SimpleIpcItemRcvr;
	this.itemSender = new SimpleIpcItemSender;
	this.stdout.on ('data', this.onDataRcvd.bind(this));  
  };
  
  this.onItemRcvd = function () {};
  
  this.onDataRcvd = function (data) 
  {
	data = this.itemRcvr.onDataRcvd (data);
	if (this.itemRcvr.complete)
	{
		var item = this.itemRcvr.item;
		this.itemRcvr.clear ();
		if (data)
			this.onDataRcvd (data);
		this.onItemRcvd (item);		
	}
  };
  
  this.sendItem = function (item)
  {
	return this.itemSender.sendItem (this.stdin, item);
  };
  
  this.resetState (stdin, stdout);
}


exports.SimpleIpcItem = SimpleIpcItem;
exports.SimpleIpcManager = SimpleIpcManager;