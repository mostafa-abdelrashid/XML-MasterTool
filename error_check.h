#ifndef ERROR_CHECK_H
#define ERROR_CHECK_H

#include <string>
using namespace std;

struct ErrorInfo
{
    static const int MAX_ERRORS = 100;

    int lines[MAX_ERRORS];
    string types[MAX_ERRORS];
    string tags[MAX_ERRORS];
    string descriptions[MAX_ERRORS];
    int count = 0;

    void addError(int line,
                  int level,
                  const string &tag,
                  const string &type,
                  const string &desc)
    {
        if (count >= MAX_ERRORS) return;
        lines[count] = line;
        types[count] = type;
        tags[count] = tag;
        descriptions[count] = desc;
        count++;
    }
};

ErrorInfo countErrorSummary(const string &filePath);
void highlightErrors(const string &filePath, const ErrorInfo &errors);

#endif
