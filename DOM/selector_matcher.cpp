#include "selector_matcher.h"
#include <iostream>

bool SelectorMatcher::matches(Node* node, const Selector& selector) {
    // Iterate over the parts of the selector and check if the node matches
    const auto& parts = selector.getParts();
    return matchesCompoundSelector(node, parts, 0);
}

bool SelectorMatcher::matchesCompoundSelector(Node* node, const std::vector<SelectorPart>& parts, size_t index) {
    if (index >= parts.size()) {
        // If we've matched all parts, the node matches the selector
        return true;
    }

    const SelectorPart& part = parts[index];

    // Check if the current node matches the part
    if (!matchesPart(node, part)) {
        return false;
    }

    // If the current part is matched, recursively check the next part
    return matchesCompoundSelector(node, parts, index + 1);
}

bool SelectorMatcher::matchesPart(Node* node, const SelectorPart& part) {
    // Match based on the type of selector part
    switch (part.type) {
    case SelectorType::TAG: {
        // Check if the node is an ElementNode and its tag matches the selector
        if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
            return elementNode->getTagName() == part.value;
        }
        break;
    }
    case SelectorType::ID: {
        if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
            return elementNode->getAttribute("id") == part.value;
        }
        break;
    }
    case SelectorType::CLASS: {
        if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
            return elementNode->getAttribute("class") == part.value;
        }
        break;
    }
    case SelectorType::ATTRIBUTE: {
        if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
            return elementNode->getAttribute(part.attributeName) == part.attributeValue;
        }
        break;
    }
    default:
        return false;
    }

    return false;
}

std::vector<ElementNode*> SelectorMatcher::findElements(Node* root, const std::string& selectorStr) {
    std::vector<ElementNode*> result;

    // Parse the selector string to create a Selector object
    Selector selector(selectorStr);

    // Traverse the DOM and find matching elements
    std::function<void(Node*)> traverse = [&traverse, &result, &selector](Node* node) {
        if (!node) return;

        if (matches(node, selector)) {
            if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
                result.push_back(elementNode);
            }
        }

        // Traverse the children of the node
        for (auto& child : node->getChildren()) {
            traverse(child);
        }
        };

    traverse(root);  // Start traversal from the root node

    return result;
}
