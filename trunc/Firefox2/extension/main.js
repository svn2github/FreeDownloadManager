var {FdmExtension} = require ("./fdm_extension.js");
var fdmext = new FdmExtension;
fdmext.register ();

exports.onUnload = function (reason)
{
	fdmext.unregister ();
};

/*var self = require('sdk/self');
// a dummy function, to show how tests work.
// to see how to test this function, look at test/test-index.js
function dummy(text, callback) {
  callback(text);
}
exports.dummy = dummy;*/