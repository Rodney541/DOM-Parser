#pragma once
#include <string>

// Forward declaration of ElementNode to avoid circular dependencies
class ElementNode;

enum class EventPhase {
    NONE,
    CAPTURING_PHASE,
    AT_TARGET,
    BUBBLING_PHASE
};

class Event {
protected:
    std::string type;                       // The type of the event (e.g., "click")
    ElementNode* target;                    // The original target of the event
    ElementNode* currentTarget;             // The current target during event propagation
    EventPhase phase;                       // The current phase of event propagation
    bool bubbles;                           // Whether the event bubbles up through the DOM tree
    bool cancelable;                        // Whether the event's default action can be prevented
    bool defaultPrevented;                  // Whether the default action has been prevented
    bool stopPropagationFlag;               // Flag to stop event propagation to subsequent nodes
    bool stopImmediatePropagationFlag;      // Flag to stop propagation and further listeners on the current node

public:
    // Constructor to initialize an Event object
    Event(const std::string& eventType, bool canBubble = true, bool canCancel = true);

    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~Event() = default;

    // Getters
    std::string getType() const { return type; }
    ElementNode* getTarget() const { return target; }
    ElementNode* getCurrentTarget() const { return currentTarget; }
    EventPhase getPhase() const { return phase; }
    bool getBubbles() const { return bubbles; }
    bool getCancelable() const { return cancelable; }

    // Event control methods
    void preventDefault();                         // Prevent the default action if the event is cancelable
    bool isDefaultPrevented() const { return defaultPrevented; }
    void stopPropagation();                        // Stop propagation to subsequent nodes
    void stopImmediatePropagation();               // Stop propagation and further listeners on the current node

    // For internal use by event dispatcher
    void setTarget(ElementNode* node) { target = node; }
    void setCurrentTarget(ElementNode* node) { currentTarget = node; }
    void setPhase(EventPhase p) { phase = p; }
    bool shouldStopPropagation() const { return stopPropagationFlag; }
    bool shouldStopImmediatePropagation() const { return stopImmediatePropagationFlag; }

    // Reset propagation flags (useful if reusing the event object)
    void resetFlags() {
        stopPropagationFlag = false;
        stopImmediatePropagationFlag = false;
    }
};

class MouseEvent : public Event {
private:
    int clientX;       // X-coordinate of the mouse event
    int clientY;       // Y-coordinate of the mouse event
    bool ctrlKey;      // Whether the Ctrl key was pressed
    bool altKey;       // Whether the Alt key was pressed
    bool shiftKey;     // Whether the Shift key was pressed

public:
    // Constructor for MouseEvent
    MouseEvent(const std::string& type, int x, int y, bool ctrl = false, bool alt = false, bool shift = false);

    // Getters
    int getClientX() const { return clientX; }
    int getClientY() const { return clientY; }
    bool getCtrlKey() const { return ctrlKey; }
    bool getAltKey() const { return altKey; }
    bool getShiftKey() const { return shiftKey; }
};