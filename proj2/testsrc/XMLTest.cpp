#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(CXMLWriter, WriteEntityTest) {
    CStringDataSink Sink;
    CXMLWriter Writer(Sink);

    SXMLEntity Entity;
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "root";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    EXPECT_EQ(Sink.String(), "<root>");
}

TEST(CXMLWriter, WriteEntityWithAttributesTest) {
    CStringDataSink Sink;
    CXMLWriter Writer(Sink);

    SXMLEntity Entity;
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "root";
    Entity.DAttributes.push_back({"attr1", "value1"});
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    EXPECT_EQ(Sink.String(), "<root attr1=\"value1\">");
}

TEST(CXMLWriter, WriteCompleteElementTest) {
    CStringDataSink Sink;
    CXMLWriter Writer(Sink);

    SXMLEntity Entity;
    Entity.DType = SXMLEntity::EType::CompleteElement;
    Entity.DNameData = "root";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    EXPECT_EQ(Sink.String(), "<root />");
}

TEST(CXMLWriter, WriteTextTest) {
    CStringDataSink Sink;
    CXMLWriter Writer(Sink);

    SXMLEntity Entity;
    Entity.DType = SXMLEntity::EType::CharData;
    Entity.DNameData = "Hello, World!";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    EXPECT_EQ(Sink.String(), "Hello, World!");
}

TEST(CXMLReader, ReadEntityTest) {
    std::string Input = "<root>";
    CStringDataSource Source(Input);
    CXMLReader Reader(Source);

    SXMLEntity Entity;
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "root");
}

TEST(CXMLReader, ReadEntityWithAttributesTest) {
    std::string Input = "<root attr1=\"value1\">";
    CStringDataSource Source(Input);
    CXMLReader Reader(Source);

    SXMLEntity Entity;
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "root");
    EXPECT_EQ(Entity.DAttributes.size(), 1);
    EXPECT_EQ(Entity.DAttributes[0].first, "attr1");
    EXPECT_EQ(Entity.DAttributes[0].second, "value1");
}

TEST(CXMLReader, ReadCompleteElementTest) {
    std::string Input = "<root />";
    CStringDataSource Source(Input);
    CXMLReader Reader(Source);

    SXMLEntity Entity;
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CompleteElement);
    EXPECT_EQ(Entity.DNameData, "root");
}

TEST(CXMLReader, ReadTextTest) {
    std::string Input = "Hello, World!";
    CStringDataSource Source(Input);
    CXMLReader Reader(Source);

    SXMLEntity Entity;
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "Hello, World!");
}