#ifndef ERRORFIXING_H
#define ERRORFIXING_H

#include <string>
using namespace std;

struct ErrorInfo;

// Fixes XML structural errors found by the parser
string fixErrors(const string &content, ErrorInfo &errors);

#endif
