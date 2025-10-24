#pragma once
#include <string>

class Category {
public:
    std::string id;
    std::string name;
    std::string parentId;
    std::string budgetId;

    bool isSubcategory() const {
        try {
            for (unsigned char uc : parentId) {
                if (uc != ' ' && uc != '\t' && uc != '\n' &&
                    uc != '\r' && uc != '\v' && uc != '\f') {
                    return true;
                    }
            }
            return false;
        } catch (...) {

            return false;
        }
    }
};
