// Define the struct here 

// Add user defined functions in the struct

#include "DSVReader.h"
#include "StringUtils.h"
struct CDSVReader::SImplementation{
    std::shared_ptr< CDataSource > dataSource;
    std::vector<std::string> leftover;
    char delim;
    
    SImplementation(std::shared_ptr< CDataSource > src, char delimiter){
        dataSource = src;
        delim = delimiter;
    }

    bool End() const{
        return dataSource->End() && leftover.empty();
    }

    bool CheckSpecialCase(std::vector<std::string> &row){
        for (int i = 0; i < row.size(); i++){
            // If the first character of the row element is a double quote & the last character of the row element is a double quote
            if (row[i][0] == '\"' && row[i][row[i].size() - 1] == '\"'){
                    // Remove the first and last characters of the row element
                    row[i] = StringUtils::Slice(row[i], 1, -1);
                    row[i] = StringUtils::Replace(row[i], "\"\"", "\"");
            }
            // If the first character of the row element is a double quote
            else if (row[i][0] == '\"' && row[i][row[i].size() - 1] != '\"'){
                return false;
            }
            else if (row[i][0] != '\"' && row[i][row[i].size() - 1] == '\"'){
                return false;
            }
        }
        return true;
    }

    bool ReadRow (std::vector<std::string> &row){
        row.clear();

        // Define an empty character vector 
        std::vector<char> buf;

        // Define the buffer size
        std::size_t buffer_size = 256;

        // If the leftover vector has multiple elements, then return the first row element without reading from the data source
        if (leftover.size() > 1){
            // Split the first element of the leftover vector by the delimiter
            row = StringUtils::Split(leftover[0], delim);

            // Remove the first element of the leftover vector
            leftover.erase(leftover.begin());

            // If there is an issue with the special case, return false
            if (!CheckSpecialCase(row)){
                return false;
            }

            // Otherwise, return true
            return true;
        }
        
        // If the data source is at the end, return false
        else if (End()){
            return false;
        }

        else if (dataSource->Read(buf, buffer_size)){
            // Define a string from the buffer
            std::string str(buf.begin(), buf.end());

            // Split the string by newline after adding any previous leftover elements
            std::vector<std::string> temp = StringUtils::Split(leftover[0] + str, "\n");

            // Remove the first element of the leftover vector
            leftover.clear();

            // Split the first element of the temp vector by the delimiter
            row = StringUtils::Split(temp[0], delim);

            // If there is an issue with the special case, return false
            if (!CheckSpecialCase(row)){
                return false;
            }

            // If the temp vector has more than one element, add the remaining elements to the leftover vector
            if (temp.size() > 1){
                for (int i = 1; i < temp.size(); i++){
                    leftover.push_back(temp[i]);
                }
            }
            return true;
        }
        return false;
    }
}

CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
}

CDSVReader::End() const{
    return DImplementation->End();
}

CDSVReader::ReadRow(std::vector<std::string> &row){
    return DImplementation->ReadRow(row);
}