#include <node.h>

using namespace v8;
using namespace node;

char *str2chr(Handle<Value> val) {
	String::Utf8Value utf8(val->ToString());

	int len = utf8.length() + 1;
	char *str = new char[len];
	strncpy(str, *utf8, len);

	return str;
}

