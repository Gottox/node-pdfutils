/*
 * PdfPage.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */


var bindings = require('./bindings');
var myUtil = require('./util');
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

for(var key in aliases) {
	PdfPage.prototype[key] = myUtil.mkAlias(aliases[key]);
}

PdfPage.prototype.as = bindings._page.as;

module.exports = PdfPage;
