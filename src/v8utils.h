/*
 * v8utils.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef V8UTILS_H
#define V8UTILS_H

#include <v8.h>

#define THROW_FREE(t, msg) { \
	v8::Handle<v8::Value> ex = v8::ThrowException(v8::Exception:: t (v8::String::New(msg))); \
	delete msg; \
	return ex; \
}

#define THROW(t, msg) { \
	return v8::ThrowException(v8::Exception:: t (v8::String::New(msg))); \
}

int v8ToInt(v8::Local<v8::Value> value, int fallback = -1);

double v8ToDouble(v8::Local<v8::Value> value, double fallback = 0.0);

#endif /* !V8UTILS_H */
