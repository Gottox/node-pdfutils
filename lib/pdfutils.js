/*
 * pdfutils.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

var env = process.env.PDFUTILS_ENV;

var fs = require("fs");
var stream = require("stream");
var extend = require("extend");
var PdfPage = require("./PdfPage");
var PdfDocument = require("./PdfDocument");
var native = require('../build/'+env+'/pdfutils');

function cbWrapper(cb, doc) {
	return function(err) {
		if(err)
			return cb(err, null);
		return cb(null, err);
	};
}

function pdfutils(src, options, cb) {
	var fd;
	var handle;
	var result;
	var doc = new PdfDocument();

	if(typeof options === 'function') {
		cb = options;
		options = undefined;
	}

	var callback = cb;

	options = extend({
		engine: 'poppler'
	}, options || {});

	this._engine = require('../build/'+env+'/'+options.engine+'Engine');

	if(typeof src === 'string' || src instanceof String) {
		// Async Open File
		if(cb) {
			fd = fs.open(src, "r", function(err, fd) {
				if(err)
					return cb(err);
				native.asyncLoadFromFd(doc, options.password, fd,
									   cbWrapper(cb, doc));
			});
		}
		// Sync Open File
		else {
			fd = fs.openSync(src, "r");
			native.syncLoadFromFd(doc, options.password, fd);
		}
	}
	else if(src instanceof Buffer) {
		// Async Read Buffer
		if(cb) {
			handle = native.asyncLoad(doc, options.password,
					cbWrapper(cb, doc));
			handle.data(src);
			handle.end();
		}
		// Sync Read Buffer
		else {
			native.syncLoad(doc, options.password, src);
		}
	}
	else if(src instanceof stream.Readable) {
		// Async Stream Read
		if(cb) {
			handle = native.asyncLoad(doc, options.password,
					cbWrapper(cb, doc));
			src.on('data', function(data) {
				handle.data(data);
			})
			.on('end', function() {
				handle.end();
			})
			.on('error', function(err) {
				handle.cancel();
				cb(err);
			});
		}
		// Sync Stream Read
		else {
			// TODO find better error message.
			throw new Error("streams can only be read asynchronously. "+
							"Please provide a CallBack.");
		}
	}

	if(!cb)
		return doc;
}

pdfutils.PdfDocument = require("./PdfDocument.js");
pdfutils.PdfPage = require("./PdfPage.js");
module.exports = pdfutils;
