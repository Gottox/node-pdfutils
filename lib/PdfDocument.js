/*
 * PdfDocument.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */
var inherits = require('util').inherits;

var PdfDocument = require("./bindings").PdfDocument;
inherits(PdfDocument, Array);

module.exports = PdfDocument;
