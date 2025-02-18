#include "DSVReader.h"
#include "StringUtils.h"
#include <memory>
#include <vector>
#include <string>
#include <iostream>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DataSource;
    char Delimiter;

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : DataSource(src), Delimiter(delimiter) {}

    bool End() const {
        return DataSource->End();
    }

    void SplitRow(std::vector<std::string> &row, const std::string& content, char delimiter) {
        std::string Field;
        bool InQuotes = false;

        for (size_t i = 0; i < content.size(); i++) {
            char ch = content[i];
            char next_ch = (i + 1) < content.size() ? content[i + 1] : '\0';

            if (ch == '\"') {
                if (!InQuotes) {
                    InQuotes = true;
                } else if (next_ch == '\"') { 
                    Field += ch;
                    i++;
                } else {
                    InQuotes = false;
                }
            } 
            else if (ch == delimiter && !InQuotes) {
                row.push_back(Field);
                Field.clear();
            } 
            else {
                Field += ch;
            }
        }
        
        row.push_back(Field); // Ensure last field is added
    }

    bool ReadRow (std::vector<std::string> &row){
        row.clear();
        std::vector<char> buf;
        bool InQuotes = false;
        char ch, next_ch;

        while (!End()) {
            if (DataSource->Get(ch)) {
                if (DataSource->Peek(next_ch)) {
                    if (ch == '\"') {
                        if (!InQuotes) {
                            InQuotes = true;
                        } else if (next_ch == '\"') {
                            buf.push_back(ch);
                            DataSource->Get(next_ch);
                        } else {
                            InQuotes = false;
                        }
                    } 
                    else if (!InQuotes && (ch == '\n' || ch == '\r')) {
                        if (ch == '\r' && next_ch == '\n') {
                            DataSource->Get(next_ch);
                        }
                        break;
                    } 
                    else {
                        buf.push_back(ch);
                    }
                }
            }
        }

        std::string data(buf.begin(), buf.end());
        SplitRow(row, data, Delimiter);
        return !row.empty();
    }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter) {
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
}

CDSVReader::~CDSVReader() = default;

bool CDSVReader::End() const {
    return DImplementation->End();
}

bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    return DImplementation->ReadRow(row);
}
