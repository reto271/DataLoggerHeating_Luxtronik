#pragma once

#include <fstream>
#include <vector>
#include <memory>

#include "DataDecoder/src/IFileDataWriterCSV.hpp"

/// Writes CSV files with the provided data. The data structure how the data must be provides is defined
///  in IFileDataWriterCSV.
class FileDataWriterCSV : public IFileDataWriterCSV
{
public:
    /// Constructs a CSV file writer object.
    /// \param[in] fileName is the name of the file the CSV data shall be written to.
    FileDataWriterCSV(std::string fileName);

    /// Closes the CSV file and tears-down the object.
    virtual ~FileDataWriterCSV();

    bool writeHeader(std::string headerLine) override;
    bool writeData(std::vector<DataEntryCSV> dataVector, const uint32_t nrColumnExclTimeStamp) override;

private:
    std::string m_fileName;    ///< File name of the CSV file.
    std::ofstream m_csvFile;   ///< File stream to write the data to.
    bool m_fileIsEmpty;        ///< True if the file is empty.
};
