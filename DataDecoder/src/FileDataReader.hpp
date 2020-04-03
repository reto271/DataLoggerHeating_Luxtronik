#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include "IFileDataWriterCSV.hpp"
#include "Common/src/CommonTypeDefs.hpp"

class IValueTable;

class FileDataReader
{
public:
    FileDataReader(std::string fileName, IFileDataWriterCSV_SPtr csvWriter, bool enableLog);
    virtual ~FileDataReader();

    bool readHeaderData();

    bool decodeData();

protected:
    void readRawDataFromFile_v1_v2();
    void readRawDataFromFile();

    bool writeToCSV(std::string headerLine);

    // Debug functions
    void printRawBuffer();

private:
    void determineFileVersion();
    void validateFile();
    void validateHeaderSize();
    void validateNrEntriesPerRecord();
    void validateFileLength();

    std::string prepareHeader();
    std::string prepareUnits();

    std::string m_fileName;
    uint32_t m_fileLength;

    // Header data
    uint32_t m_fileVersion;
    uint32_t m_sizeFileHeader;
    uint32_t m_nrDataEntriesPerRecord;
    uint32_t m_nrRecords;

    std::vector<uint32_t> m_csvBuffer;
    std::ifstream m_inputFileStream;
    std::shared_ptr<IValueTable> m_pValueTable;
    IFileDataWriterCSV_SPtr m_csvWriter;

    bool m_enableLog;
};

PointerDefinition(FileDataReader);
