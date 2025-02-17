#include "DSVReader.h"
#include "StringUtils.h"
#include <memory>
#include <vector>
#include <string>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DataSource;
    std::vector<std::string> Leftover;
    char Delimiter;

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : DataSource(src), Delimiter(delimiter) {}

    bool End() const {
        return DataSource->End() && Leftover.empty();
    }

    bool CheckSpecialCase(std::vector<std::string>& row) {
        for (size_t i = 0; i < row.size(); i++) {
            if (row[i].empty()) {
                continue; // Skip empty fields
            }
            if (row[i][0] == '\"') {
                if (row[i].back() != '\"') {
                    return false; // Mismatched quotes
                }
                // Remove the surrounding quotes
                row[i] = row[i].substr(1, row[i].size() - 2);
                // Replace escaped quotes ("") with a single quote (")
                row[i] = StringUtils::Replace(row[i], "\"\"", "\"");
            }
        }
        return true;
    }

    std::vector<std::string> SplitRow(const std::string& row, char delimiter) {
        std::vector<std::string> Fields;
        std::string Field;
        bool InQuotes = false;

        for (char ch : row) {
            if (ch == '\"') {
                InQuotes = !InQuotes;
            } else if (ch == delimiter && !InQuotes) {
                Fields.push_back(Field);
                Field.clear();
            } else {
                Field += ch;
            }
        }

        if (!Field.empty()) {
            Fields.push_back(Field);
        }

        return Fields;
    }

    bool ReadRow(std::vector<std::string>& row) {
        row.clear();
    
        std::string AccumulatedData;
        if (!Leftover.empty()) {
            AccumulatedData = Leftover[0];
            Leftover.clear();
        }
    
        while (true) {
            std::vector<char> Buffer;
            std::size_t BufferSize = 256;
            if (!DataSource->Read(Buffer, BufferSize)) {
                if (AccumulatedData.empty()) {
                    return false;
                }
                break;
            }
    
            AccumulatedData += std::string(Buffer.begin(), Buffer.end());
    
            size_t NewlinePos = AccumulatedData.find('\n');
            if (NewlinePos != std::string::npos) {
                break;
            }
        }
    
        size_t NewlinePos = AccumulatedData.find('\n');
        std::string CurrentRow = AccumulatedData.substr(0, NewlinePos);
        if (NewlinePos != std::string::npos) {
            Leftover.push_back(AccumulatedData.substr(NewlinePos + 1));
        }
    
        row = SplitRow(CurrentRow, Delimiter);
    
        if (!CheckSpecialCase(row)) {
            return false;
        }
    
        return true;
    }

        size_t NewlinePos = AccumulatedData.find('\n');
        std::string CurrentRow = AccumulatedData.substr(0, NewlinePos);
        if (NewlinePos != std::string::npos) {
            Leftover.push_back(AccumulatedData.substr(NewlinePos + 1));
        }

        row = SplitRow(CurrentRow, Delimiter);

        if (!CheckSpecialCase(row)) {
            return false;
        }

        return true;
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