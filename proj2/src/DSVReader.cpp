// Define the struct here 

// Add user defined functions in the struct

#include "DSVReader.h"
#include "StringUtils.h"
struct CDSVReader::SImplementation{
    std::shared_ptr< CDataSource > dataSource;
    char delim;
    
    SImplementation(std::shared_ptr< CDataSource > src, char delimiter){
        dataSource = src;
        delim = delimiter;
    }

    bool End() const{
        return dataSource->End();
    }

    bool ReadRow (std::vector<std::string> &row){
        
    }
}

CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
}

CDSVReader::End() const{
    return DImplementation->End();
}

