#include "DSVReader.h"
#include "StringUtils.h"
#include <memory>
#include <vector>
#include <string>

// Define the SImplementation struct
struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DataSource;
    std::vector<std::string> Leftover;
    char Delimiter;

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : DataSource(src), Delimiter(delimiter) {}

    // Check if the data source has reached the end and no leftover data exists
    bool End() const {
        return DataSource->End() && Leftover.empty();
    }

    // Handle special cases like quoted fields and escaped quotes
    bool CheckSpecialCase(std::vector<std::string>& row) {
        for (size_t i = 0; i < row.size(); i++) {
            if (row[i].empty()) {
                continue; // Skip empty fields
            }
            if (row[i][0] == '\"') {
                // If the field starts with a quote, it must end with a quote
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

    // Split a row into fields, handling quoted fields and embedded delimiters
    std::vector<std::string> SplitRow(const std::string& row, char delimiter) {
        std::vector<std::string> Fields;
        std::string Field;
        bool InQuotes = false;

        for (size_t i = 0; i < row.size(); i++) {
            char ch = row[i];
            if (ch == '\"') {
                InQuotes = !InQuotes; // Toggle quote state
            } else if (ch == delimiter && !InQuotes) {
                Fields.push_back(Field);
                Field.clear();
            } else {
                Field += ch;
            }
        }

        // Add the last field
        Fields.push_back(Field);

        return Fields;
    }

    // Read a row from the data source
    bool ReadRow(std::vector<std::string>& row) {
        row.clear();

        // Accumulate data from the source until we find a complete row
        std::string AccumulatedData;
        if (!Leftover.empty()) {
            AccumulatedData = Leftover[0];
            Leftover.clear();
        }

        while (true) {
            // Read from the data source
            std::vector<char> Buffer;
            std::size_t BufferSize = 256;
            if (!DataSource->Read(Buffer, BufferSize)) {
                if (AccumulatedData.empty()) {
                    return false; // No more data
                }
                break; // Process the remaining data
            }

            // Append the new data to the accumulated data
            AccumulatedData += std::string(Buffer.begin(), Buffer.end());

            // Check if we have a complete row (ends with a newline)
            size_t NewlinePos = AccumulatedData.find('\n');
            if (NewlinePos != std::string::npos) {
                break; // We have a complete row
            }
        }

        // Split the accumulated data into rows
        size_t NewlinePos = AccumulatedData.find('\n');
        std::string CurrentRow = (NewlinePos != std::string::npos) ? AccumulatedData.substr(0, NewlinePos) : AccumulatedData;
        if (NewlinePos != std::string::npos) {
            Leftover.push_back(AccumulatedData.substr(NewlinePos + 1));
        }

        // Split the row by the delimiter
        row = SplitRow(CurrentRow, Delimiter);

        // Check for special cases (e.g., mismatched quotes)
        if (!CheckSpecialCase(row)) {
            return false; // Mismatched quotes
        }

        return true;
    }
};

// Constructor
CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter) {
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
}

// Destructor
CDSVReader::~CDSVReader() = default;

// End() function
bool CDSVReader::End() const {
    return DImplementation->End();
}

// ReadRow() function
bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    return DImplementation->ReadRow(row);
}