/*
 * v8utils.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef V8UTILS_H
#define V8UTILS_H

#include <v8.h>

char *v8ToChar(v8::Local<v8::Value> value);

int v8ToInt(v8::Local<v8::Value> value, int fallback = -1);

#endif /* !V8UTILS_H */
