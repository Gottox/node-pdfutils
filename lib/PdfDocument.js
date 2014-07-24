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
inherits(PdfDocument, Array);

PdfDocument.prototype.as = bindings._document.as;
PdfDocument.prototype.asPdf = PdfDocument.prototype.asPDF = myUtil.mkAlias('pdf');

module.exports = PdfDocument;
