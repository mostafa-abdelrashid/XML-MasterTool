#include "error_check.h"
#include "XMLTokenizer.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

// ======================= CORE LOGIC =======================

ErrorInfo countErrorSummary(const std::string &filePath)
{
    ErrorInfo errors;
    XMLTokenizer tokenizer;

    std::vector<Token> tokens = tokenizer.tokenize(filePath);
    std::stack<Token> tagStack;

    for (const auto &t : tokens)
    {
        if (t.type == OPEN_TAG)
        {
            tagStack.push(t);
        }
        else if (t.type == CLOSE_TAG)
        {
            if (tagStack.empty())
            {
                errors.addError(
                    t.line,
                    0,
                    t.value,
                    "Mismatched",
                    "Closing tag without opening <" + t.value + ">"
                );
            }
            else if (tagStack.top().value != t.value)
            {
                errors.addError(
                    t.line,
                    0,
                    t.value,
                    "Mismatched",
                    "Expected </" + tagStack.top().value +
                    "> but found </" + t.value + ">"
                );
                tagStack.pop();
            }
            else
            {
                tagStack.pop();
            }
        }
    }

    while (!tagStack.empty())
    {
        Token t = tagStack.top();
        tagStack.pop();

        errors.addError(
            t.line,
            0,
            t.value,
            "Missing",
            "Missing closing tag </" + t.value + ">"
        );
    }

    return errors;
}

// ======================= OUTPUT =======================

void highlightErrors(const std::string &filePath, const ErrorInfo &errors)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    int lineNum = 1;
    int i = 0;

    while (std::getline(file, line))
    {
        if (i < errors.count && errors.lines[i] == lineNum)
        {
            std::cout << ">> ERROR: "
                      << errors.descriptions[i]
                      << "\n>> LINE " << lineNum << ": "
                      << line << "\n\n";
            i++;
        }
        else
        {
            std::cout << line << "\n";
        }
        lineNum++;
    }

    while (i < errors.count)
    {
        std::cout << ">> ERROR: " << errors.descriptions[i] << "\n";
        i++;
    }
}
