/*
function toUTF(string)
{
    string = string.replace(/\r\n/g,"\n");
    var utftext = "";
    for (var n = 0; n < string.length; n++)
    {
        var c = string.charCodeAt(n);
        if (c < 128)
        {
            utftext += String.fromCharCode(c);
        }
        else if((c > 127) && (c < 2048))
		{
			var a = c.toString(16);
			if( a.length==2){
				a='00' + a;
			}
			else if(a.length==3){
				a='0'+ a;
			}
			utftext += '\\u'+a;
		}
    }
    return utftext;
}
*/
function readAllText(path) {

	var ado = new ActiveXObject('ADODB.Stream');
	ado.CharSet = "utf-8";
	ado.Open();
	ado.LoadFromFile( path );
	return ado.ReadText();
}

function writeAllText(path, text) {
	var ado = new ActiveXObject('ADODB.Stream');
	ado.CharSet = "utf-8";
	ado.Open();
	ado.WriteText( text );
	return ado.SaveToFile ( path , 2);
}

function removeUserExtensionFromPrefs(extensionId, profilePath)
{
	var prefsPath = getPrefsPath(profilePath);
	var prefs = JSON.parse(readAllText(prefsPath));
	
	if(typeof prefs.extensions.settings[extensionId] != 'undefined'){
		
		delete prefs.extensions.settings[extensionId];
		
		writeAllText(prefsPath, JSON.stringify(prefs));
	}

	var fso = new ActiveXObject('Scripting.FileSystemObject');
	var extensionPath = profilePath + "\\Extensions\\" + extensionId;
	if (fso.FolderExists(extensionPath))
		fso.DeleteFolder(extensionPath);
}

function removeUserExtension(extensionId, browser)
{
	// 
	var userDataPath = getBrowserUserDataPath(browser);
	var fso = new ActiveXObject('Scripting.FileSystemObject');
	var folder = fso.GetFolder(userDataPath);
	var subFlds = new Enumerator(folder.SubFolders);
	var subfolderName;

	for (; !subFlds.atEnd(); subFlds.moveNext()) {
		subfolderName = subFlds.item().Name;

		if (/^(Default|Profile\s\d+|Opera\sStable)$/.test(subfolderName))
		{
			removeUserExtensionFromPrefs(extensionId, userDataPath + "\\" + subfolderName);
		}
	}
}

function getEnvironmentVariables()
{
	var oShell = new ActiveXObject("WScript.Shell");
	var oUserEnv = oShell.Environment("Process");

	var colVars = new Enumerator(oUserEnv);
	var parts;
	var env = {};
	for(; ! colVars.atEnd(); colVars.moveNext())
	{
		parts = colVars.item().split('=');
		env[parts[0]] = parts[1];
	}

	return env;
}

function getBrowserUserDataPath(browser)
{
	var env = getEnvironmentVariables();
	if ("chrome" == browser)
		return env['LOCALAPPDATA'] + "\\Google\\Chrome\\User Data";
	else
		return env['APPDATA'] + "\\Opera Software";
}

function getExtensionsPath(profilePath) {
	return profilePath + '\\Extensions';
}

function getPrefsPath(profilePath) {
	return profilePath + '\\Preferences';
}

function getSecurePrefsPath(profilePath) {
	return profilePath + '\\Secure Preferences';
}

function getExtensionInfoPath(extensionPath) {
	return extensionPath + '\\extension_info.json';
}

function getManifestPath(extensionPath) {
	return extensionPath + '\\manifest.json';
}

function copyFolder(src, dst)
{
	var fso = new ActiveXObject("Scripting.FileSystemObject");
	var parts = dst.split("\\");
	var fld = parts[0];
	for (var i = 1, l = parts.length; i < l; ++i)
	{
		if (!fso.FolderExists(fld))
			fso.CreateFolder(fld);

		fld += "\\" + parts[i];
	}

	fso.CopyFolder(src, dst);
}

function addExtensionToProfile(profilePath, unpacked_path, key, info, manifest)
{
	var prefsPath = getPrefsPath(profilePath);
	var prefs = JSON.parse(readAllText(prefsPath));
	var extensionsFolderPath = profilePath + "\\Extensions";
	var extensionPath = extensionsFolderPath + "\\" + info.id + "\\" + info.version;
	var extension = {};

	copyFolder(unpacked_path, extensionPath);
	
	manifest['key'] = key;
	extension['manifest'] = manifest;
	if(typeof prefs['extensions']['settings'] == 'undefined') {
		prefs['extensions']['settings'] = {};
	}
	extension['path'] = info.id + '\\' + info.version;
	extension['state'] = 1;
	extension['incognito'] = false;
	extension['location'] = 1;
	extension['granted_permissions'] = {
	   "api": [ "tabs" ],
	   "full": false,
	   "host": [ "http://*/*", "https://*/*" ]
	};
	prefs['extensions']['settings'][info.id] = extension;

	//writeAllText(prefsPath, toUTF(JSON.stringify(prefs)));
	writeAllText(prefsPath, JSON.stringify(prefs) );
}

