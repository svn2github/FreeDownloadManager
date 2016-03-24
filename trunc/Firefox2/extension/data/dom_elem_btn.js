function DomElementButton (target)
{
	this.target = target;
	this.button = null;
	
	this.buttonPartElement = function (elem)
	{
		if (elem == this.button)
			return true;
		if (elem.parentNode)
			return this.buttonPartElement (elem.parentNode);
		return false;
	};
	
	this.show = function (sh)
	{
		throw "pure function call";
	};
}

function DomElementButtonsManager (window)
{
	this.window = window;
	this.button = null;
	this.buttons = new WeakMap;
	this.lastElement = null;
	
	this.onMouseEventInElement = function (ev)
	{
		ev = ev || this.window.event;
		var elem = ev.target || ev.srcElement;
		
		if (this.lastElement == elem)
			return; // nothing to do
		
		this.lastElement = elem;
		
		if (this.button)
		{
			if (this.button.buttonPartElement (elem) ||
				this.button.target == elem)
			{
				return;
			}
			
			this.button.show (false);
			this.button = null;
		}
		
		var button = this.buttons.get (elem);
		if (button)
		{
			this.showButton (button);
			return;
		}
		
		this.queryElementType (elem, this.onElementType.bind (this, elem));
	};
	
	this.onElementType = function (elem, isTarget, suggestedElem)
	{
		if (!isTarget)
			return;
			
		var button = this.createButton (suggestedElem ? suggestedElem : elem);
		this.buttons.set (elem, button);
		
		if (elem == this.lastElement)
			this.showButton (button);
	};
	
	this.showButton = function (button)
	{
		this.button = button;
		this.button.show (true);
	};
	
	this.createButton = function (target)
	{
		throw "pure function call";
	};
	
	this.queryElementType = function (elem, callback)
	{
		throw "pure function call";
	};
	
	var onMouseEventInElementBinded = this.onMouseEventInElement.bind (this);
	
	this.register = function ()
	{
		this.window.addEventListener ("mousemove", 
			onMouseEventInElementBinded, true);
			
		this.window.addEventListener ("mouseover", 
			onMouseEventInElementBinded, false);
	};
	
	this.unregister = function ()
	{		
		this.window.removeEventListener ("mousemove", 
			onMouseEventInElementBinded, true);
			
		this.window.removeEventListener ("mouseover", 
			onMouseEventInElementBinded, false);
			
		if (this.button)
		{
			this.button.show (false);
			this.button = null;
		}
		
		this.lastElement = null;
	};
}