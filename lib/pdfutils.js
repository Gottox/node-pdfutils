var Stream = require('stream').Stream;
var util = require('util')
var fs = require('fs')

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
