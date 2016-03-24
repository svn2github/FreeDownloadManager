const {Cc,Ci,Cu} = require("chrome");
var { atob, btoa } = Cu.import("resource://gre/modules/Services.jsm", {});

function to_utf8(s) 
{
  return unescape(encodeURIComponent(s));
}

function from_utf8(s) 
{
  return decodeURIComponent(escape(s));
}

function to_base64 (s)
{
	return btoa (s);
}

function from_base64 (s)
{
	return atob (s);
}


exports.to_utf8 = to_utf8;
exports.from_utf8 = from_utf8;
exports.to_base64 = to_base64;
exports.from_base64 = from_base64;