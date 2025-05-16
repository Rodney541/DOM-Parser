#pragma once
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>

class InputHandler {
public:
    // Get a number from user within a range
    static int getNumberInput(int min, int max, const std::string& prompt);

    // Get a line of text from user
    static std::string getLineInput(const std::string& prompt);

    // Get multi-line text input from user
    static std::string getMultiLineInput(const std::string& prompt,
        const std::string& endMarker = "END");

    // Read HTML from a file
    static std::string readFromFile(const std::string& filename);

    // Get a filename from user and read its contents
    static std::string getFileInput();
};

#endif // INPUT_HANDLER_H