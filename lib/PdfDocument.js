/*
 * PdfDocument.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

var inherits = require('util').inherits;

var bindings = require("./bindings");
var PdfDocument = bindings.PdfDocument;

var aliases = {
	asSVG: 'svg',
	asSvg: 'svg',
	asPNG: 'png',
	asPng: 'png',
	asText: 'text',
	asPDF: 'pdf',
	asPdf: 'pdf'
};

function mkAlias(value) {
	return function() {
		var args = Array.prototype.slice.call(arguments);
		args.unshift(value);
		this.as.apply(this, args);
	};
}

for(var key in aliases) {
	PdfDocument.prototype[key] = mkAlias(aliases[key]);
}

PdfDocument.prototype.as = bindings._document.as;

inherits(PdfDocument, Array);

module.exports = PdfDocument;
