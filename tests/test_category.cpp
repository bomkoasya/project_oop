#include "gtest/gtest.h"
#include "../Category.h"

const char* kParentId = "parent";

TEST(CategoryTest, GIVEN_ParentIdSet_WHEN_IsSubcategoryCalled_THEN_ReturnsTrue) {
    Category cat;
    cat.parentId = kParentId;
    EXPECT_TRUE(cat.isSubcategory());
}

TEST(CategoryTest, GIVEN_EmptyParentId_WHEN_IsSubcategoryCalled_THEN_ReturnsFalse) {
    Category cat;
    cat.parentId = "";
    EXPECT_FALSE(cat.isSubcategory());
}

TEST(CategoryTest, GIVEN_UninitializedParentId_WHEN_IsSubcategoryCalled_THEN_ReturnsFalse) {
    Category cat;
    cat.parentId = "";
    EXPECT_FALSE(cat.isSubcategory());
}

TEST(CategoryTest, GIVEN_WhitespaceParentId_WHEN_IsSubcategoryCalled_THEN_TreatedAsEmpty) {
    Category cat;
    cat.parentId = "   ";
    EXPECT_FALSE(cat.isSubcategory());
}

TEST(CategoryTest, GIVEN_SpecialParentIdZeroString_WHEN_IsSubcategoryCalled_THEN_DefinedBehavior) {
    Category cat;
    cat.parentId = "0";
    EXPECT_TRUE(cat.isSubcategory());
}
