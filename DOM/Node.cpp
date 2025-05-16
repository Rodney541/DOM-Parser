#include "node.h"
#include "event.h"
#include <iostream> // Fixed include syntax

// Base Node methods
Node::Node() : parent(nullptr) {}

Node::~Node() {
    for (auto child : children) delete child;
}

void Node::appendChild(Node* child) { // Renamed from addChild
    children.push_back(child);
    child->parent = this;
}

Node* Node::getParent() const {
    return parent;
}

const std::vector<Node*>& Node::getChildren() const {
    return children;
}

// TextNode methods
TextNode::TextNode(const std::string& text) : content(text) {}

std::string TextNode::getContent() const {
    return content;
}

void TextNode::setContent(const std::string& text) {
    content = text;
}

std::string TextNode::toString() const {
    return content;
}

Node* TextNode::clone() const {
    return new TextNode(content);
}

// CDATANode methods
CDATANode::CDATANode(const std::string& data) : content(data) {}

std::string CDATANode::toString() const {
    return "<![CDATA[" + content + "]]>";
}

Node* CDATANode::clone() const {
    return new CDATANode(content);
}

// ElementNode methods
ElementNode::ElementNode(const std::string& tag) : tagName(tag) {}

void ElementNode::setAttribute(const std::string& name, const std::string& value) {
    attributes[name] = value;
}

std::string ElementNode::getAttribute(const std::string& name) const {
    auto it = attributes.find(name);
    return (it != attributes.end()) ? it->second : "";
}

std::string ElementNode::getTagName() const {
    return tagName;
}

std::string ElementNode::toString() const {
    std::string result = "<" + tagName;
    for (const auto& attr : attributes) {
        result += " " + attr.first + "=\"" + attr.second + "\"";
    }
    if (children.empty()) result += "/>";
    else {
        result += ">";
        for (const auto& child : children) result += child->toString();
        result += "</" + tagName + ">";
    }
    return result;
}

Node* ElementNode::clone() const {
    ElementNode* copy = new ElementNode(tagName);
    for (const auto& attr : attributes) copy->setAttribute(attr.first, attr.second);
    for (const auto& child : children) copy->appendChild(child->clone()); // Updated to appendChild
    return copy;
}

bool ElementNode::dispatchEvent(Event* event) {
    std::cout << "Event '" << event->getType() << "' dispatched on element <" << tagName << ">\n";
    auto listeners = getListeners(event->getType());
    for (auto& listener : listeners) {
        listener(event);
    }
    return !event->isDefaultPrevented();
}