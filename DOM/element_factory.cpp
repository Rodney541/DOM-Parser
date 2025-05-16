// element_factory.cpp
#include "element_factory.h"

// Initialize static member
std::map<std::string, std::shared_ptr<ElementNode>> ElementFactory::elementPool;