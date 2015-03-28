/*
 * PdfDocument.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

var inherits = require('util').inherits;
var myUtil = require('./util');

var bindings = require("./bindings");
var PdfDocument = bindings.PdfDocument;

var aliases = {
	asPDF: 'pdf',
	asPdf: 'pdf'
};

PdfDocument.prototype.as = bindings._document.as;
for(var key in aliases) {
	PdfDocument.prototype[key] = myUtil.mkAlias(aliases[key]);
}

module.exports = PdfDocument;
