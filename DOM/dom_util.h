#pragma once
#ifndef DOM_UTIL_H
#define DOM_UTIL_H
#include "node.h"
#include <vector>
#include <string>
#include <map>
#include <queue>

class DOMUtil {
public:
    // Existing methods
    static void depthFirstTraversal(Node* node, int depth = 0);
    static std::vector<ElementNode*> getElementsByTagName(Node* root, const std::string& tagName);
    static ElementNode* getElementById(Node* root, const std::string& id);
    static std::vector<ElementNode*> querySelectorAll(Node* root, const std::string& selector);
    // Add querySelector method
    static ElementNode* querySelector(Node* root, const std::string& selector) {
        auto elements = querySelectorAll(root, selector);
        return elements.empty() ? nullptr : elements[0];
    }
    // New methods
    static void breadthFirstTraversal(Node* root, int maxDepth = -1);
    static void buildCache(Node* root);
    static void clearCache();
    static ElementNode* getElementByIdFast(const std::string& id);
    static std::vector<ElementNode*> getElementsByTagNameFast(const std::string& tagName);
    static std::vector<ElementNode*> getElementsByClassNameFast(const std::string& className);

private:
    static void collectElementsByTagName(Node* node, const std::string& tagName, std::vector<ElementNode*>& result);
    static std::map<std::string, ElementNode*> idCache;
    static std::map<std::string, std::vector<ElementNode*>> tagCache;
    static std::map<std::string, std::vector<ElementNode*>> classCache;
    static bool cacheBuilt;
    static void traverseAndBuildCache(Node* node);
};

#endif // DOM_UTIL_H