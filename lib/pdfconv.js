var Stream = require('stream').Stream;
var util = require('util')
var fs = require('fs')
var internal = require('../build/Debug/pdfconv')
  , Document = internal.Document
  , PageJob = internal.PageJob;

util.inherits(PageJob, Stream);

PageJob.prototype.toFile = function() {
	return this.pipe(fs.createWriteStream.apply(fs, arguments));
}

function pdfconv(src, callback) {
	if(typeof src == 'string') {
		fs.readFile(src, function(err, data) {
			if(err)
				throw err;
			pdfconv(data, callback);
		});
		return;
	}

	var doc = new Document(src, callback);
}

exports.pdfconv = pdfconv
