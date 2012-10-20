#include <node.h>

using namespace v8;
using namespace node;
#define LENGTH(X)               (sizeof X / sizeof X[0])

char *str2chr(Handle<Value> val);
void print_trace (void);
