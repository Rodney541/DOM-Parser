#include "dom_util.h"
#include <iostream>
#include <map>
#include <sstream>
#include <queue>

// Initialize static members for caching
std::map<std::string, ElementNode*> DOMUtil::idCache;
std::map<std::string, std::vector<ElementNode*>> DOMUtil::tagCache;
std::map<std::string, std::vector<ElementNode*>> DOMUtil::classCache;
bool DOMUtil::cacheBuilt = false;

void DOMUtil::depthFirstTraversal(Node* node, int depth) {
    if (!node) return;
    std::string indent(depth * 2, ' ');
    if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
        std::cout << indent << "Element: " << elementNode->getTagName() << std::endl;
        // Print attributes if present
        std::string attrValue;
        for (const auto& attrName : { "id", "class", "href", "src" }) {
            attrValue = elementNode->getAttribute(attrName);
            if (!attrValue.empty()) {
                std::cout << indent << "  Attribute: " << attrName << "=\"" << attrValue << "\"" << std::endl;
            }
        }
    }
    else if (auto textNode = dynamic_cast<TextNode*>(node)) {
        std::string content = textNode->getContent();
        // Trim and truncate long text content for display
        if (content.length() > 50) {
            content = content.substr(0, 47) + "...";
        }
        std::cout << indent << "Text: " << content << std::endl;
    }
    else if (auto cdataNode = dynamic_cast<CDATANode*>(node)) {
        std::cout << indent << "CDATA: " << cdataNode->toString() << std::endl;
    }
    for (const auto& child : node->getChildren()) depthFirstTraversal(child, depth + 1);
}

void DOMUtil::breadthFirstTraversal(Node* root, int maxDepth) {
    if (!root) return;

    // Queue for BFS: pairs of (node, depth)
    std::queue<std::pair<Node*, int>> nodeQueue;
    nodeQueue.push({ root, 0 });

    while (!nodeQueue.empty()) {
        auto& [currentNode, depth] = nodeQueue.front();
         nodeQueue.pop();

        // Stop if we've reached the maximum depth
        if (maxDepth >= 0 && depth > maxDepth) {
            continue;
        }

        // Process the current node (similar to depthFirstTraversal)
        std::string indent(depth * 2, ' ');
        if (auto elementNode = dynamic_cast<ElementNode*>(currentNode)) {
            std::cout << indent << "Element: " << elementNode->getTagName() << std::endl;
            // Print attributes if present
            std::string attrValue;
            for (const auto& attrName : { "id", "class", "href", "src" }) {
                attrValue = elementNode->getAttribute(attrName);
                if (!attrValue.empty()) {
                    std::cout << indent << "  Attribute: " << attrName << "=\"" << attrValue << "\"" << std::endl;
                }
            }
        }
        else if (auto textNode = dynamic_cast<TextNode*>(currentNode)) {
            std::string content = textNode->getContent();
            // Trim and truncate long text content for display
            if (content.length() > 50) {
                content = content.substr(0, 47) + "...";
            }
            std::cout << indent << "Text: " << content << std::endl;
        }
        else if (auto cdataNode = dynamic_cast<CDATANode*>(currentNode)) {
            std::cout << indent << "CDATA: " << cdataNode->toString() << std::endl;
        }

        // Add all children to the queue
        for (const auto& child : currentNode->getChildren()) {
            nodeQueue.push({ child, depth + 1 });
        }
    }
}

std::vector<ElementNode*> DOMUtil::getElementsByTagName(Node* root, const std::string& tagName) {
    std::vector<ElementNode*> result;
    collectElementsByTagName(root, tagName, result);
    return result;
}

ElementNode* DOMUtil::getElementById(Node* root, const std::string& id) {
    if (!root) return nullptr;
    if (auto element = dynamic_cast<ElementNode*>(root)) {
        if (element->getAttribute("id") == id) return element;
    }
    for (auto child : root->getChildren()) {
        if (auto found = getElementById(child, id)) return found;
    }
    return nullptr;
}

std::vector<ElementNode*> DOMUtil::querySelectorAll(Node* root, const std::string& selector) {
    std::vector<ElementNode*> result;

    // Simple query selector implementation (you can expand it based on your requirements)
    std::function<void(Node*)> traverse = [&traverse, &result, &selector](Node* node) {
        if (!node) return;

        if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
            // Match the selector (you can improve this with more advanced matching)
            if (elementNode->matchesSelector(selector)) {
                result.push_back(elementNode);
            }
        }

        // Recursively traverse children
        for (auto& child : node->getChildren()) {
            traverse(child);
        }
        };

    traverse(root);  // Start traversal from the root node
    return result;
}
void DOMUtil::collectElementsByTagName(Node* node, const std::string& tagName, std::vector<ElementNode*>& result) {
    if (!node) return;
    if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
        if (elementNode->getTagName() == tagName) result.push_back(elementNode);
    }
    for (const auto& child : node->getChildren()) {
        collectElementsByTagName(child, tagName, result);
    }
}

// Cache building methods
void DOMUtil::buildCache(Node* root) {
    // Clear existing caches
    idCache.clear();
    tagCache.clear();
    classCache.clear();

    // Build new caches
    traverseAndBuildCache(root);
    cacheBuilt = true;
}

void DOMUtil::traverseAndBuildCache(Node* node) {
    if (!node) return;

    if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
        // Cache by tag name
        std::string tagName = elementNode->getTagName();
        tagCache[tagName].push_back(elementNode);

        // Cache by ID (if present)
        std::string id = elementNode->getAttribute("id");
        if (!id.empty()) {
            idCache[id] = elementNode;
        }

        // Cache by class names
        std::string classAttr = elementNode->getAttribute("class");
        if (!classAttr.empty()) {
            std::istringstream iss(classAttr);
            std::string className;
            while (iss >> className) {
                if (!className.empty()) {
                    classCache[className].push_back(elementNode);
                }
            }
        }
    }

    // Recurse to children
    for (const auto& child : node->getChildren()) {
        traverseAndBuildCache(child);
    }
}

void DOMUtil::clearCache() {
    idCache.clear();
    tagCache.clear();
    classCache.clear();
    cacheBuilt = false;
}

// Fast lookup methods using cache
ElementNode* DOMUtil::getElementByIdFast(const std::string& id) {
    if (!cacheBuilt) {
        std::cerr << "Warning: Cache not built. Call buildCache() first." << std::endl;
        return nullptr;
    }

    auto it = idCache.find(id);
    if (it != idCache.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<ElementNode*> DOMUtil::getElementsByTagNameFast(const std::string& tagName) {
    if (!cacheBuilt) {
        std::cerr << "Warning: Cache not built. Call buildCache() first." << std::endl;
        return {};
    }

    auto it = tagCache.find(tagName);
    if (it != tagCache.end()) {
        return it->second;
    }
    return {};
}

std::vector<ElementNode*> DOMUtil::getElementsByClassNameFast(const std::string& className) {
    if (!cacheBuilt) {
        std::cerr << "Warning: Cache not built. Call buildCache() first." << std::endl;
        return {};
    }

    auto it = classCache.find(className);
    if (it != classCache.end()) {
        return it->second;
    }
    return {};
}