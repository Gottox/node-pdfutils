/*
 * PdfDocument.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

var inherits = require('util').inherits;

var bindings = require("./bindings");
var PdfDocument = bindings.PdfDocument;

PdfDocument.prototype.as = bindings._document.as;

inherits(PdfDocument, Array);

module.exports = PdfDocument;
