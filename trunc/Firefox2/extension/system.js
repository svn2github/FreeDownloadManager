const {Cc, Cu, Ci} = require("chrome");
const fileIO = require("sdk/io/file");
const Constants = require("./constants.js");
Cu.import("resource://gre/modules/osfile.jsm");
Cu.import("resource://gre/modules/ctypes.jsm");
var {setTimeout} = require ("sdk/timers");

exports.OSName = function()
{
    var xulRuntime = Cc["@mozilla.org/xre/app-info;1"].getService(Ci.nsIXULRuntime);
    return xulRuntime.OS;
};

exports.fileExists = function(path)
{
    try {
        return fileIO.exists(path); // why they can't just return false?..
    } catch (e) {
        return false;
    }
};

exports.appDataPath = function()
{
    var path = exports.appDataPathAsIFile();
	return path ? path.path : "";
};

exports.appDataPathAsIFile = function()
{
	var path = null;
	var os = exports.OSName();
    if (os == 'Darwin')
	{
        path = Cc["@mozilla.org/file/directory_service;1"].getService(Ci.nsIProperties).get('Home', Ci.nsIFile);
		path.append('Library');
		path.append('Application Support');
	}
	else if (os == 'WINNT')
	{
		path = Cc ["@mozilla.org/file/directory_service;1"].getService(Ci.nsIProperties).get("LocalAppData", Ci.nsIFile);
	}
	else
	{
		console.error('appDataPathAsIFile TODO: OS ' + os);
	}
	if (path)
		path.append(Constants.AppName);
	return path;
}

// baseWindow must be Ci.nsIBaseWindow
exports.showWindow = function(baseWindow, show)
{
	exports.nativeShowWindow(
		baseWindow.nativeHandle, show);
}

exports.isWindowVisible = function(baseWindow)
{
	return exports.nativeIsWindowVisible(
		baseWindow.nativeHandle);
}

exports.nativeShowWindow = function(windowHandle, show)
{
	var os = exports.OSName();
    if (os == 'Darwin')
	{
		var objc = ctypes.open(ctypes.libraryName('objc'));
		// types
		let id = ctypes.voidptr_t;
		let SEL = ctypes.voidptr_t;
		// constants
		let nil = ctypes.voidptr_t(0);
		//common functions
		let sel_registerName = objc.declare('sel_registerName', ctypes.default_abi, SEL, ctypes.char.ptr);
		let objc_msgSend = objc.declare('objc_msgSend', ctypes.default_abi, id, id, SEL, '...');

		var NSWindowPtr = ctypes.voidptr_t(ctypes.UInt64(windowHandle));
		
		if (show)
		{
			var orderFront = sel_registerName('orderFront:');
			var rez_orderFront = objc_msgSend(NSWindowPtr, orderFront, nil);
		}
		else
		{
			var orderOut = sel_registerName('orderOut:');
			var rez_orderOut = objc_msgSend(NSWindowPtr, orderOut, nil);
		}
	}
	else
	{
		throw "unsupported OS";
	}
}

exports.nativeIsWindowVisible = function(windowHandle)
{
	var os = exports.OSName();
    if (os == 'Darwin')
	{
		var objc = ctypes.open(ctypes.libraryName('objc'));
		// types
		let id = ctypes.voidptr_t;
		let SEL = ctypes.voidptr_t;
		// constants
		let nil = ctypes.voidptr_t(0);
		//common functions
		let sel_registerName = objc.declare('sel_registerName', ctypes.default_abi, SEL, ctypes.char.ptr);
		let objc_msgSend = objc.declare('objc_msgSend', ctypes.default_abi, id, id, SEL, '...');

		var NSWindowPtr = ctypes.voidptr_t(ctypes.UInt64(windowHandle));
		
		var selIsVisible = sel_registerName('isVisible');
		return objc_msgSend(NSWindowPtr, selIsVisible, nil) != 0;
	}
	else
	{
		throw "unsupported OS";
	}
}

exports.showWindow2 = function(baseWindow, show, waitState)
{
	exports.showWindow2wa(baseWindow, show, waitState, 1500);
	/*if (!waitState || 
		exports.isWindowVisible(baseWindow) == show)
	{
		exports.showWindow2(baseWindow, show);
	}
	else
	{
		setTimeout(function() {
			exports.showWindow2(baseWindow, show, true);
		}, 100);
	}*/
}

exports.showWindow2wa = function(baseWindow, show, waitState, totalTimeMs)
{
	exports.showWindow(baseWindow, show);
	
	if (waitState && totalTimeMs >= 100)
	{
		setTimeout(function() {
			exports.showWindow2wa(baseWindow, show, true, totalTimeMs - 100);
		}, 100);
	}
}