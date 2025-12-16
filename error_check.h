#ifndef ERROR_CHECK_H
#define ERROR_CHECK_H

#include <string>
#include <vector>
#include <iostream>

// Token struct used by the error checker
struct Token {
    std::string type;
    std::string text;
    int level {};
};

// ErrorInfo struct to hold error details
struct ErrorInfo {
    int count {};
    std::vector<int> lines;
    std::vector<std::string> descriptions;
    std::vector<std::string> types;
    std::vector<std::string> tags;
    std::vector<int> levels;

    // Helper to add error details
    void addError(int line, int level,
                  const std::string& tag,
                  const std::string& type,
                  const std::string& desc)
    {
        count++;
        lines.push_back(line);
        levels.push_back(level);
        tags.push_back(tag);
        types.push_back(type);
        descriptions.push_back(desc);
    }
};

// Function Declarations
std::vector<Token> tokenizeLine(const std::string& line);
bool verifyImproved(const std::string& tokens);
bool isClosingTag(const std::string &tag);
std::string getTagName(const std::string &tagText);

// Main function to detect errors
ErrorInfo countErrorSummary(const std::string &content);

// Helper to print errors to console
void highlightErrors(const std::string &content, const ErrorInfo &errors);

#endif // ERROR_CHECK_H