#include "event.h"

// ===== Event Implementation =====
Event::Event(const std::string& eventType, bool canBubble, bool canCancel)
    : type(eventType),
    target(nullptr), // Assuming a null target initially
    currentTarget(nullptr), // Similarly, null for the current target initially
    phase(EventPhase::NONE), // Start with NONE phase
    bubbles(canBubble),
    cancelable(canCancel),
    defaultPrevented(false),
    stopPropagationFlag(false), // Initialize the flag here
    stopImmediatePropagationFlag(false) // Initialize the flag here
{
}

void Event::preventDefault() {
    if (cancelable) {
        defaultPrevented = true;
    }
}

// ===== MouseEvent Implementation =====
MouseEvent::MouseEvent(const std::string& type, int x, int y, bool ctrl, bool alt, bool shift)
    : Event(type), clientX(x), clientY(y), ctrlKey(ctrl), altKey(alt), shiftKey(shift) {
}
