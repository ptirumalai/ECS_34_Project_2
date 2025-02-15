#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

// Tests for CDSVWriter
TEST(CDSVWriter, WriteRowTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');

    std::vector<std::string> Row = {"value1", "value2", "value3"};
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "value1,value2,value3\n");
}

TEST(CDSVWriter, WriteRowWithQuotesTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');

    std::vector<std::string> Row = {"value,1", "value\"2", "value\n3"};
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "\"value,1\",\"value\"\"2\",\"value\n3\"\n");
}

TEST(CDSVWriter, WriteRowWithQuoteAllTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',', true); // quoteall = true

    std::vector<std::string> Row = {"value1", "value2", "value3"};
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "\"value1\",\"value2\",\"value3\"\n");
}

TEST(CDSVWriter, WriteEmptyRowTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');

    std::vector<std::string> Row = {};
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "\n");
}

// Tests for CDSVReader
TEST(CDSVReader, ReadRowTest) {
    std::string Input = "value1,value2,value3\n";
    auto Source = std::make_shared<CStringDataSource>(Input);
    CDSVReader Reader(Source, ',');

    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), size_t(3));
    EXPECT_EQ(Row[0], "value1");
    EXPECT_EQ(Row[1], "value2");
    EXPECT_EQ(Row[2], "value3");
}

TEST(CDSVReader, ReadRowWithQuotesTest) {
    std::string Input = "\"value,1\",\"value\"\"2\",\"value\n3\"\n";
    auto Source = std::make_shared<CStringDataSource>(Input);
    CDSVReader Reader(Source, ',');

    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), 3);
    EXPECT_EQ(Row[0], "value,1");
    EXPECT_EQ(Row[1], "value\"2");
    EXPECT_EQ(Row[2], "value\n3");
}

TEST(CDSVReader, ReadEmptyRowTest) {
    std::string Input = "\n";
    auto Source = std::make_shared<CStringDataSource>(Input);
    CDSVReader Reader(Source, ',');

    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), size_t(0));
}

TEST(CDSVReader, ReadMultipleRowsTest) {
    std::string Input = "value1,value2,value3\nvalue4,value5,value6\n";
    auto Source = std::make_shared<CStringDataSource>(Input);
    CDSVReader Reader(Source, ',');

    std::vector<std::string> Row1, Row2;
    EXPECT_TRUE(Reader.ReadRow(Row1));
    EXPECT_TRUE(Reader.ReadRow(Row2));

    EXPECT_EQ(Row1.size(), 3);
    EXPECT_EQ(Row1[0], "value1");
    EXPECT_EQ(Row1[1], "value2");
    EXPECT_EQ(Row1[2], "value3");

    EXPECT_EQ(Row2.size(), 3);
    EXPECT_EQ(Row2[0], "value4");
    EXPECT_EQ(Row2[1], "value5");
    EXPECT_EQ(Row2[2], "value6");
}