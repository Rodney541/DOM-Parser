#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <sstream>


#include "node.h"
#include "parser.h"
#include "dom_util.h"
#include "input_handler.h"
#include "html_templates.h"
#include "element_factory.h"
#include "event.h"
#include "event_dispatcher.h"
#include "selector.h"
#include "selector_matcher.h"
#include <chrono>

int main() {
    std::cout << "===== DOM Parser Interactive Demo =====\n";
    std::cout << "This program demonstrates parsing and manipulating HTML/XML documents.\n\n";

    std::string html;

    // Menu for data input method
    std::cout << "How would you like to input HTML?\n";
    std::cout << "1. From a file\n";
    std::cout << "2. Enter HTML manually\n";
    std::cout << "3. Use a template\n";
    std::cout << "4. Generate HTML with a wizard\n";

    int choice = InputHandler::getNumberInput(1, 4, "Enter your choice (1-4): ");

    switch (choice) {
    case 1: // From a file
        html = InputHandler::getFileInput();
        if (html.empty()) {
            std::cout << "Using default HTML instead.\n";
            html = HTMLTemplates::getBasicTemplate();
        }
        break;

    case 2: // Manual input
        html = InputHandler::getMultiLineInput("Enter your HTML content:");
        break;

    case 3: // Use a template
    {
        std::cout << "Choose a template:\n";
        std::cout << "1. Basic HTML document\n";
        std::cout << "2. News article\n";
        std::cout << "3. Contact form\n";

        int templateChoice = InputHandler::getNumberInput(1, 3, "Enter your choice (1-3): ");

        switch (templateChoice) {
        case 1:
            html = HTMLTemplates::getBasicTemplate();
            break;
        case 2:
            html = HTMLTemplates::getArticleTemplate();
            break;
        case 3:
            html = HTMLTemplates::getFormTemplate();
            break;
        }
    }
    break;

    case 4: // Generate with wizard
    {
        std::cout << "HTML Generation Wizard\n";
        std::cout << "-----------------------\n";

        std::string title = InputHandler::getLineInput("Enter page title: ");
        std::string headingText = InputHandler::getLineInput("Enter main heading: ");
        int paragraphCount = InputHandler::getNumberInput(1, 10, "How many paragraphs (1-10)? ");

        std::vector<std::string> paragraphs;
        for (int i = 0; i < paragraphCount; i++) {
            std::string paragraph = InputHandler::getLineInput("Enter paragraph " + std::to_string(i + 1) + ": ");
            paragraphs.push_back(paragraph);
        }

        bool addComment = InputHandler::getLineInput("Add a comment? (y/n): ") == "y";
        std::string commentText;
        if (addComment) {
            commentText = InputHandler::getLineInput("Enter comment text: ");
        }

        bool addCdata = InputHandler::getLineInput("Add CDATA section? (y/n): ") == "y";
        std::string cdataText;
        if (addCdata) {
            cdataText = InputHandler::getLineInput("Enter CDATA content: ");
        }

        // Build the HTML
        std::stringstream ss;
        ss << "<!DOCTYPE html>\n<html>\n<head>\n    <title>" << title << "</title>\n</head>\n<body>\n";
        ss << "    <h1>" << headingText << "</h1>\n";

        for (const auto& para : paragraphs) {
            ss << "    <p>" << para << "</p>\n";
        }

        if (addComment) {
            ss << "    <!-- " << commentText << " -->\n";
        }

        if (addCdata) {
            ss << "    <script><![CDATA[" << cdataText << "]]></script>\n";
        }

        ss << "</body>\n</html>";
        html = ss.str();

        std::cout << "\nGenerated HTML:\n";
        std::cout << "---------------\n";
        std::cout << html << "\n\n";
    }
    break;
    }

    // Parse the HTML
    std::cout << "\nParsing HTML...\n";
    Node* dom = DOMParser::parse(html);
    std::cout << "Parsing complete!\n\n";

    // Display menu for operations
    bool running = true;
    while (running) {
        std::cout << "\n===== DOM Operations =====\n";
        std::cout << "1. Print the full DOM as HTML\n";
        std::cout << "2. Print the DOM structure\n";
        std::cout << "3. Find elements by tag name\n";
        std::cout << "4. Find element by ID\n";
        std::cout << "5. Count elements\n";
        std::cout << "6. Print DOM statistics\n";
        std::cout << "7. Find elements with CSS selector\n";
        std::cout << "8. Register event listener\n";
        std::cout << "9. Trigger event\n";
        std::cout << "10. Breadth-first traversal\n";
        std::cout << "11. Generate performance report\n";
        std::cout << "12. Exit\n";

        choice = InputHandler::getNumberInput(1, 12, "Enter your choice (1-12): ");

        switch (choice) {
        case 1: // Print full DOM
            std::cout << "\n=== Full DOM as HTML ===\n";
            std::cout << dom->toString() << std::endl;
            break;

        case 2: // Print DOM structure
            std::cout << "\n=== DOM Structure ===\n";
            DOMUtil::depthFirstTraversal(dom);
            break;

        case 3: // Find elements by tag name
        {
            std::string tagName = InputHandler::getLineInput("Enter tag name to search for: ");
            std::vector<ElementNode*> elements = DOMUtil::getElementsByTagName(dom, tagName);
            std::cout << "Found " << elements.size() << " " << tagName << " elements:\n";
            for (size_t i = 0; i < elements.size(); i++) {
                std::cout << "\n--- Element " << (i + 1) << " ---\n";
                std::cout << elements[i]->toString() << std::endl;
            }
        }
        break;

        case 4: // Find element by ID
        {
            std::string id = InputHandler::getLineInput("Enter ID to search for: ");
            ElementNode* element = DOMUtil::getElementById(dom, id);
            if (element) {
                std::cout << "Found element with ID '" << id << "':\n";
                std::cout << element->toString() << std::endl;
            }
            else {
                std::cout << "No element found with ID '" << id << "'\n";
            }
        }
        break;

        case 5: // Count elements
        {
            std::function<void(Node*, std::map<std::string, int>&)> countNodes =
                [&countNodes](Node* node, std::map<std::string, int>& counts) {
                if (!node) return;
                if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
                    counts[elementNode->getTagName()]++;
                }
                else if (dynamic_cast<TextNode*>(node)) {
                    counts["#text"]++;
                }
                else if (dynamic_cast<CDATANode*>(node)) {
                    counts["#cdata"]++;
                }
                for (const auto& child : node->getChildren()) {
                    countNodes(child, counts);
                }
                };

            std::map<std::string, int> counts;
            countNodes(dom, counts);

            std::cout << "\n=== Element Count ===\n";
            for (const auto& pair : counts) {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }
        }
        break;

        case 6: // DOM statistics
        {
            std::function<void(Node*, int&, int&, int&, int&, int&)> countStats =
                [&countStats](Node* node, int& elements, int& texts, int& cdatas, int& depth, int& currDepth) {
                if (!node) return;
                if (dynamic_cast<ElementNode*>(node)) elements++;
                else if (dynamic_cast<TextNode*>(node)) texts++;
                else if (dynamic_cast<CDATANode*>(node)) cdatas++;

                currDepth++;
                depth = std::max(depth, currDepth);

                for (const auto& child : node->getChildren()) {
                    countStats(child, elements, texts, cdatas, depth, currDepth);
                }

                currDepth--;
                };

            int elements = 0, texts = 0, cdatas = 0, maxDepth = 0, currentDepth = 0;
            countStats(dom, elements, texts, cdatas, maxDepth, currentDepth);

            std::cout << "\n=== DOM Statistics ===\n";
            std::cout << "Total Elements: " << elements << std::endl;
            std::cout << "Text Nodes: " << texts << std::endl;
            std::cout << "CDATA Sections: " << cdatas << std::endl;
            std::cout << "Total Nodes: " << (elements + texts + cdatas) << std::endl;
            std::cout << "Maximum Nesting Depth: " << maxDepth << std::endl;
        }
        break;

        case 7: // Find elements with CSS selector
        {
            std::string selector = InputHandler::getLineInput("Enter CSS selector: ");
            std::vector<ElementNode*> elements = DOMUtil::querySelectorAll(dom, selector);

            std::cout << "Found " << elements.size() << " elements matching '" << selector << "':\n";
            for (size_t i = 0; i < elements.size(); i++) {
                std::cout << "\n--- Element " << (i + 1) << " ---\n";
                std::cout << elements[i]->toString() << std::endl;
            }
        }
        break;

        case 8: // Register event listener
        {
            std::string elementId = InputHandler::getLineInput("Enter ID of element to attach listener to: ");
            ElementNode* element = DOMUtil::getElementById(dom, elementId);
            if (!element) {
                std::cout << "Element with ID '" << elementId << "' not found.\n";
                break;
            }

            std::string eventType = InputHandler::getLineInput("Enter event type (click, mouseover, etc.): ");
            element->addEventListener(eventType, [elementId, eventType](Event* event) {
                std::cout << "Event '" << eventType << "' triggered on element with ID '" << elementId << "'\n";
                });

            std::cout << "Event listener registered successfully.\n";
        }
        break;

        case 9: // Trigger event
        {
            std::string elementId = InputHandler::getLineInput("Enter ID of element to trigger event on: ");
            ElementNode* element = DOMUtil::getElementById(dom, elementId);
            if (!element) {
                std::cout << "Element with ID '" << elementId << "' not found.\n";
                break;
            }

            std::string eventType = InputHandler::getLineInput("Enter event type to trigger: ");
            Event* event = new Event(eventType);
            bool canceled = !element->dispatchEvent(event);

            std::cout << "Event dispatched successfully.\n";
            if (canceled) {
                std::cout << "Event was canceled by a listener.\n";
            }

            delete event;
        }
        break;

        case 10: // Breadth-first traversal
            std::cout << "\n=== Breadth-First DOM Traversal ===\n";
            DOMUtil::breadthFirstTraversal(dom);
            break;

        case 11: // Performance report
        {
            std::cout << "\n=== Performance Report ===\n";

            auto start = std::chrono::high_resolution_clock::now();
            DOMUtil::buildCache(dom);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> cacheBuildTime = end - start;
            std::cout << "Time to build index cache: " << cacheBuildTime.count() << " ms\n";

            std::string testId = "test";
            start = std::chrono::high_resolution_clock::now();
            ElementNode* element = DOMUtil::getElementById(dom, testId);
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> getByIdTime = end - start;
            std::cout << "Time for getElementById: " << getByIdTime.count() << " ms\n";

            std::string testTag = "div";
            start = std::chrono::high_resolution_clock::now();
            std::vector<ElementNode*> elements = DOMUtil::getElementsByTagName(dom, testTag);
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> getByTagTime = end - start;
            std::cout << "Time for getElementsByTagName: " << getByTagTime.count() << " ms\n";

            std::string testSelector = "div.container > p";
            start = std::chrono::high_resolution_clock::now();
            element = DOMUtil::querySelector(dom, testSelector);
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> querySelectorTime = end - start;
            std::cout << "Time for querySelector: " << querySelectorTime.count() << " ms\n";

            size_t nodeCount = 0;
            std::function<void(Node*)> countNodes = [&countNodes, &nodeCount](Node* node) {
                if (!node) return;
                nodeCount++;
                for (const auto& child : node->getChildren()) {
                    countNodes(child);
                }
                };
            countNodes(dom);
            size_t estimatedMemory = nodeCount * sizeof(Node);
            std::cout << "Approximate memory usage: " << (estimatedMemory / 1024.0) << " KB\n";
        }
        break;

        case 12: // Exit
            std::cout << "Exiting program...\n";
            running = false;
            break;
        }

        if (running) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }

    }


    // Clean up
    delete dom;
    std::cout << "Memory cleaned up. Goodbye!\n";
    return 0;
}