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

            // Split the first element of the temp vector by the delimiter
            row = StringUtils::Split(temp[0], delim);

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