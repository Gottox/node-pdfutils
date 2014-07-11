/*
 * v8utils.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "v8utils.h"
#include <cstring>

int v8ToInt(v8::Local<v8::Value> value, int fallback) {
	if (value->IsNumber()) {
		return value->IntegerValue();
	}
	return fallback;
}

double v8ToDouble(v8::Local<v8::Value> value, int fallback) {
	if (value->IsNumber()) {
		return value->NumberValue();
	}
	return fallback;
}
