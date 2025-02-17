// #include "XMLReader.h"
// #include "StringUtils.h"
// #include <memory>
// #include <vector>
// #include <string>
// #include <expat.h>

// struct CXMLReader::SImplementation {
//     std::shared_ptr<CDataSource> DSource;
//     XML_Parser DParser;
//     SXMLEntity DCurrentEntity;
//     bool DHasEntity;

//     SImplementation(std::shared_ptr<CDataSource> src)
//         : DSource(src), DHasEntity(false) {
//         DParser = XML_ParserCreate(nullptr);
//         XML_SetUserData(DParser, this);
//         XML_SetElementHandler(DParser, StartElementHandler, EndElementHandler);
//         XML_SetCharacterDataHandler(DParser, CharacterDataHandler);
//     }

//     ~SImplementation() {
//         XML_ParserFree(DParser);
//     }

//     static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts) {
//         SImplementation *impl = static_cast<SImplementation*>(userData);
//         impl->DCurrentEntity.DType = SXMLEntity::EType::StartElement;
//         impl->DCurrentEntity.DNameData = name;
//         impl->DHasEntity = true;
//     }

//     static void EndElementHandler(void *userData, const XML_Char *name) {
//         SImplementation *impl = static_cast<SImplementation*>(userData);
//         impl->DCurrentEntity.DType = SXMLEntity::EType::EndElement;
//         impl->DCurrentEntity.DNameData = name;
//         impl->DHasEntity = true;
//     }

//     static void CharacterDataHandler(void *userData, const XML_Char *s, int len) {
//         SImplementation *impl = static_cast<SImplementation*>(userData);
//         impl->DCurrentEntity.DType = SXMLEntity::EType::CharData;
//         impl->DCurrentEntity.DNameData = std::string(s, len);
//         impl->DHasEntity = true;
//     }

//     bool ReadEntity(SXMLEntity& entity, bool skipcdata) {
//         std::vector<char> Buffer(1024); // Use a vector for the buffer
//         while (!DSource->End()) {
//             size_t BytesRead = DSource->Read(Buffer, Buffer.size()); // Pass the vector to Read
//             if (BytesRead > 0) {
//                 if (!XML_Parse(DParser, Buffer.data(), BytesRead, XML_FALSE)) {
//                     // Handle parsing error
//                     XML_Error errorCode = XML_GetErrorCode(DParser);
//                     const char *errorString = XML_ErrorString(errorCode);
//                     // Log or handle the error
//                     return false;
//                 }
//                 if (DHasEntity) {
//                     entity = DCurrentEntity;
//                     DHasEntity = false;
//                     return true;
//                 }
//             }
//         }
//         XML_Parse(DParser, nullptr, 0, XML_TRUE); // Finalize parsing
//         return true;
//     }
// };

// // Constructor
// CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) {
//     DImplementation = std::make_unique<SImplementation>(src);
// }

// // Destructor
// CXMLReader::~CXMLReader() = default;

// // End() function
// bool CXMLReader::End() const {
//     return DImplementation->DSource->End();
// }

// // ReadEntity() function
// bool CXMLReader::ReadEntity(SXMLEntity& entity, bool skipcdata) {
//     return DImplementation->ReadEntity(entity, skipcdata);
// }

