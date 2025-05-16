#pragma once
#ifndef NODE_H
#define NODE_H

#include <string>
#include <functional>
#include <vector>
#include <map>

// Forward declarations
class Event;
class EventDispatcher;

// Base Node class
class Node {
protected:
    Node* parent;
    std::vector<Node*> children;

public:
    Node();
    virtual ~Node();

    void appendChild(Node* child); // Renamed from addChild
    Node* getParent() const;
    const std::vector<Node*>& getChildren() const;

    virtual std::string toString() const = 0;
    virtual Node* clone() const = 0;
};

// TextNode
class TextNode : public Node {
private:
    std::string content;
public:
    TextNode(const std::string& text);
    std::string getContent() const;
    void setContent(const std::string& text);
    std::string toString() const override;
    Node* clone() const override;
};

// CDATANode
class CDATANode : public Node {
private:
    std::string content;
public:
    CDATANode(const std::string& data);
    std::string toString() const override;
    Node* clone() const override;
};

typedef std::function<void(Event*)> EventListener;

// ElementNode
class ElementNode : public Node {
private:
    std::string tagName;
    std::map<std::string, std::string> attributes;
    std::map<std::string, std::vector<EventListener>> eventListeners;
public:
    ElementNode(const std::string& tag);

    void setAttribute(const std::string& name, const std::string& value);
    std::string getAttribute(const std::string& name) const;
    std::string getTagName() const;

    std::string toString() const override;
    Node* clone() const override;

    void addEventListener(const std::string& type, EventListener listener) {
        eventListeners[type].push_back(listener);
    }

    void removeEventListener(const std::string& type, EventListener listener) {
        eventListeners[type].clear();
    }

    bool dispatchEvent(Event* event);

    std::vector<EventListener> getListeners(const std::string& type) const {
        if (eventListeners.find(type) != eventListeners.end()) {
            return eventListeners.at(type);
        }
        return std::vector<EventListener>();
    }

    bool matchesSelector(const std::string& selector) const {
        if (selector[0] == '#') {
            std::string id = selector.substr(1);
            return getAttribute("id") == id;
        }
        else if (selector[0] == '.') {
            std::string className = selector.substr(1);
            std::string classAttr = getAttribute("class");
            return classAttr.find(className) != std::string::npos;
        }
        else {
            return getTagName() == selector;
        }
    }
};

#endif // NODE_H