// #include "XMLWriter.h"
// #include "DataSink.h"
// #include <stack>
// #include <vector>

// // Private implementation struct
// struct CXMLWriter::SImplementation {
//     std::shared_ptr<CDataSink> DSink; // Data sink for writing XML
//     std::stack<std::string> DEndElements; // Stack to track open elements

//     // Constructor
//     SImplementation(std::shared_ptr<CDataSink> sink)
//         : DSink(sink) {}

//     // Write a string to the data sink
//     bool WriteString(const std::string& str) {
//         std::vector<char> data(str.begin(), str.end());
//         return DSink->Write(data);
//     }

//     // Write a character to the data sink
//     bool WriteChar(char ch) {
//         return DSink->Put(ch);
//     }
// };

// // CXMLWriter constructor
// CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
//     : DImplementation(std::make_unique<SImplementation>(sink)) {}

// // CXMLWriter destructor
// CXMLWriter::~CXMLWriter() {
//     Flush(); // Ensure all open elements are closed
// }

// // Flush the writer (close all open elements)
// bool CXMLWriter::Flush() {
//     while (!DImplementation->DEndElements.empty()) {
//         const std::string& Element = DImplementation->DEndElements.top();
//         DImplementation->WriteString("</");
//         DImplementation->WriteString(Element);
//         DImplementation->WriteChar('>');
//         DImplementation->DEndElements.pop();
//     }
//     return true;
// }

// // Write an XML entity to the data sink
// bool CXMLWriter::WriteEntity(const SXMLEntity& entity) {
//     switch (entity.DType) {
//         case SXMLEntity::EType::StartElement:
//             DImplementation->WriteChar('<');
//             DImplementation->WriteString(entity.DNameData);
//             for (const auto& Attr : entity.DAttributes) {
//                 DImplementation->WriteChar(' ');
//                 DImplementation->WriteString(Attr.first);
//                 DImplementation->WriteChar('=');
//                 DImplementation->WriteChar('"');
//                 DImplementation->WriteString(Attr.second);
//                 DImplementation->WriteChar('"');
//             }
//             DImplementation->WriteChar('>');
//             DImplementation->DEndElements.push(entity.DNameData);
//             break;

//         case SXMLEntity::EType::EndElement:
//             DImplementation->WriteString("</");
//             DImplementation->WriteString(entity.DNameData);
//             DImplementation->WriteChar('>');
//             break;

//         case SXMLEntity::EType::CompleteElement:
//             DImplementation->WriteChar('<');
//             DImplementation->WriteString(entity.DNameData);
//             for (const auto& Attr : entity.DAttributes) {
//                 DImplementation->WriteChar(' ');
//                 DImplementation->WriteString(Attr.first);
//                 DImplementation->WriteChar('=');
//                 DImplementation->WriteChar('"');
//                 DImplementation->WriteString(Attr.second);
//                 DImplementation->WriteChar('"');
//             }
//             DImplementation->WriteString(" />");
//             break;

//         case SXMLEntity::EType::CharData:
//             DImplementation->WriteString(entity.DNameData);
//             break;
//     }
//     return true;
// }

#include "XMLWriter.h"
#include "DataSink.h"
#include <stack>
#include <vector>
#include <sstream>

// Private implementation struct
struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink; // Data sink for writing XML
    std::stack<std::string> DEndElements; // Stack to track open elements
    int IndentLevel = 0; // Track indentation level for pretty-printing

    // Constructor
    SImplementation(std::shared_ptr<CDataSink> sink)
        : DSink(sink) {}

    // Write a string to the data sink
    bool WriteString(const std::string& str) {
        std::vector<char> data(str.begin(), str.end());
        return DSink->Write(data);
    }

    // Write a character to the data sink
    bool WriteChar(char ch) {
        return DSink->Put(ch);
    }

    // Escape special characters in a string
    std::string EscapeString(const std::string& str) {
        std::stringstream ss;
        for (char ch : str) {
            switch (ch) {
                case '&': ss << "&amp;"; break;
                case '"': ss << "&quot;"; break;
                case '\'': ss << "&apos;"; break;
                case '<': ss << "&lt;"; break;
                case '>': ss << "&gt;"; break;
                default: ss << ch; break;
            }
        }
        return ss.str();
    }

    // Write indentation (tabs) based on the current indentation level
    bool WriteIndent() {
        for (int i = 0; i < IndentLevel; i++) {
            if (!WriteChar('\t')) {
                return false;
            }
        }
        return true;
    }
};

// CXMLWriter constructor
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(sink)) {}

// CXMLWriter destructor
CXMLWriter::~CXMLWriter() {
    Flush(); // Ensure all open elements are closed
}

// Flush the writer (close all open elements)
bool CXMLWriter::Flush() {
    while (!DImplementation->DEndElements.empty()) {
        const std::string& Element = DImplementation->DEndElements.top();
        DImplementation->WriteIndent();
        DImplementation->WriteString("</");
        DImplementation->WriteString(Element);
        DImplementation->WriteChar('>');
        DImplementation->WriteChar('\n');
        DImplementation->DEndElements.pop();
        DImplementation->IndentLevel--;
    }
    return true;
}

// Write an XML entity to the data sink
bool CXMLWriter::WriteEntity(const SXMLEntity& entity) {
    switch (entity.DType) {
        case SXMLEntity::EType::StartElement:
            DImplementation->WriteIndent();
            DImplementation->WriteChar('<');
            DImplementation->WriteString(entity.DNameData);
            for (const auto& Attr : entity.DAttributes) {
                DImplementation->WriteChar(' ');
                DImplementation->WriteString(Attr.first);
                DImplementation->WriteChar('=');
                DImplementation->WriteChar('"');
                DImplementation->WriteString(DImplementation->EscapeString(Attr.second));
                DImplementation->WriteChar('"');
            }
            DImplementation->WriteChar('>');
            DImplementation->WriteChar('\n');
            DImplementation->DEndElements.push(entity.DNameData);
            DImplementation->IndentLevel++;
            break;

        case SXMLEntity::EType::EndElement:
            DImplementation->IndentLevel--;
            DImplementation->WriteIndent();
            DImplementation->WriteString("</");
            DImplementation->WriteString(entity.DNameData);
            DImplementation->WriteChar('>');
            DImplementation->WriteChar('\n');
            break;

        case SXMLEntity::EType::CompleteElement:
            DImplementation->WriteIndent();
            DImplementation->WriteChar('<');
            DImplementation->WriteString(entity.DNameData);
            for (const auto& Attr : entity.DAttributes) {
                DImplementation->WriteChar(' ');
                DImplementation->WriteString(Attr.first);
                DImplementation->WriteChar('=');
                DImplementation->WriteChar('"');
                DImplementation->WriteString(DImplementation->EscapeString(Attr.second));
                DImplementation->WriteChar('"');
            }
            DImplementation->WriteString(" />");
            DImplementation->WriteChar('\n');
            break;

        case SXMLEntity::EType::CharData:
            DImplementation->WriteIndent();
            DImplementation->WriteString(DImplementation->EscapeString(entity.DNameData));
            DImplementation->WriteChar('\n');
            break;
    }
    return true;
}