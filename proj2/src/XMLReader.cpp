#include "XMLReader.h"
#include "StringUtils.h"
#include <memory>
#include <vector>
#include <string>
#include <expat.h>

// Define the SImplementation struct
struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> DSource;
    XML_Parser DParser;
    SXMLEntity DCurrentEntity;
    bool DHasEntity;

    SImplementation(std::shared_ptr<CDataSource> src)
        : DSource(src), DHasEntity(false) {
        DParser = XML_ParserCreate(nullptr);
    }

    ~SImplementation() {
        XML_ParserFree(DParser);
    }

    bool ReadEntity(SXMLEntity& entity, bool skipcdata) {
        std::vector<char> Buffer(1024); // Use a vector for the buffer
        while (!DSource->End()) {
            size_t BytesRead = DSource->Read(Buffer, Buffer.size()); // Pass the vector to Read
            if (BytesRead > 0) {
                if (!XML_Parse(DParser, Buffer.data(), BytesRead, XML_FALSE)) {
                    // Handle parsing error
                    return false;
                }
                if (DHasEntity) {
                    entity = DCurrentEntity;
                    DHasEntity = false;
                    return true;
                }
            }
        }
        XML_Parse(DParser, nullptr, 0, XML_TRUE); // Finalize parsing
        return false;
    }
};

// Constructor
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) {
    DImplementation = std::make_unique<SImplementation>(src);
}

// Destructor
CXMLReader::~CXMLReader() = default;

// End() function
bool CXMLReader::End() const {
    return DImplementation->DSource->End();
}

// ReadEntity() function
bool CXMLReader::ReadEntity(SXMLEntity& entity, bool skipcdata) {
    return DImplementation->ReadEntity(entity, skipcdata);
}