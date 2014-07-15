/*
 * PdfPage.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */


var bindings = require('./bindings');
var PdfPage = bindings.PdfPage;

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
	PdfPage.prototype[key] = mkAlias(aliases[key]);
}

PdfPage.prototype.as = bindings._page.as;

module.exports = PdfPage;
