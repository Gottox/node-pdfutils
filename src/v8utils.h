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

char *v8ToChar(v8::Local<v8::Value> value);

int v8ToInt(v8::Local<v8::Value> value, int fallback = -1);

v8::Handle<v8::Value> charToV8(const char *value);

#endif /* !V8UTILS_H */
