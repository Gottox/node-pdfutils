/*
 * pdfutils.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

var env = process.env.PDFUTILS_ENV || "Release";

var fs = require("fs");
var path = require("path");
var stream = require("stream");
var extend = require("extend");
var inherits = require('util').inherits;

var bindings = require('./bindings');
var _document = bindings._document;
var PdfDocument = require("./PdfDocument");
var PdfPage =  require("./PdfPage");

function cbWrapper(cb, doc) {
	if(!cb)
		return null;
	return function(err) {
		if(err)
			return cb(err, null);
		return cb(null, doc);
	};
}

function pdfPageFactory(doc, n) {
	for(var i = 0; i < n; i++) {
		doc[i] = new PdfPage();
	}
}

function pdfutils(src, options, cb) {
	var reader;
	var result;
	var doc = new PdfDocument();

	if(typeof options === 'function') {
		cb = options;
		options = undefined;
	}

	var callback = cb;

	options = extend({
	}, options || {});

	// TODO: Don't use popplerEngine by default
	if(!options.engine)
		options.engine = require('../build/'+env+'/popplerEngine').engine;

	if(typeof src === 'string' || src instanceof String) {
		src = path.resolve(src);
		_document.load.call(doc, src, options, cbWrapper(cb, doc));
	}
	else if(Buffer.isBuffer(src)) {
		// Async Read Buffer
		if(cb) {
			reader = bindings.openFromReader(doc, PdfPage, options.password,
					cbWrapper(cb, doc));
			reader.data(src);
			reader.end();
		}
		// Sync Read Buffer
		else {
			_document.load.call(doc, src, options, cbWrapper(cb, doc));
		}
	}
	else if(typeof src === 'function' && src instanceof stream.Readable) {
		// Async Stream Read
		if(cb) {
			reader = bindings.asyncLoad(doc, options.password,
					cbWrapper(cb, doc));
			src.on('data', function(data) {
				reader.data(data);
			})
			.on('end', function() {
				reader.end();
			})
			.on('error', function(err) {
				reader.cancel();
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

pdfutils.PdfDocument = PdfDocument;
pdfutils.PdfPage = PdfPage;
module.exports = pdfutils;
