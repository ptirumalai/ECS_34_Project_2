#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

// Tests for CXMLWriter
TEST(CXMLWriterTest, WriteEntityTest) {
    auto Sink = std::make_shared<CStringDataSink>(); // Wrap in shared_ptr
    CXMLWriter Writer(Sink);

    SXMLEntity Entity;
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "root";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    EXPECT_EQ(Sink->String(), "<root>");
}

TEST(CXMLWriterTest, WriteEntityWithAttributesTest) {
    auto Sink = std::make_shared<CStringDataSink>(); // Wrap in shared_ptr
    CXMLWriter Writer(Sink);

    SXMLEntity Entity;
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "root";
    Entity.DAttributes.push_back({"attr1", "value1"});
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    EXPECT_EQ(Sink->String(), "<root attr1=\"value1\">");
}

// Tests for CXMLReader
TEST(CXMLReaderTest, ReadEntityTest) {
    std::string Input = "<root>Hello, World!</root>";
    auto Source = std::make_shared<CStringDataSource>(Input); // Wrap in shared_ptr
    CXMLReader Reader(Source);

    SXMLEntity Entity;
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "root");

    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "Hello, World!");

    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "root");
}

TEST(CXMLReaderTest, ReadEntityWithAttributesTest) {
    std::string Input = "<root attr1=\"value1\">";
    auto Source = std::make_shared<CStringDataSource>(Input); // Wrap in shared_ptr
    CXMLReader Reader(Source);

    SXMLEntity Entity;
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "root");
    EXPECT_EQ(Entity.DAttributes.size(), size_t(1)); // Fix signed/unsigned comparison
    EXPECT_EQ(Entity.DAttributes[0].first, "attr1");
    EXPECT_EQ(Entity.DAttributes[0].second, "value1");
}