#pragma once
// element_factory.h
#pragma once
#include "node.h"
#include <string>
#include <map>
#include <memory>

class ElementFactory {
private:
    // Pool of reusable elements by tag
    static std::map<std::string, std::shared_ptr<ElementNode>> elementPool;

public:
    static ElementNode* getElement(const std::string& tagName) {
        // Check if we have a template element for this tag
        if (elementPool.find(tagName) == elementPool.end()) {
            // Create a new template element
            elementPool[tagName] = std::make_shared<ElementNode>(tagName);
        }

        // Clone the template element
        return new ElementNode(*elementPool[tagName]);
    }
};