function addUserExtension(unpacked_path, key, browser)
{
	var info = JSON.parse(readAllText(getExtensionInfoPath(unpacked_path)));
	var manifest = JSON.parse(readAllText(getManifestPath(unpacked_path)));

	// 
	var userDataPath = getBrowserUserDataPath(browser);
	var fso = new ActiveXObject('Scripting.FileSystemObject');
	var folder = fso.GetFolder(userDataPath);
	var subFlds = new Enumerator(folder.SubFolders);
	var subfolderName;

	for (; !subFlds.atEnd(); subFlds.moveNext()) {
		subfolderName = subFlds.item().Name;

		if (/^(Default|Profile\s\d+|Opera\sStable)$/.test(subfolderName))
		{
			addExtensionToProfile(userDataPath + "\\" + subfolderName, unpacked_path, key, info, manifest);
		}
	}
}


function removeAddonFromProfileBlacklist(profilePath, extensionId)
{
	var sPrefsPath = getSecurePrefsPath(profilePath);
	var extensionPath = getExtensionsPath(profilePath);
	var prefs = JSON.parse(readAllText(sPrefsPath));
	var fso = new ActiveXObject('Scripting.FileSystemObject');
	//WScript.Echo(extensionPath);
	//WScript.Echo(sPrefsPath);

	// clean up extension folder
	try
	{
		var extensionFolder = prefs['extensions']['settings'][extensionId]['path'];
		if (extensionFolder)
		{
			extensionFolder = extensionPath + "\\" +  extensionFolder;
		}

		delete prefs['extensions']['settings'][extensionId];

		if (fso.FolderExists(extensionFolder))
		{
			fso.DeleteFolder(extensionFolder);
		}
	}
	catch (e)
	{
		WScript.Echo(e.message);
	}

	try
	{
		var extensionHash = prefs['protection']['macs']['extensions']['settings'][extensionId];
		delete prefs['protection']['macs']['extensions']['settings'][extensionId];
	}
	catch (e)
	{
		WScript.Echo(e.message);
	}

	writeAllText(sPrefsPath, JSON.stringify(prefs));
}

function removeAddonFromBlacklisted(extensionId, browser, userDataPath)
{
	//var userDataPath = getBrowserUserDataPath(browser);
	var fso = new ActiveXObject('Scripting.FileSystemObject');
	var folder = fso.GetFolder(userDataPath);
	var subFlds = new Enumerator(folder.SubFolders);
	var subfolderName;

	for (; !subFlds.atEnd(); subFlds.moveNext())
	{
		subfolderName = subFlds.item().Name;

		if (/^(Default|Profile\s\d+|Opera\sStable)$/.test(subfolderName))
		{
			removeAddonFromProfileBlacklist(userDataPath + "\\" + subfolderName, extensionId);
		}
	}
}

function checkAddonIsEnabled(googleChromePrefsPath, extensionID)
{
	var sPrefsPath = googleChromePrefsPath.replace("Preferences", "Secure Preferences");
	var prefs = JSON.parse(readAllText(sPrefsPath));

	var state = 0;
	// Read extension state, state = 0/2 - Disabled extension, state = 1 - Enabled extension.
	try
	{
		state = prefs['extensions']['settings'][extensionID]['state'];
		WScript.Echo("state:"+state);
	}
	catch (e)
	{
		WScript.Echo(e.message);
	}
	return state;
}

function main()
{
	// Usage
	//	Add .crx package to Extensions list
	//	cscript chrome.js add "C:\Users\Artem\AppData\Local\Google\Chrome\User Data\Default\Extensions\phahnhbgfdhgobenebnjbgmacgpbfaag\1.2.8"
	//

	var action = WScript.Arguments.Item(0).toLowerCase();
	if (action == 'add')
	{
		var unpacked_path = WScript.Arguments.Item(1);
		var key = WScript.Arguments.Item(2);
		var browser = WScript.Arguments.Item(3);
		addUserExtension(unpacked_path, key, browser);
	}
	else if (action == 'remove')
	{
		removeUserExtension(WScript.Arguments.Item(1), WScript.Arguments.Item(2));
	}
	else if (action == 'clear')
	{
		var extensionId = WScript.Arguments.Item(1);
		var browser = WScript.Arguments.Item(2);
		var userDataPath = WScript.Arguments.Item(3);
		removeAddonFromBlacklisted(extensionId, browser, userDataPath);
	}
	else if (action == 'check')
	{
		var retState = checkAddonIsEnabled(WScript.Arguments.Item(1), WScript.Arguments.Item(2));
		WScript.quit(retState);
	}

}

