#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"


TEST(CDSVWriter, WriteRowTest) {
    CStringDataSink Sink;
    CDSVWriter Writer(Sink, ',');
    std::vector<std::string> Row = {"abc", "def", "ghi"};
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink.String(), "abc,def,ghi\n");
}

TEST(CDSVWriter, WriteRowWithQuotesTest) {
    CStringDataSink Sink;
    CDSVWriter Writer(Sink, ',');
    std::vector<std::string> Row = {"abc", "de,f", "ghi"};
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink.String(), "abc,\"de,f\",ghi\n");
}

TEST(CDSVWriter, WriteRowWithDoubleQuotesTest) {
    CStringDataSink Sink;
    CDSVWriter Writer(Sink, ',');
    std::vector<std::string> Row = {"abc", "de\"f", "ghi"};
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink.String(), "abc,\"de\"\"f\",ghi\n");
}

TEST(CDSVWriter, WriteEmptyRowTest) {
    CStringDataSink Sink;
    CDSVWriter Writer(Sink, ',');
    std::vector<std::string> Row = {};
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink.String(), "\n");
}

TEST(CDSVReader, ReadRowTest) {
    std::string Input = "abc,def,ghi\n";
    CStringDataSource Source(Input);
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, (std::vector<std::string>{"abc", "def", "ghi"}));
}

TEST(CDSVReader, ReadRowWithQuotesTest) {
    std::string Input = "abc,\"de,f\",ghi\n";
    CStringDataSource Source(Input);
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, (std::vector<std::string>{"abc", "de,f", "ghi"}));
}

TEST(CDSVReader, ReadRowWithDoubleQuotesTest) {
    std::string Input = "abc,\"de\"\"f\",ghi\n";
    CStringDataSource Source(Input);
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row, (std::vector<std::string>{"abc", "de\"f", "ghi"}));
}

TEST(CDSVReader, ReadEmptyRowTest) {
    std::string Input = "\n";
    CStringDataSource Source(Input);
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_TRUE(Row.empty());
}