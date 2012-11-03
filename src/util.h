#include <map>
#define LENGTH(X)               (sizeof X / sizeof X[0])
typedef std::map<int, const char *> Settings;
typedef std::pair<int, const char *> Setting;
