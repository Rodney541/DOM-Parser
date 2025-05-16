#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "node.h"
#include <string>
#include <map>
#include <utility>

class DOMParser {
private:
    static std::string trim(const std::string& str);
    static std::pair<std::string, std::map<std::string, std::string>> parseTag(const std::string& tag);

public:
    static Node* parse(const std::string& html);
    static void parseElement(const std::string& html, size_t& pos, Node* parent);
};

#endif // PARSER_H