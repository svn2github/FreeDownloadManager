// thanks to Noitidart
// http://stackoverflow.com/questions/26175475/add-an-option-to-the-download-file-dialog

const {Cc,Ci,Cu} = require("chrome");
Cu.import ("resource://gre/modules/Services.jsm");

function WindowListener ()
{
	////////////////////////////////////////////////////////
	//DO NOT EDIT HERE
	////////////////////////////////////////////////////////
    this.onOpenWindow = function (aXULWindow) 
	{
        // Wait for the window to finish loading
        let aDOMWindow = aXULWindow.QueryInterface(Ci.nsIInterfaceRequestor).getInterface(Ci.nsIDOMWindowInternal || Ci.nsIDOMWindow);
        aDOMWindow.addEventListener('load', function() 
		{
            aDOMWindow.removeEventListener('load', arguments.callee, false);
            this.loadIntoWindow(aDOMWindow);
        }.bind (this), false);
    };
	
    this.onCloseWindow = function(aXULWindow) {};
	
	this.onWindowTitleChange = function(aXULWindow, aNewTitle) {};
	
    this.register = function() 
	{
        // Load into any existing windows
        let DOMWindows = Services.wm.getEnumerator(null);
        while (DOMWindows.hasMoreElements()) 
		{
            let aDOMWindow = DOMWindows.getNext();
            this.loadIntoWindow(aDOMWindow);
        }
        // Listen to new windows
        Services.wm.addListener(this);
    };
	
    this.unregister = function() 
	{
        // Unload from any existing windows
        let DOMWindows = Services.wm.getEnumerator(null);
        while (DOMWindows.hasMoreElements()) 
		{
            let aDOMWindow = DOMWindows.getNext();
            this.unloadFromWindow(aDOMWindow);
        }
        //Stop listening so future added windows don't get this attached
        Services.wm.removeListener(this);
    };
	////////////////////////////////////////////////////////
    //END - DO NOT EDIT HERE
	////////////////////////////////////////////////////////
	
	this.loadIntoWindow = function(aDOMWindow) 
	{
        if (!aDOMWindow) {
            return;
        }
    };
	
    this.unloadFromWindow = function(aDOMWindow) 
	{
        if (!aDOMWindow) {
            return;
        }
    }
}

function XulWindowListener (xulUrl)
{
	WindowListener.call (this);
	
	this.xulUrl = xulUrl;
	
	this.loadIntoWindow = function(aDOMWindow) 
	{
        if (!aDOMWindow) {
            return;
        }
		
		if (aDOMWindow.location == this.xulUrl)
			this.loadIntoXulWindow (aDOMWindow);
    };
	
    this.unloadFromWindow = function(aDOMWindow) 
	{
        if (!aDOMWindow) {
            return;
        }
    }
	
	this.loadIntoXulWindow = function (window) {};
}


exports.WindowListener = WindowListener;
exports.XulWindowListener = XulWindowListener;