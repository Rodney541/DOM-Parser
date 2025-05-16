#pragma once
#ifndef HTML_TEMPLATES_H
#define HTML_TEMPLATES_H

#include <string>

class HTMLTemplates {
public:
    static std::string getBasicTemplate();
    static std::string getArticleTemplate();
    static std::string getFormTemplate();
};

#endif // HTML_TEMPLATES_H