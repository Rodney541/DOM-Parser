#include "parser.h"
#include <cctype>
#include <algorithm>

std::string DOMParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

std::pair<std::string, std::map<std::string, std::string>> DOMParser::parseTag(const std::string& tag) {
    std::string tagName;
    std::map<std::string, std::string> attributes;
    size_t spacePos = tag.find(' ');
    if (spacePos == std::string::npos) tagName = tag;
    else {
        tagName = tag.substr(0, spacePos);
        std::string attrStr = tag.substr(spacePos + 1);
        size_t pos = 0;
        while (pos < attrStr.length()) {
            while (pos < attrStr.length() && isspace(attrStr[pos])) pos++;
            if (pos >= attrStr.length()) break;
            size_t eqPos = attrStr.find('=', pos);
            if (eqPos == std::string::npos) break;
            std::string name = trim(attrStr.substr(pos, eqPos - pos));
            pos = eqPos + 1;
            while (pos < attrStr.length() && isspace(attrStr[pos])) pos++;
            if (pos < attrStr.length() && attrStr[pos] == '"') {
                pos++;
                size_t closeQuote = attrStr.find('"', pos);
                if (closeQuote == std::string::npos) break;
                std::string value = attrStr.substr(pos, closeQuote - pos);
                attributes[name] = value;
                pos = closeQuote + 1;
            }
        }
    }
    return std::make_pair(tagName, attributes);
}

Node* DOMParser::parse(const std::string& html) {
    ElementNode* root = new ElementNode("root");
    size_t pos = 0;
    parseElement(html, pos, root);
    return root;
}

void DOMParser::parseElement(const std::string& html, size_t& pos, Node* parent) {
    while (pos < html.length()) {
        size_t openBracket = html.find('<', pos);

        if (openBracket > pos) {
            std::string text = html.substr(pos, openBracket - pos);
            if (!trim(text).empty()) {
                parent->appendChild(new TextNode(text));
            }
            pos = openBracket;
        }

        if (openBracket == std::string::npos) {
            pos = html.length();
            break;
        }

        if (html.compare(openBracket, 4, "<!--") == 0) {
            size_t endComment = html.find("-->", openBracket);
            if (endComment == std::string::npos) {
                pos = html.length();
            }
            else {
                pos = endComment + 3;
            }
            continue;
        }

        if (html.compare(openBracket, 9, "<![CDATA[") == 0) {
            size_t end = html.find("]]>", openBracket);
            if (end != std::string::npos) {
                std::string data = html.substr(openBracket + 9, end - openBracket - 9);
                parent->appendChild(new CDATANode(data));
                pos = end + 3;
                continue;
            }
        }

        if (html.compare(openBracket, 2, "<!") == 0) {
            size_t closeBracket = html.find('>', openBracket);
            pos = (closeBracket == std::string::npos) ? html.length() : closeBracket + 1;
            continue;
        }

        if (html[openBracket + 1] == '/') {
            size_t closeBracket = html.find('>', openBracket);
            pos = (closeBracket == std::string::npos) ? html.length() : closeBracket + 1;
            return;
        }

        size_t closeBracket = html.find('>', openBracket);
        if (closeBracket == std::string::npos) {
            pos = html.length();
            break;
        }

        std::string tagContent = html.substr(openBracket + 1, closeBracket - openBracket - 1);
        bool selfClosing = false;
        if (!tagContent.empty() && tagContent.back() == '/') {
            selfClosing = true;
            tagContent.pop_back();
        }

        auto parsedTag = parseTag(tagContent);
        std::string tagName = parsedTag.first;
        const auto& attributes = parsedTag.second;

        ElementNode* elementNode = new ElementNode(tagName);
        for (const auto& attr : attributes) {
            elementNode->setAttribute(attr.first, attr.second);
        }
        parent->appendChild(elementNode);
        pos = closeBracket + 1;

        if (!selfClosing) {
            parseElement(html, pos, elementNode);
        }
    }
}