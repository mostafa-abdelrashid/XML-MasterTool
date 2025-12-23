#ifndef ERROR_CHECK_H
#define ERROR_CHECK_H

#include <string>

struct ErrorInfo
{
    static const int MAX_ERRORS = 100;

    int lines[MAX_ERRORS];
    std::string types[MAX_ERRORS];
    std::string tags[MAX_ERRORS];
    std::string descriptions[MAX_ERRORS];
    int count = 0;

    void addError(int line,
                  int level,
                  const std::string &tag,
                  const std::string &type,
                  const std::string &desc)
    {
        if (count >= MAX_ERRORS) return;
        lines[count] = line;
        types[count] = type;
        tags[count] = tag;
        descriptions[count] = desc;
        count++;
    }
};

ErrorInfo countErrorSummary(const std::string &filePath);
void highlightErrors(const std::string &filePath, const ErrorInfo &errors);

#endif
