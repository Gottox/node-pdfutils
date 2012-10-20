var Stream = require('stream').Stream;
var util = require('util')
var fs = require('fs')
var Document = require('../build/Release/pdfconv').Document;

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