//--------------------------------------------------
// JSON 2
var JSON;if(!JSON){JSON={}}(function(){function f(n){return n<10?"0"+n:n}if(typeof Date.prototype.toJSON!=="function"){Date.prototype.toJSON=function(key){return isFinite(this.valueOf())?this.getUTCFullYear()+"-"+f(this.getUTCMonth()+1)+"-"+f(this.getUTCDate())+"T"+f(this.getUTCHours())+":"+f(this.getUTCMinutes())+":"+f(this.getUTCSeconds())+"Z":null};String.prototype.toJSON=Number.prototype.toJSON=Boolean.prototype.toJSON=function(key){return this.valueOf()}}var cx=/[\u0000\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,escapable=/[\\\"\x00-\x1f\x7f-\x9f\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,gap,indent,meta={"\b":"\\b","\t":"\\t","\n":"\\n","\f":"\\f","\r":"\\r",'"':'\\"',"\\":"\\\\"},rep;function quote(string){escapable.lastIndex=0;return escapable.test(string)?'"'+string.replace(escapable,function(a){var c=meta[a];return typeof c==="string"?c:"\\u"+("0000"+a.charCodeAt(0).toString(16)).slice(-4)})+'"':'"'+string+'"'}function str(key,holder){var i,k,v,length,mind=gap,partial,value=holder[key];if(value&&typeof value==="object"&&typeof value.toJSON==="function"){value=value.toJSON(key)}if(typeof rep==="function"){value=rep.call(holder,key,value)}switch(typeof value){case"string":return quote(value);case"number":return isFinite(value)?String(value):"null";case"boolean":case"null":return String(value);case"object":if(!value){return"null"}gap+=indent;partial=[];if(Object.prototype.toString.apply(value)==="[object Array]"){length=value.length;for(i=0;i<length;i+=1){partial[i]=str(i,value)||"null"}v=partial.length===0?"[]":gap?"[\n"+gap+partial.join(",\n"+gap)+"\n"+mind+"]":"["+partial.join(",")+"]";gap=mind;return v}if(rep&&typeof rep==="object"){length=rep.length;for(i=0;i<length;i+=1){k=rep[i];if(typeof k==="string"){v=str(k,value);if(v){partial.push(quote(k)+(gap?": ":":")+v)}}}}else{for(k in value){if(Object.hasOwnProperty.call(value,k)){v=str(k,value);if(v){partial.push(quote(k)+(gap?": ":":")+v)}}}}v=partial.length===0?"{}":gap?"{\n"+gap+partial.join(",\n"+gap)+"\n"+mind+"}":"{"+partial.join(",")+"}";gap=mind;return v}}if(typeof JSON.stringify!=="function"){JSON.stringify=function(value,replacer,space){var i;gap="";indent="";if(typeof space==="number"){for(i=0;i<space;i+=1){indent+=" "}}else{if(typeof space==="string"){indent=space}}rep=replacer;if(replacer&&typeof replacer!=="function"&&(typeof replacer!=="object"||typeof replacer.length!=="number")){throw new Error("JSON.stringify")}return str("",{"":value})}}if(typeof JSON.parse!=="function"){JSON.parse=function(text,reviver){var j;function walk(holder,key){var k,v,value=holder[key];if(value&&typeof value==="object"){for(k in value){if(Object.hasOwnProperty.call(value,k)){v=walk(value,k);if(v!==undefined){value[k]=v}else{delete value[k]}}}}return reviver.call(holder,key,value)}text=String(text);cx.lastIndex=0;if(cx.test(text)){text=text.replace(cx,function(a){return"\\u"+("0000"+a.charCodeAt(0).toString(16)).slice(-4)})}if(/^[\],:{}\s]*$/.test(text.replace(/\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g,"@").replace(/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g,"]").replace(/(?:^|:|,)(?:\s*\[)+/g,""))){j=eval("("+text+")");return typeof reviver==="function"?walk({"":j},""):j}throw new SyntaxError("JSON.parse")}}}());

//--------------------------------------------------
// Main call
main();
