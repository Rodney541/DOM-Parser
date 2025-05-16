#include "Selector.h"
#include <sstream>
#include <iostream>
#include <cctype>

// Constructor
Selector::Selector(const std::string& selectorStr) {
    parts = SelectorParser::tokenize(selectorStr);  // Use the public tokenize function
}

// Debugging helper to convert the selector to a string for easy viewing
std::string Selector::toString() const {
    std::string result;
    for (const auto& part : parts) {
        result += part.value + " ";
    }
    return result;
}

// Implement the `SelectorParser` static methods

SelectorPart SelectorParser::parseSimpleSelector(const std::string& selector) {
    SelectorPart part;

    // Check if it's a tag
    if (selector[0] != '.' && selector[0] != '#' && selector[0] != '[') {
        part.type = SelectorType::TAG;
        part.value = selector;
    }
    // Check if it's an ID (starts with #)
    else if (selector[0] == '#') {
        part.type = SelectorType::ID;
        part.value = selector.substr(1);
    }
    // Check if it's a class (starts with .)
    else if (selector[0] == '.') {
        part.type = SelectorType::CLASS;
        part.value = selector.substr(1);
    }
    // Attribute selector (starts with [)
    else if (selector[0] == '[') {
        part.type = SelectorType::ATTRIBUTE;
        size_t pos = selector.find('=');
        if (pos != std::string::npos) {
            part.attributeName = selector.substr(1, pos - 1);
            part.attributeValue = selector.substr(pos + 1, selector.length() - pos - 2); // Remove closing bracket
            part.attributeOperator = "=";
        }
        else {
            part.value = selector.substr(1, selector.length() - 2); // Remove brackets
        }
    }

    return part;
}

std::vector<SelectorPart> SelectorParser::tokenize(const std::string& selector) {
    std::vector<SelectorPart> parts;
    std::stringstream ss(selector);
    std::string token;

    while (ss >> token) {
        parts.push_back(parseSimpleSelector(token));
    }

    return parts;
}

// Define the `parse` method that uses the `tokenize` method
Selector SelectorParser::parse(const std::string& selectorStr) {
    // Tokenize the selector string
    std::vector<SelectorPart> parts = tokenize(selectorStr);

    // Return the Selector object
    return Selector(selectorStr);  // Or you could directly construct the Selector with the tokenized parts
}

// Selector matching (you can expand this later)
bool Selector::match(const ElementNode* element) const {
    for (const auto& part : parts) {
        switch (part.type) {
        case SelectorType::TAG:
            if (element->getTagName() != part.value) return false;
            break;
        case SelectorType::ID:
            if (element->getAttribute("id") != part.value) return false;
            break;
        case SelectorType::CLASS:
            if (element->getAttribute("class") != part.value) return false;
            break;
        case SelectorType::ATTRIBUTE:
            if (element->getAttribute(part.attributeName) != part.attributeValue) return false;
            break;
        default:
            break;
        }
    }
    return true;
}
