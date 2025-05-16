#pragma once
#include <string>
#include <vector>
#include "node.h" 

enum class SelectorType {
    TAG,         // e.g., "div"
    ID,          // e.g., "#header"
    CLASS,       // e.g., ".container"
    ATTRIBUTE,   // e.g., "[href]" or "[href='value']"
    CHILD,       // e.g., "parent > child"
    DESCENDANT,  // e.g., "ancestor descendant"
    ADJACENT,    // e.g., "prev + next"
    SIBLING      // e.g., "prev ~ siblings"
};

struct SelectorPart {
    SelectorType type;
    std::string value;
    std::string attributeName;
    std::string attributeValue;
    std::string attributeOperator; // =, ^=, $=, *=, etc.
};

class Selector {
private:
    std::vector<SelectorPart> parts;

public:
    Selector(const std::string& selectorStr); // Constructor
    const std::vector<SelectorPart>& getParts() const { return parts; }  // Getter for parts

    bool match(const ElementNode* element) const;  // Match function

    // Debugging helper
    std::string toString() const;
};

// Implementation of the selector parser
class SelectorParser {
public:
    static Selector parse(const std::string& selectorStr);
    static std::vector<SelectorPart> tokenize(const std::string& selector);
private:
    static SelectorPart parseSimpleSelector(const std::string& selector);
     
};
