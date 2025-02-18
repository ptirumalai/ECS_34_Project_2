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
        // std::vector<std::string> Fields;
        std::string Field;
        bool InQuotes = false;

        for (size_t i = 0; i < content.size(); i++) {
            char ch = content[i];
            // Account for being at the end of the row
            char next_ch = (i + 1) < content.size() ? content[i + 1] : '\0';
            if (ch == '\"' && next_ch != '\"') {    
                InQuotes = !InQuotes;
                Field += ch;
            } else if (InQuotes && ch == '\"' && next_ch == '\"') {
                Field += ch;
                i++;
            } else if (ch == delimiter && !InQuotes) {
                row.push_back(Field);
                Field.clear();
            } else {
                Field += ch;
            }                      
        }
        
        if (!Field.empty()) {
            row.push_back(Field);
        }
    }

    bool ReadRow (std::vector<std::string> &row){
        row.clear();

        // Define an empty character vector
        std::vector<char> buf;
        bool InQuotes = false;
        char ch;
        char next_ch;
        while(!End()){
            if (DataSource->Get(ch)){
                if (DataSource->Peek(next_ch)){    
                    if (ch == '\"' && !InQuotes){
                        InQuotes = !InQuotes;
                        buf.push_back(ch);
                    }
                    else if (ch == '\"' && next_ch == Delimiter && InQuotes){
                        InQuotes = !InQuotes;
                        buf.push_back(ch);
                    }
                    else if (InQuotes && ch == '\"' && (next_ch == '\n' || next_ch == '\r')){
                        buf.push_back(ch);
                        DataSource->Get(next_ch);
                        break;
                    }
                    else if (ch == '\n' && !InQuotes){
                        break;
                    }
                    else if (ch == '\r' && !InQuotes){
                        if (DataSource->Peek(next_ch)){
                            if (next_ch == '\n'){
                                DataSource->Get(next_ch);
                            }
                        }
                        break;
                    }
                    else{
                        buf.push_back(ch);
                    }
                }
            }
            // std::string data(buf.begin(), buf.end());
            // std::cout << "buf: " << data << std::endl;
        }
                    
        // Define a string variable to store the data read from the data source
        std::string data(buf.begin(), buf.end());
        // std::cout << "Data: " << data << std::endl;
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