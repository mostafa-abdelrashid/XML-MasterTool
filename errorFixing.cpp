string fixXMLErrors(const vector<Token>& tokens,
    vector<int>& errorLines,
    int& errorCount) {
    stack<string> st;
    string fixedXML = "";
    errorCount = 0;

    for (const Token& token : tokens) {
        switch (token.type) {

        case OPEN_TAG: {
            string tagName = token.value;

            // Possible missing closing slash
            if (!st.empty() && st.top() == tagName) {
                st.pop();
                fixedXML += "</" + tagName + ">";
                errorLines.push_back(token.line);
                errorCount++;
            }
            else {
                st.push(tagName);
                fixedXML += "<" + tagName + ">";
            }
            break;
        }

        case CLOSE_TAG: {
            string tagName = token.value;

            if (!st.empty() && st.top() == tagName) {
                st.pop();
                fixedXML += "</" + tagName + ">";
            }
            else {
                errorLines.push_back(token.line);

                while (!st.empty() && st.top() != tagName) {
                    fixedXML += "</" + st.top() + ">";
                    st.pop();
                    errorCount++;
                }

                if (!st.empty()) st.pop();
                fixedXML += "</" + tagName + ">";
                errorCount++;
            }
            break;
        }

        case TEXT_NODE:
            fixedXML += token.value;
            break;

        case UNKNOWN: {
            string tagName = token.value;
            bool isClosing = false;

            if (!tagName.empty() && tagName[0] == '/') {
                isClosing = true;
                tagName = tagName.substr(1);
            }

            if (isClosing) {
                while (!st.empty() && st.top() != tagName) {
                    fixedXML += "</" + st.top() + ">";
                    st.pop();
                    errorCount++;
                }

                if (!st.empty()) st.pop();
                fixedXML += "</" + tagName + ">";
                errorCount++;
            }
            else {
                if (!st.empty() && st.top() == tagName) {
                    st.pop();
                    fixedXML += "</" + tagName + ">";
                }
                else {
                    st.push(tagName);
                    fixedXML += "<" + tagName + ">";
                }
                errorCount++;
            }

            errorLines.push_back(token.line);
            break;
        }

        } // switch
    } // for

    while (!st.empty()) {
        fixedXML += "</" + st.top() + ">";
        st.pop();
        errorCount++;
    }

    return fixedXML;
}
