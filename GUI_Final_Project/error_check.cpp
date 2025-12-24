#include "error_check.h"
#include "XMLTokenizer.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

using namespace std;

// ======================= CORE LOGIC =======================

ErrorInfo countErrorSummary(const string &filePath)
{
    ErrorInfo errors;
    XMLTokenizer tokenizer;

    vector<Token> tokens = tokenizer.tokenize(filePath);
    stack<Token> tagStack;

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

void highlightErrors(const string &filePath, const ErrorInfo &errors)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Cannot open file: " << filePath << endl;
        return;
    }

    string line;
    int lineNum = 1;
    int i = 0;

    while (getline(file, line))
    {
        if (i < errors.count && errors.lines[i] == lineNum)
        {
            cout << ">> ERROR: "
                 << errors.descriptions[i]
                 << "\n>> LINE " << lineNum << ": "
                 << line << "\n\n";
            i++;
        }
        else
        {
            cout << line << "\n";
        }
        lineNum++;
    }

    while (i < errors.count)
    {
        cout << ">> ERROR: " << errors.descriptions[i] << "\n";
        i++;
    }
}
