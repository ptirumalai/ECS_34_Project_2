#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

// Tests for CXMLWriter
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

TEST(CXMLWriter, WriteNestedElementsTest) {
    CStringDataSink Sink;
    CXMLWriter Writer(Sink);

    // Write the root element
    SXMLEntity RootEntity;
    RootEntity.DType = SXMLEntity::EType::StartElement;
    RootEntity.DNameData = "root";
    EXPECT_TRUE(Writer.WriteEntity(RootEntity));

    // Write a nested child element
    SXMLEntity ChildEntity;
    ChildEntity.DType = SXMLEntity::EType::StartElement;
    ChildEntity.DNameData = "child";
    EXPECT_TRUE(Writer.WriteEntity(ChildEntity));

    // Close the child element
    SXMLEntity ChildEndEntity;
    ChildEndEntity.DType = SXMLEntity::EType::EndElement;
    ChildEndEntity.DNameData = "child";
    EXPECT_TRUE(Writer.WriteEntity(ChildEndEntity));

    // Close the root element
    SXMLEntity RootEndEntity;
    RootEndEntity.DType = SXMLEntity::EType::EndElement;
    RootEndEntity.DNameData = "root";
    EXPECT_TRUE(Writer.WriteEntity(RootEndEntity));

    // Verify the output
    EXPECT_EQ(Sink.String(), "<root><child></child></root>");
}

TEST(CXMLWriter, WriteMultipleAttributesTest) {
    CStringDataSink Sink;
    CXMLWriter Writer(Sink);

    SXMLEntity Entity;
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "root";
    Entity.DAttributes.push_back({"attr1", "value1"});
    Entity.DAttributes.push_back({"attr2", "value2"});
    Entity.DAttributes.push_back({"attr3", "value3"});
    EXPECT_TRUE(Writer.WriteEntity(Entity));

    // Verify the output
    EXPECT_EQ(Sink.String(), "<root attr1=\"value1\" attr2=\"value2\" attr3=\"value3\">");
}

// Tests for CXMLReader
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

TEST(CXMLReader, ReadNestedElementsTest) {
    std::string Input = "<root><child></child></root>";
    CStringDataSource Source(Input);
    CXMLReader Reader(Source);

    // Read the root element
    SXMLEntity RootEntity;
    EXPECT_TRUE(Reader.ReadEntity(RootEntity));
    EXPECT_EQ(RootEntity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(RootEntity.DNameData, "root");

    // Read the child element
    SXMLEntity ChildEntity;
    EXPECT_TRUE(Reader.ReadEntity(ChildEntity));
    EXPECT_EQ(ChildEntity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(ChildEntity.DNameData, "child");

    // Read the end of the child element
    SXMLEntity ChildEndEntity;
    EXPECT_TRUE(Reader.ReadEntity(ChildEndEntity));
    EXPECT_EQ(ChildEndEntity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(ChildEndEntity.DNameData, "child");

    // Read the end of the root element
    SXMLEntity RootEndEntity;
    EXPECT_TRUE(Reader.ReadEntity(RootEndEntity));
    EXPECT_EQ(RootEndEntity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(RootEndEntity.DNameData, "root");
}

TEST(CXMLReader, ReadMultipleAttributesTest) {
    std::string Input = "<root attr1=\"value1\" attr2=\"value2\" attr3=\"value3\">";
    CStringDataSource Source(Input);
    CXMLReader Reader(Source);

    SXMLEntity Entity;
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "root");
    EXPECT_EQ(Entity.DAttributes.size(), 3);
    EXPECT_EQ(Entity.DAttributes[0].first, "attr1");
    EXPECT_EQ(Entity.DAttributes[0].second, "value1");
    EXPECT_EQ(Entity.DAttributes[1].first, "attr2");
    EXPECT_EQ(Entity.DAttributes[1].second, "value2");
    EXPECT_EQ(Entity.DAttributes[2].first, "attr3");
    EXPECT_EQ(Entity.DAttributes[2].second, "value3");
}