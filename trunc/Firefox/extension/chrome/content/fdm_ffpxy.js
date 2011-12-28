var fdm_FfPrefs = Components.classes["@mozilla.org/preferences;1"].getService (Components.interfaces.nsIPrefBranch);

// write current proxy settings of browser to registry settings of FDM
// prot - name of protocol ("http", "ssl", "ftp")
function fdm_ReadProxyPrefs_protocol (prot)
{
  var str = "network.proxy."; 
  str += prot;
  fdm_FDM.SetProxy (prot, fdm_FfPrefs.getCharPref (str), fdm_FfPrefs.getIntPref (str + "_port"));
}

function fdm_ReadProxyPrefs ()
{
  fdm_FDM.SetProxyType (fdm_FfPrefs.getIntPref ("network.proxy.type"));
  fdm_ReadProxyPrefs_protocol ("http");
  fdm_ReadProxyPrefs_protocol ("ssl");
  fdm_ReadProxyPrefs_protocol ("ftp");
}

/*============== Listen prefs =================*/

var fdm_prefsListener = { 
 observe: function (subject, topic, data) {
        // settings was changed. update proxy settings.
        fdm_ReadProxyPrefs ();
 },

 QueryInterface: function (aIID) {
   if (!aIID.equals (Components.interfaces.nsISupports) && !aIID.equals (Components.interfaces.nsIObserver))
      throw Components.result.NS_ERROR_NO_INTERFACE; 
   return this;
 }
};

fdm_ReadProxyPrefs ();

fdm_FfPrefs.addObserver ("network.proxy", fdm_prefsListener, false);