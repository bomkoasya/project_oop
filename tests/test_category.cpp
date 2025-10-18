#include "gtest/gtest.h"
#include "../Category.h"

TEST(CategoryTest, IsSubcategory) {
    Category cat;
    cat.parentId = "parent";
    EXPECT_TRUE(cat.isSubcategory());
}

TEST(CategoryTest, IsNotSubcategory) {
    Category cat;
    EXPECT_FALSE(cat.isSubcategory());
}

TEST(CategoryTest, IsNotSubcategoryWithEmptyParentId) {
    Category cat;
    cat.parentId = "";
    EXPECT_FALSE(cat.isSubcategory());
}