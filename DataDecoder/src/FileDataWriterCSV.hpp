#pragma once

#include <fstream>
#include <vector>
#include <memory>

#include "DataDecoder/src/IFileDataWriterCSV.hpp"

class FileDataWriterCSV : public IFileDataWriterCSV
{
public:
    FileDataWriterCSV(std::string fileName);
    virtual ~FileDataWriterCSV();

    bool writeHeader(std::string headerLine) override;
    bool writeData(std::vector<std::uint32_t> dataVector, const uint32_t nrColumnExclTimeStamp) override;

private:
    std::string m_fileName;
    std::ofstream m_csvFile;
};
