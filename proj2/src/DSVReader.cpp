// Define the struct here 

// Add user defined functions in the struct

#include "DSVReader.h"
class CDSVReader{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;

    public:
        CDSVReader(std::shared_ptr< CDataSource > src, char delimiter);
        // Destructor doesn't need to be explicitly defined
        // Unique pointer automatically handles deletion of SImplementation object
        ~CDSVReader() = default;

        bool End() const;
        bool ReadRow(std::vector<std::string> &row);
};

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

}

CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
}