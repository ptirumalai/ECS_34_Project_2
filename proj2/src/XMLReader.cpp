#include "XMLReader.h"
#include "DataSource.h"
#include <expat.h>
#include <list>
#include <iostream>

// Private implementation struct
struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> DSource; // Data source for reading XML
    XML_Parser Parser;                    // Expat parser
    std::list<SXMLEntity> DEntities;      // Queue of parsed entities

    // Constructor
    SImplementation(std::shared_ptr<CDataSource> src)
        : DSource(src), Parser(XML_ParserCreate(nullptr)) {
        if (!Parser) {
            throw std::runtime_error("Failed to create XML parser");
        }
        XML_SetUserData(Parser, this);
        XML_SetElementHandler(Parser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(Parser, CharDataHandler);
    }

    // Destructor
    ~SImplementation() {
        XML_ParserFree(Parser);
    }

    // Expat start element handler
    static void StartElementHandler(void* userData, const XML_Char* name, const XML_Char** attrs) {
        SImplementation* Impl = static_cast<SImplementation*>(userData);
        SXMLEntity Entity;
        Entity.DType = SXMLEntity::EType::StartElement;
        Entity.DNameData = name;
        for (int i = 0; attrs[i]; i += 2) {
            Entity.DAttributes.push_back({attrs[i], attrs[i + 1]});
        }
        Impl->DEntities.push_back(Entity);
    }

    // Expat end element handler
    static void EndElementHandler(void* userData, const XML_Char* name) {
        SImplementation* Impl = static_cast<SImplementation*>(userData);
        SXMLEntity Entity;
        Entity.DType = SXMLEntity::EType::EndElement;
        Entity.DNameData = name;
        Impl->DEntities.push_back(Entity);
    }

    // Expat character data handler
    static void CharDataHandler(void* userData, const XML_Char* str, int len) {
        SImplementation* Impl = static_cast<SImplementation*>(userData);
        SXMLEntity Entity;
        Entity.DType = SXMLEntity::EType::CharData;
        Entity.DNameData = std::string(str, len);
        Impl->DEntities.push_back(Entity);
    }
};

// CXMLReader constructor
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>(src)) {}

// CXMLReader destructor
CXMLReader::~CXMLReader() {}

// Check if the data source has ended
bool CXMLReader::End() const {
    return DImplementation->DSource->End();
}

// Read the next XML entity
bool CXMLReader::ReadEntity(SXMLEntity& entity, bool skipcdata) {
    char Buffer[1024];
    while (!DImplementation->DSource->End()) {
        size_t BytesRead = DImplementation->DSource->Read((unsigned char*)Buffer, sizeof(Buffer));
        if (XML_Parse(DImplementation->Parser, Buffer, BytesRead, BytesRead == 0) == XML_STATUS_OK) {
            if (!DImplementation->DEntities.empty()) {
                entity = DImplementation->DEntities.front();
                DImplementation->DEntities.pop_front();
                if (skipcdata && entity.DType == SXMLEntity::EType::CharData) {
                    continue; // Skip CDATA if requested
                }
                return true;
            }
        } else {
            std::cerr << "XML Parse Error: " << XML_ErrorString(XML_GetErrorCode(DImplementation->Parser)) << std::endl;
            return false;
        }
    }
    return false;
}