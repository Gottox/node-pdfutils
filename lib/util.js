/*
 * utils.js
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */


exports.mkAlias = function(value) {
	return function() {
		var args = Array.prototype.slice.call(arguments);
		args.unshift(value);
		this.as.apply(this, args);
	};
};
