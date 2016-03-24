const {Cc,Ci,Cu} = require("chrome");
Cu.import("resource://gre/modules/Services.jsm");
const ResProtocolHandler = Services.io.getProtocolHandler("resource").
                           QueryInterface(Ci.nsIResProtocolHandler);
const ChromeRegistry = Cc["@mozilla.org/chrome/chrome-registry;1"].
                       getService(Ci.nsIChromeRegistry);					   
const self = require("sdk/self");
const fileIO = require("sdk/io/file");
const System = require("./system.js");
const Constants = require("./constants.js");
const FileUtils = Cu.import("resource://gre/modules/FileUtils.jsm").FileUtils;
Cu.import("resource://gre/modules/osfile.jsm");

// http://stackoverflow.com/questions/19382201/how-to-load-dll-from-sdk-addon-data-folder
function resolveToFile(uri) 
{
  switch (uri.scheme) {
    case "chrome":
      return resolveToFile(ChromeRegistry.convertChromeURL(uri));
    case "resource":
      return resolveToFile(Services.io.newURI(ResProtocolHandler.resolveURI(uri), null, null));
    case "file":
      return uri.QueryInterface(Ci.nsIFileURL).file;
    default:
      throw new Error("Cannot resolve");
  }
}

function FdmAppInfo ()
{
	try {
		this.regKey = Cc ["@mozilla.org/windows-registry-key;1"].createInstance (Ci.nsIWindowsRegKey);
		this.regKey.open (this.regKey.ROOT_KEY_CURRENT_USER,
			"Software\\FreeDownloadManager.ORG\\Free Download Manager",
			this.regKey.ACCESS_READ);
		this.install_path = this.regKey.readStringValue ("Path");
	} catch (e) {}
	
	try { 
		this.regKeyFfExt = Cc ["@mozilla.org/windows-registry-key;1"].createInstance (Ci.nsIWindowsRegKey);
		this.regKeyFfExt.open (this.regKey.ROOT_KEY_CURRENT_USER,
			"Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\Update\\FirefoxExt",
			this.regKey.ACCESS_READ);
	} catch (e) {}
	
	this.findBrowserHelperExe ();
}

FdmAppInfo.prototype.getDbValue = function (key)
{
	var path = System.appDataPathAsIFile();
	path.append(Constants.AppDbName);
	if (!path.exists())
	{
		console.error('could not find DB, path: ', path.path);
		return undefined;
	}

    var dbService = Cc["@mozilla.org/storage/service;1"].getService(Ci.mozIStorageService);
    var dbConn = dbService.openDatabase(path);

    var it = dbConn.createStatement('SELECT value FROM settings WHERE key = "' + key + '";');
    var helperPath;
    if (!it.executeStep())
	{
		console.error('Cannot get value requested');
		return undefined;
	}
    
	var result = it.getString(0);
            
    it.reset();
	
	return result;
}

FdmAppInfo.prototype.findBrowserHelperExe = function ()
{
	var path = "";
	
	try { // FDM5
		path = this.getDbValue(Constants.BrowserHelperPathKey);	
		if (path)
		{
			path = OS.Path.normalize(path);
			if (fileIO.exists (path))
			{
				this.brhlpr_path = path;
				return;
			}
		}
	}catch(e){}
	
	// FDM3
	
	const exe_name = "FdmBrowserHelper.exe";
	
	try {
		path = self.data.url (exe_name);
		path = resolveToFile (Services.io.newURI (path, null, null));
		if (fileIO.exists (path))
		{
			this.brhlpr_path = path;
			return;
		}
	}catch (e){}
	
	try {
		path = this.regKeyFfExt.readStringValue ("Path");
		path += "\\" + exe_name;
		if (fileIO.exists (path))
		{
			this.brhlpr_path = path;
			return;
		}
	}catch (e){}
	
	try {
		path = this.regKey.readStringValue ("BhPath");
		if (fileIO.exists (path))
		{
			this.brhlpr_path = path;
			return;
		}
	}catch (e){}
		
	path = this.install_path + "\\" + exe_name;
	this.brhlpr_path = path;
}


exports.FdmAppInfo = FdmAppInfo;