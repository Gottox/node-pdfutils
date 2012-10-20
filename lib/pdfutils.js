var Stream = require('stream').Stream;
var util = require('util')
var fs = require('fs')
var internal = require('../build/Debug/pdfutils')
  , Document = internal.Document
  , PageJob = internal.PageJob;

util.inherits(PageJob, Stream);

PageJob.prototype.toFile = function() {
	return this.pipe(fs.createWriteStream.apply(fs, arguments));
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
