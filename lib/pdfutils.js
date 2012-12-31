var Stream = require('stream').Stream;
var util = require('util')
var fs = require('fs')

function extend(opt) {
	for(var i = 1; i < arguments.length; i++)
		for(var k in arguments[i])
			opt[k] = arguments[i][k];
		return opt;
}

var bindings
try {
	bindings = require('../build/Release/pdfutils');
} catch (e) { try {
	bindings = require('../build/Debug/pdfutils');
} catch (e) { try {
	bindings = require('../build/default/pdfutils');
} catch (e) {
	throw e;
}}}

var Document = bindings.Document
  , PageJob = bindings.PageJob;

util.inherits(PageJob, Stream);

PageJob.prototype.toFile = function() {
	return this.pipe(fs.createWriteStream.apply(fs, arguments), {end: true});
}
Object.defineProperty(PageJob.prototype, "links", {
	get: function() {
		var links = this.page.links;
		var result = new Array(links.length);
		var hf = this.width / this.page.width;
		var vf = this.height / this.page.height;
		for(var i = 0; i < links.length; i++) {
			result[i] = extend({}, links[i]);
			result[i].x *= hf;
			result[i].width *= hf;
			result[i].y *= vf;
			result[i].height *= vf;
		}
		return result;
	}
});

function pdfutils(src, callback) {
	if(typeof src == 'string') {
		fs.readFile(src, function(err, data) {
			if(err)
				throw err;
			pdfutils(data, callback);
		});
		return;
	}

	var doc = new Document(src, callback);
}

exports.pdfutils = pdfutils;
