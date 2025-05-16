// selector_matcher.h
#pragma once
#include "selector.h"
#include "node.h"
#include <vector>

class SelectorMatcher {
public:
    static bool matches(Node* node, const Selector& selector);
    static std::vector<ElementNode*> findElements(Node* root, const std::string& selectorStr);

private:
    static bool matchesPart(Node* node, const SelectorPart& part);
    static bool matchesCompoundSelector(Node* node, const std::vector<SelectorPart>& parts, size_t index);
};