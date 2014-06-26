/*
 * pdfutils.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

var env = process.env.PDFUTILS_ENV;

var fs = require("fs");
var path = require("path");
var stream = require("stream");
var extend = require("extend");
var native = require('../build/'+env+'/pdfutils');
var inherits = require('util').inherits;

var PdfDocument = native.PdfDocument;
var PdfPage = native.PdfPage;

inherits(PdfDocument, require("./PdfDocument"));
inherits(PdfPage, require("./PdfPage"));

function cbWrapper(cb, doc) {
	if(!cb)
		return null;
	return function(err) {
		if(err)
			return cb(err, null);
		return cb(null, err);
	};
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
		engine: 'poppler'
	}, options || {});

	doc._engine = require('../build/'+env+'/'+options.engine+'Engine').engine;

	if(typeof src === 'string' || src instanceof String) {
		src = path.resolve(src);
		native.openFromPath(doc, options.password, src,
							   cbWrapper(cb, doc));
	}
	else if(src instanceof Buffer) {
		// Async Read Buffer
		if(cb) {
			reader = native.openFromReader(doc, options.password,
					cbWrapper(cb, doc));
			reader.data(src);
			reader.end();
		}
		// Sync Read Buffer
		else {
			native.syncLoad(doc, options.password, src);
		}
	}
	else if(src instanceof stream.Readable) {
		// Async Stream Read
		if(cb) {
			reader = native.asyncLoad(doc, options.password,
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
