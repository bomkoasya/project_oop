#pragma once
#include <string>


class Category {
public:
    std::string id;
    std::string name;
    std::string parentId;
    std::string budgetId;


    // Methods
    bool isSubcategory() const { return !parentId.empty(); }
};