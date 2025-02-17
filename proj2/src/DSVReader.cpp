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

    // bool CheckSpecialCase(std::vector<std::string>& row) {
    //     // Look at all the fields in the row
    //     for (size_t i = 0; i < row.size(); i++) {
    //         if (row[i].empty()) {
    //             continue; // Skip empty fields
    //         }
    //         if (row[i][0] == '\"') {
    //             if (row[i].back() != '\"') {
    //                 return false; // Mismatched quotes
    //             }
    //             // Remove quotes and unescape double quotes
    //             row[i] = StringUtils::Slice(row[i], 1, -1);
    //             row[i] = StringUtils::Replace(row[i], "\"\"", "\"");
    //         }
    //     }
    //     return true;
    // }

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
            } else if (InQuotes && ch == '\"' && next_ch == '\"') {
                Field += ch;
                i++;
            } else if (ch == delimiter && !InQuotes) {
                row.push_back(Field);
                Field.clear();
            } else {
                Field += ch;
            }
            std::cout << "Field: " << Field << std::endl;                         
        }
        
        if (!Field.empty()) {
            row.push_back(Field);
        }
    }

    // void NormalizeRow(std::string& data) {
    //     // Replace all '\r' with '\n' and remove '\n' after '\r'
    //     for (size_t i = 0; i < data.size(); i++) {
    //         if (data[i] == '\r') {
    //             data[i] = '\n';
    //             if (i + 1 < data.size() && data[i + 1] == '\n') {
    //                 data.erase(i + 1, 1);
    //             }
    //         }
    //     }
    // }

    bool ReadRow (std::vector<std::string> &row){
        row.clear();
        bool InQuotes = false;

        // Define an empty character vector
        std::vector<char> buf;

        while(!DataSource->End()){
            char ch;
            char next_ch;
            if (DataSource->Get(ch)){
                if (ch == '\"' && next_ch != '\"') {    
                    InQuotes = !InQuotes;
                }
                else if (InQuotes && ch == '\"' && next_ch == '\"') {
                    buf.push_back(ch);
                    buf.push_back(next_ch);
                    DataSource->Get(next_ch);
                }
                // If the character is a newline character and not in quotes, break
                else if (ch == '\n' && !InQuotes){
                    break;
                } 
                // If the character is a carriage return character, then check if the next character is a newline character
                if (ch == '\r' && !InQuotes){
                    if (DataSource->Peek(next_ch)){
                        if (next_ch == '\n'){
                            DataSource->Get(next_ch);
                        }
                    }
                    break;
                }
            };
            // This works because we're making a copy of the character to add to buf
            buf.push_back(ch);
            }

        // Define a string variable to store the data read from the data source
        std::string data(buf.begin(), buf.end());

        SplitRow(row, data, Delimiter);
    }
        

    // bool ReadRow (std::vector<std::string> &row){
    //     row.clear();

    //     // Define an empty character vector 
    //     std::vector<char> buf;

    //     // Define the buffer size
    //     std::size_t buffer_size = 256;

    //     // If the leftover vector has multiple elements, then return the first row element without reading from the data source
    //     if (Leftover.size() > 1){

    //         // Split the first element of the leftover vector by the delimiter
    //         SplitRow(row, Leftover[0], Delimiter);

    //         // Remove the first element of the leftover vector
    //         Leftover.erase(Leftover.begin());

    //         // Return true
    //         return true;
    //     }

    //     else if (End()){
    //         return false;
    //     }

    //     else if (DataSource->Read(buf, buffer_size)){
    //         // Define a string variable to store the data read from the data source
    //         std::string data(buf.begin(), buf.end());

    //         if (Leftover.size() == 1){
    //             data = Leftover[0] + data;
    //         }

    //         // Normalize the row to only have newline characters
    //         NormalizeRow(data);

    //         // Split the data by the newline character to get each row separately in a vector
    //         std::vector<std::string> rows = StringUtils::Split(data, "\n");
    //         std::cout << "Rows: " << rows.size() << std::endl;

    //         SplitRow(row, rows[0], Delimiter);
    //         std::cout << "Row: " << row.size() << std::endl;

    //         if (rows.size() > 1){
    //             for (size_t i = 1; i < rows.size(); i++){
    //                 Leftover.push_back(rows[i]);
    //             }
    //         }

    //         // Return true
    //         return true;
    //     }
        
    //     else{
    //         return false;
    //     }
    // }
                        


    // bool ReadRow(std::vector<std::string>& row) {
    //     row.clear();
    
    //     std::string AccumulatedData;
    //     if (!Leftover.empty()) {
    //         AccumulatedData = Leftover[0];
    //         Leftover.clear();
    //     }
    
    //     while (true) {
    //         std::vector<char> Buffer;
    //         std::size_t BufferSize = 256;
    //         if (!DataSource->Read(Buffer, BufferSize)) {
    //             if (AccumulatedData.empty()) {
    //                 return false;
    //             }
    //             break;
    //         }
    
    //         AccumulatedData += std::string(Buffer.begin(), Buffer.end());
    
    //         size_t NewlinePos = AccumulatedData.find('\n');
    //         if (NewlinePos != std::string::npos) {
    //             break;
    //         }
    //     }
    
    //     size_t NewlinePos = AccumulatedData.find('\n');
    //     std::string CurrentRow = AccumulatedData.substr(0, NewlinePos);
    //     if (NewlinePos != std::string::npos) {
    //         Leftover.push_back(AccumulatedData.substr(NewlinePos + 1));
    //     }
    
    //     row = SplitRow(CurrentRow, Delimiter);
    
    //     if (!CheckSpecialCase(row)) {
    //         return false;
    //     }
    
    //     return true;
    // }
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