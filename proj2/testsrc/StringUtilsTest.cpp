#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest) {
    EXPECT_EQ(StringUtils::Slice("hello world", 0, 5), "hello");
    EXPECT_EQ(StringUtils::Slice("hello world", 6), "world");
    EXPECT_EQ(StringUtils::Slice("hello world", -5, -1), "worl");
}

TEST(StringUtilsTest, Capitalize) {
    EXPECT_EQ(StringUtils::Capitalize("hello"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize("HELLO"), "Hello");
}

TEST(StringUtilsTest, Upper) {
    EXPECT_EQ(StringUtils::Upper("hello"), "HELLO");
    EXPECT_EQ(StringUtils::Upper("Hello World"), "HELLO WORLD");
}

TEST(StringUtilsTest, Lower) {
    EXPECT_EQ(StringUtils::Lower("HELLO"), "hello");
    EXPECT_EQ(StringUtils::Lower("Hello World"), "hello world");
}

TEST(StringUtilsTest, LStrip) {
    EXPECT_EQ(StringUtils::LStrip("   hello"), "hello");
    EXPECT_EQ(StringUtils::LStrip("\t\n  hello"), "hello");
}

TEST(StringUtilsTest, RStrip) {
    EXPECT_EQ(StringUtils::RStrip("hello   "), "hello");
    EXPECT_EQ(StringUtils::RStrip("hello\t\n  "), "hello");
}

TEST(StringUtilsTest, Strip) {
    EXPECT_EQ(StringUtils::Strip("   hello   "), "hello");
    EXPECT_EQ(StringUtils::Strip("\t\n  hello\t\n  "), "hello");
}

TEST(StringUtilsTest, Center) {
    EXPECT_EQ(StringUtils::Center("hello", 11), "   hello   ");
    EXPECT_EQ(StringUtils::Center("hello", 10, '-'), "--hello---");
}

TEST(StringUtilsTest, LJust) {
    EXPECT_EQ(StringUtils::LJust("hello", 10), "hello     ");
    EXPECT_EQ(StringUtils::LJust("hello", 10, '-'), "hello-----");
}

TEST(StringUtilsTest, RJust) {
    EXPECT_EQ(StringUtils::RJust("hello", 10), "     hello");
    EXPECT_EQ(StringUtils::RJust("hello", 10, '-'), "-----hello");
}

TEST(StringUtilsTest, Replace) {
    EXPECT_EQ(StringUtils::Replace("hello world", "world", "everyone"), "hello everyone");
    EXPECT_EQ(StringUtils::Replace("hello hello", "hello", "hi"), "hi hi");
}

TEST(StringUtilsTest, Split) {
    std::vector<std::string> result = {"hello", "world"};
    EXPECT_EQ(StringUtils::Split("hello world", std::string(" ")), result);
EXPECT_EQ(StringUtils::Split("a,b,c", std::string(",")), std::vector<std::string>({"a", "b", "c"}));
}

TEST(StringUtilsTest, Join) {
    std::vector<std::string> input = {"hello", "world"};
    EXPECT_EQ(StringUtils::Join(" ", input), "hello world");
    EXPECT_EQ(StringUtils::Join("-", input), "hello-world");
}

TEST(StringUtilsTest, ExpandTabs) {
    EXPECT_EQ(StringUtils::ExpandTabs("\thello\tworld", 4), "    hello    world");
    EXPECT_EQ(StringUtils::ExpandTabs("a\tb\tc", 2), "a  b  c");
}

TEST(StringUtilsTest, EditDistance) {
    EXPECT_EQ(StringUtils::EditDistance("kitten", "sitting"), 3);
    EXPECT_EQ(StringUtils::EditDistance("flaw", "lawn"), 2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}