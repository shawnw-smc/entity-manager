#include "Utils.hpp"

#include <boost/container/flat_map.hpp>
#include <nlohmann/json.hpp>

#include <regex>
#include <string>
#include <variant>

#include "gtest/gtest.h"

using namespace std::string_literals;

TEST(TemplateCharReplace, replaceOneInt)
{
    nlohmann::json j = {{"foo", "$bus"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["BUS"] = 23;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = 23;
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, replaceOneStr)
{
    nlohmann::json j = {{"foo", "$TEST"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = std::string("Test");

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "Test";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, replaceSecondStr)
{
    nlohmann::json j = {{"foo", "the $TEST"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = std::string("Test");

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "the Test";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, replaceMiddleStr)
{
    nlohmann::json j = {{"foo", "the $TEST worked"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = std::string("Test");

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "the Test worked";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, replaceLastStr)
{
    nlohmann::json j = {{"foo", "the Test $TEST"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = 23;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "the Test 23";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, increment)
{
    nlohmann::json j = {{"foo", "3 plus 1 equals $TEST + 1"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = 3;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "3 plus 1 equals 4";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, decrement)
{
    nlohmann::json j = {{"foo", "3 minus 1 equals $TEST - 1 !"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = 3;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "3 minus 1 equals 2 !";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, modulus)
{
    nlohmann::json j = {{"foo", "3 mod 2 equals $TEST % 2"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = 3;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "3 mod 2 equals 1";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, multiply)
{
    nlohmann::json j = {{"foo", "3 * 2 equals $TEST * 2"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = 3;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "3 * 2 equals 6";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, divide)
{
    nlohmann::json j = {{"foo", "4 / 2 equals $TEST / 2"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = 4;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "4 / 2 equals 2";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, multiMath)
{
    nlohmann::json j = {{"foo", "4 * 2 % 6 equals $TEST * 2 % 6"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = 4;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "4 * 2 % 6 equals 2";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, twoReplacements)
{
    nlohmann::json j = {{"foo", "$FOO $BAR"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["FOO"] = std::string("foo");
    data["BAR"] = std::string("bar");

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "foo bar";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, twoReplacementsWithMath)
{
    nlohmann::json j = {{"foo", "4 / 2 equals $TEST / 2 $BAR"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["TEST"] = 4;
    data["BAR"] = std::string("bar");

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "4 / 2 equals 2 bar";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, hexAndWrongCase)
{
    nlohmann::json j = {{"Address", "0x54"},
                        {"Bus", 15},
                        {"Name", "$bus sensor 0"},
                        {"Type", "SomeType"}};

    boost::container::flat_map<std::string, BasicVariantType> data;
    data["BUS"] = 15;

    for (auto it = j.begin(); it != j.end(); it++)
    {
        templateCharReplace(it, data, 0);
    }
    nlohmann::json expected = {{"Address", 84},
                               {"Bus", 15},
                               {"Name", "15 sensor 0"},
                               {"Type", "SomeType"}};
    EXPECT_EQ(expected, j);
}

TEST(TemplateCharReplace, replaceSecondAsInt)
{
    nlohmann::json j = {{"foo", "twelve is $TEST"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;
    data["test"] = 12;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = "twelve is 12";
    EXPECT_EQ(expected, j["foo"]);
}

TEST(TemplateCharReplace, singleHex)
{
    nlohmann::json j = {{"foo", "0x54"}};
    auto it = j.begin();
    boost::container::flat_map<std::string, BasicVariantType> data;

    templateCharReplace(it, data, 0);

    nlohmann::json expected = 84;
    EXPECT_EQ(expected, j["foo"]);
}

TEST(MatchProbe, stringEqString)
{
    nlohmann::json j = R"("foo")"_json;
    BasicVariantType v = "foo"s;
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringRegexEqString)
{
    nlohmann::json j = R"("foo*")"_json;
    BasicVariantType v = "foobar"s;
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringNeqString)
{
    nlohmann::json j = R"("foobar")"_json;
    BasicVariantType v = "foo"s;
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringRegexError)
{
    nlohmann::json j = R"("foo[")"_json;
    BasicVariantType v = "foobar"s;
    EXPECT_THROW(matchProbe(j, v), std::regex_error);
}

TEST(MatchProbe, stringZeroEqFalse)
{
    nlohmann::json j = R"("0")"_json;
    BasicVariantType v = false;
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringOneEqTrue)
{
    nlohmann::json j = R"("1")"_json;
    BasicVariantType v = true;
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringElevenNeqTrue)
{
    nlohmann::json j = R"("11")"_json;
    BasicVariantType v = true;
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringFalseNeqFalse)
{
    nlohmann::json j = R"("false")"_json;
    BasicVariantType v = false;
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringTrueNeqTrue)
{
    nlohmann::json j = R"("true")"_json;
    BasicVariantType v = true;
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringFalseNeqTrue)
{
    nlohmann::json j = R"("false")"_json;
    BasicVariantType v = true;
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringEqUint8)
{
    nlohmann::json j = R"("255")"_json;
    BasicVariantType v = uint8_t(255);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringNeqUint8Overflow)
{
    nlohmann::json j = R"("65535")"_json;
    BasicVariantType v = uint8_t(255);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringFalseNeqUint8Zero)
{
    nlohmann::json j = R"("false")"_json;
    BasicVariantType v = uint8_t(0);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringTrueNeqUint8Zero)
{
    nlohmann::json j = R"("true")"_json;
    BasicVariantType v = uint8_t(1);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringEqUint32)
{
    nlohmann::json j = R"("11")"_json;
    BasicVariantType v = uint32_t(11);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringNeqUint32)
{
    nlohmann::json j = R"("12")"_json;
    BasicVariantType v = uint32_t(11);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringEqInt32)
{
    nlohmann::json j = R"("-11")"_json;
    BasicVariantType v = int32_t(-11);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringNeqInt32)
{
    nlohmann::json j = R"("-12")"_json;
    BasicVariantType v = int32_t(-11);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringRegexEqInt32)
{
    nlohmann::json j = R"("1*4")"_json;
    BasicVariantType v = int32_t(124);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringNeqUint64)
{
    nlohmann::json j = R"("foo")"_json;
    BasicVariantType v = uint64_t(65535);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringEqDouble)
{
    nlohmann::json j = R"("123.4")"_json;
    BasicVariantType v = double(123.4);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, stringNeqDouble)
{
    nlohmann::json j = R"("-123.4")"_json;
    BasicVariantType v = double(123.4);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, stringNeqEmpty)
{
    nlohmann::json j = R"("-123.4")"_json;
    BasicVariantType v;
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, boolStringError)
{
    nlohmann::json j = R"(false)"_json;
    BasicVariantType v = "false"s;
    EXPECT_THROW(matchProbe(j, v), std::invalid_argument);
}

TEST(MatchProbe, trueEqTrue)
{
    nlohmann::json j = R"(true)"_json;
    BasicVariantType v = true;
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, falseEqFalse)
{
    nlohmann::json j = R"(false)"_json;
    BasicVariantType v = false;
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, trueNeqFalse)
{
    nlohmann::json j = R"(true)"_json;
    BasicVariantType v = false;
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, trueNeqInt32Zero)
{
    nlohmann::json j = R"(true)"_json;
    BasicVariantType v = int32_t(0);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, trueNeqInt32NegativeOne)
{
    nlohmann::json j = R"(true)"_json;
    BasicVariantType v = int32_t(-1);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, falseNeqUint32One)
{
    nlohmann::json j = R"(false)"_json;
    BasicVariantType v = uint32_t(1);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, falseEqUint32Zero)
{
    nlohmann::json j = R"(false)"_json;
    BasicVariantType v = uint32_t(0);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, trueNeqDoubleNegativeOne)
{
    nlohmann::json j = R"(true)"_json;
    BasicVariantType v = double(-1.1);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, trueEqDoubleOne)
{
    nlohmann::json j = R"(true)"_json;
    BasicVariantType v = double(1.0);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, falseNeqDoubleOne)
{
    nlohmann::json j = R"(false)"_json;
    BasicVariantType v = double(1.0);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, falseEqDoubleZero)
{
    nlohmann::json j = R"(false)"_json;
    BasicVariantType v = double(0.0);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, falseEmptyError)
{
    nlohmann::json j = R"(false)"_json;
    BasicVariantType v;
    EXPECT_THROW(matchProbe(j, v), std::invalid_argument);
}

TEST(MatchProbe, trueEmptyError)
{
    nlohmann::json j = R"(true)"_json;
    BasicVariantType v;
    EXPECT_THROW(matchProbe(j, v), std::invalid_argument);
}

TEST(MatchProbe, uintStringError)
{
    nlohmann::json j = R"(11)"_json;
    BasicVariantType v = "11"s;
    EXPECT_THROW(matchProbe(j, v), std::invalid_argument);
}

TEST(MatchProbe, uintEqTrue)
{
    nlohmann::json j = R"(1)"_json;
    BasicVariantType v = true;
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, uintEqFalse)
{
    nlohmann::json j = R"(0)"_json;
    BasicVariantType v = false;
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, uintNeqTrue)
{
    nlohmann::json j = R"(11)"_json;
    BasicVariantType v = true;
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, uintEqUint8)
{
    nlohmann::json j = R"(11)"_json;
    BasicVariantType v = uint8_t(11);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, uintNeqUint8)
{
    nlohmann::json j = R"(11)"_json;
    BasicVariantType v = uint8_t(12);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, uintNeqUint8Overflow)
{
    nlohmann::json j = R"(65535)"_json;
    BasicVariantType v = uint8_t(255);
    EXPECT_FALSE(matchProbe(j, v));
}

TEST(MatchProbe, uintEqInt8)
{
    nlohmann::json j = R"(11)"_json;
    BasicVariantType v = int8_t(11);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, uintEqDouble)
{
    nlohmann::json j = R"(11)"_json;
    BasicVariantType v = double(11.0);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, uintEqDoubleRound)
{
    nlohmann::json j = R"(11)"_json;
    BasicVariantType v = double(11.7);
    EXPECT_TRUE(matchProbe(j, v));
}

TEST(MatchProbe, uintEmptyError)
{
    nlohmann::json j = R"(11)"_json;
    BasicVariantType v;
    EXPECT_THROW(matchProbe(j, v), std::invalid_argument);
}
