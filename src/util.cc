#include <node.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

using namespace v8;
using namespace node;

char *str2chr(Handle<Value> val) {
	String::Utf8Value utf8(val->ToString());

	int len = utf8.length() + 1;
	char *str = new char[len];
	strncpy(str, *utf8, len);

	return str;
}

void print_trace (void)
{
	void *array[10];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace (array, 10);
	strings = backtrace_symbols (array, size);

	printf ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
		printf ("%s\n", strings[i]);

	free (strings);
}
