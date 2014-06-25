/*
 * JsReader.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef JSREADER_H
#define JSREADER_H

#include "jsShim.h"
#include <node.h>
#include <v8.h>

/**
 * @brief Binder Class between JS-Events and C++
 * 
 * This class provides a Javascript interface to send chunks of data.
 * The C++ side can either trigger events directly or wait till the needed
 * amount of data has been received.
 */
class JsReader : node::ObjectWrap {
	JS_SHIM_H

public:
	/**
	 * Constructor
	 */
	JsReader(v8::Arguments &args) {
	
	}

};

#endif /* !JSREADER_H */
