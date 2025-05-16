#pragma once
#include "event.h"
#include "node.h"
#include <vector>

class EventDispatcher {
public:
    static bool dispatchEvent(Node* node, Event* event) {
        if (!node || !event) return false;

        // Set target
        ElementNode* target = dynamic_cast<ElementNode*>(node);
        if (!target) return false;
        event->setTarget(target);

        // Build propagation path (for capturing and bubbling)
        std::vector<ElementNode*> path;
        Node* current = node;
        while (current) {
            if (auto element = dynamic_cast<ElementNode*>(current)) {
                path.insert(path.begin(), element);
            }
            current = current->getParent();
        }

        // Capturing phase (top-down)
        event->setPhase(EventPhase::CAPTURING_PHASE);
        for (size_t i = 0; i < path.size() - 1; i++) {
            event->setCurrentTarget(path[i]);
            invokeListeners(path[i], event);
            if (event->shouldStopPropagation()) break;
        }

        // At target
        if (!event->shouldStopPropagation()) {
            event->setPhase(EventPhase::AT_TARGET);
            event->setCurrentTarget(target);
            invokeListeners(target, event);
        }

        // Bubbling phase (if event bubbles)
        if (event->getBubbles() && !event->shouldStopPropagation()) {
            event->setPhase(EventPhase::BUBBLING_PHASE);
            for (size_t i = path.size() - 2; i >= 0; i--) {
                event->setCurrentTarget(path[i]);
                invokeListeners(path[i], event);
                if (event->shouldStopPropagation()) break;
                if (i == 0) break;
            }
        }

        return !event->isDefaultPrevented();
    }

private:
    static void invokeListeners(ElementNode* node, Event* event) {
        if (!node) return;

        auto listeners = node->getListeners(event->getType());
        for (auto& listener : listeners) {
            listener(event);
            if (event->shouldStopImmediatePropagation()) break;
        }
    }
};