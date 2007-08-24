/*

  Overrides default download manager of Firefox.

*/

var fdm_FDM = Components.classes["@freedownloadmanager.org/FDMForFirefox;1"].createInstance ();
fdm_FDM = fdm_FDM.QueryInterface (Components.interfaces.IFDMForFirefox);

// set our "on load" handler.
window.addEventListener("load",  fdm_DM_load, false);

// extract HTTP POST data of the last download
function fdm_extractPostData(channel) 
{
    if(channel instanceof Components.interfaces.nsIUploadChannel &&
       channel.uploadStream instanceof Components.interfaces.nsISeekableStream) {
      try {
        channel.uploadStream.seek(0,0);
        const sis=Components.classes[
          '@mozilla.org/scriptableinputstream;1'].createInstance(
          Components.interfaces.nsIScriptableInputStream);
        sis.init(channel.uploadStream);
        var postData=sis.read(sis.available()).replace(/\s$/,'').split(/[\r\n]/);
        return postData[postData.length-1];
      } catch(ex) {
      } finally {
         sis.close();
      }
    }
    return null;
}

function fdm_DM_load (ev)
{
  var url = Components.classes["@freedownloadmanager.org/FDMUrl;1"].createInstance ();
  url = url.QueryInterface (Components.interfaces.IFDMUrl);
  url.Url = dialog.mLauncher.source.spec;

  if (fdm_FDM.IsLinkShouldBeSkipped (url, dialog.mLauncher.suggestedFileName)) 
    return; // do default processing

  try {
    var doc = dialog.mContext.QueryInterface(Components.interfaces.nsIWebNavigation).document;
    if (doc.URL != "about:blank")
	url.Referer = doc.URL;
    url.Cookies = doc.cookie;
  } catch(err) {}

  var fdm_Ext = Components.classes["@freedownloadmanager.org/FDMFirefoxExtension;1"].createInstance ();
  fdm_Ext = fdm_Ext.QueryInterface (Components.interfaces.IFDMFirefoxExtension);

  // fdm_Ext is used by us to store the last HTTP post data
  // extract it if available
  var lastPost = fdm_Ext.LastPost; 
  if (lastPost != null)
  {
    lastPost = lastPost.QueryInterface (Components.interfaces.nsIHttpChannel)
    if (lastPost.isPending () && url.Url == lastPost.URI.spec)
      url.PostData = fdm_extractPostData (lastPost);
  }

  if (fdm_FDM.CatchLink (url, dialog.mLauncher.suggestedFileName))
    document.documentElement.cancelDialog(); // download was forwared to FDM. cancel processing by browser.
}