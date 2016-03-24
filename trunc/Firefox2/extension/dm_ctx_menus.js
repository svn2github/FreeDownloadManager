var sdkContextMenu = require("sdk/context-menu");

function ContextMenuInfo ()
{
  this.label = "";

  this.setCtxPredicateFn = function (fn) 
  {
    this.context = sdkContextMenu.PredicateContext (fn);
  };

  // first arg - the target function
  // 2nd,... args - functions the target one depends on
  this.setOnClickDomProcessorFn = function (csfile, script) 
  {
	this.contentScriptFile = csfile;
	this.contentScript = script;
  };

  this.setOnClickFn = function (fn)
  {
    this.onMessage = fn;
  };


  this.setCtxPredicateFn (function () {return true;});
  this.setOnClickFn (function () {});
}


function ContextMenu ()
{
  this.menu = null;
  this.menuInfo = new ContextMenuInfo ();
  this.registerMenu = function () {
    this.menu = sdkContextMenu.Item (this.menuInfo);
  };
}


function DlLinkContextMenu ()
{
  ContextMenu.call (this);

  this.menuInfo.setCtxPredicateFn (function (ctx) {
    return ctx.targetName == "a" || ctx.targetName == "img";
  });

  this.menuInfo.setOnClickDomProcessorFn (
		"./dm_ctx_menus_fns.js",
		"ctxmenuOnClick(DlLinkContextMenuDomProcessor);");

  this.menuInfo.setOnClickFn (function (link_info) {});
}
DlLinkContextMenu.prototype = Object.create (ContextMenu.prototype);


function DlAllContextMenu ()
{
	ContextMenu.call (this);
	
	this.menuInfo.context = sdkContextMenu.PageContext ();
	
	this.menuInfo.setOnClickDomProcessorFn (
		"./dm_ctx_menus_fns.js",
		"ctxmenuOnClick(DlAllContextMenuDomProcessor);");
	
	this.menuInfo.setOnClickFn (function (info) {});
}


function DlSelectedContextMenu ()
{
	ContextMenu.call (this);
	
	this.menuInfo.context = sdkContextMenu.SelectionContext ();
	
	this.menuInfo.setOnClickDomProcessorFn (
		"./dm_ctx_menus_fns.js",
		"ctxmenuOnClick(DlSelectedContextMenuDomProcessor);");

	this.menuInfo.setOnClickFn (function (info) {});
}


function DlVideoContextMenu ()
{
	ContextMenu.call (this);
	
	this.menuInfo.context = sdkContextMenu.PageContext ();
	
	this.menuInfo.setOnClickDomProcessorFn (
		"./dm_ctx_menus_fns.js",
		"ctxmenuOnClick(DlVideoContextMenuDomProcessor);");
	
	this.menuInfo.setOnClickFn (function (info) {});
}


function DmContextMenus ()
{
  this.dlLink = new DlLinkContextMenu ();
  this.dlAll = new DlAllContextMenu ();
  this.dlSelected = new DlSelectedContextMenu ();
  this.dlVideo = new DlVideoContextMenu ();
}


exports.DmContextMenus = DmContextMenus;