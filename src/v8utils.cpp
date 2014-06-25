/*
 * v8utils.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "v8utils.h"
#include <cstring>

char *v8toChar(v8::Local<v8::Value> value) {
	if (value->IsString()) {
		v8::String::AsciiValue string(value);
		int len = string.length() + 1;
		char *str = new char[len];
		memcpy(str, *string, len);
		return str;
	}
	return NULL;
}

int v8toInt(v8::Local<v8::Value> value, int fallback) {
	if (value->IsNumber()) {
		return value->IntegerValue();
	}
	return fallback;
}
