#pragma once

#include <fstream>
#include <vector>

class IValueTable;

class FileDataWriterCSV
{
public:
    FileDataWriterCSV(std::string fileName);
    virtual ~FileDataWriterCSV();

    bool writeHeader(std::string headerLine);
    bool writeData(std::vector<std::uint32_t> dataVector, const uint32_t nrColumnExclTimeStamp);

private:
    std::string m_fileName;
    std::ofstream m_csvFile;
};
