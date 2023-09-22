#include <gtest/gtest.h>
#include "fangorn/util/fangornutils.hpp"

class UnknownUnserializableClass { };

TEST(util_test, test_serialize_unknown) {
    UnknownUnserializableClass obj;
    EXPECT_THROW(serialize<UnknownUnserializableClass>(obj), std::runtime_error);
}

TEST(util_test, test_deserialize_unknown) {
    EXPECT_THROW(deserialize<UnknownUnserializableClass>("stuff"), std::runtime_error);
}

TEST(util_test, test_serialize_int) {
    EXPECT_EQ(serialize<int>(0), "0");
    EXPECT_EQ(serialize<int>(7), "7");
    EXPECT_EQ(serialize<int>(-23), "-23");
    EXPECT_EQ(serialize<int>(65535), "65535");
}

TEST(util_test, test_deserialize_int) {
    EXPECT_EQ(deserialize<int>("0"), 0);
    EXPECT_EQ(deserialize<int>("7"), 7);
    EXPECT_EQ(deserialize<int>("-23"), -23);
    EXPECT_EQ(deserialize<int>("65535"), 65535);
}

TEST(util_test, test_serialize_int_vector) {
    EXPECT_EQ(serialize<std::vector<int>>({}), "[]");
    EXPECT_EQ(serialize<std::vector<int>>({1}), "[1]");
    EXPECT_EQ(serialize<std::vector<int>>({1, 2, 3}), "[1,2,3]");
    EXPECT_EQ(serialize<std::vector<int>>({-4, 6, 8789, 3}), "[-4,6,8789,3]");
}

TEST(util_test, test_deserialize_int_vector) {
    EXPECT_EQ(deserialize<std::vector<int>>("[]"), std::vector<int>({}));
    EXPECT_EQ(deserialize<std::vector<int>>("[1]"), std::vector<int>({1}));
    EXPECT_EQ(deserialize<std::vector<int>>("[1, 2 , 3]"), std::vector<int>({1, 2, 3}));
    EXPECT_EQ(deserialize<std::vector<int>>("[1,\r\n4,\n70987878\r\n\n\n\r\n]"), std::vector<int>({1, 4, 70987878}));
    EXPECT_THROW(deserialize<std::vector<int>>("1, 2, 3]"), std::runtime_error);
    EXPECT_THROW(deserialize<std::vector<int>>("[1, 2, 3"), std::runtime_error);
}

