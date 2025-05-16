#include "input_handler.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>

int InputHandler::getNumberInput(int min, int max, const std::string& prompt) {
    int choice;
    while (true) {
        std::cout << prompt;
        if (std::cin >> choice) {
            if (choice >= min && choice <= max) {
                break;
            }
            else {
                std::cout << "Please enter a number between " << min << " and " << max << ".\n";
            }
        }
        else {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::string InputHandler::getLineInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

std::string InputHandler::getMultiLineInput(const std::string& prompt, const std::string& endMarker) {
    std::cout << prompt << std::endl;
    std::cout << "Type '" << endMarker << "' on a new line when finished:" << std::endl;

    std::string result;
    std::string line;

    while (std::getline(std::cin, line) && line != endMarker) {
        result += line + "\n";
    }

    return result;
}

std::string InputHandler::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string InputHandler::getFileInput() {
    std::string filename = getLineInput("Enter filename: ");
    std::string content = readFromFile(filename);
    if (content.empty()) {
        std::cout << "Could not read file or file is empty.\n";
    }
    return content;
}