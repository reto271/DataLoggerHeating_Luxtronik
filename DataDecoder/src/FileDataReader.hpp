#pragma once

#include <iostream>
#include <fstream>

class FileDataReader
{
public:
    FileDataReader(std::string fileName);
    virtual ~FileDataReader();

    bool readHeaderData();

    bool decodeData();

protected:
    bool decodeDataCurrent();
    bool decodeDataV2();
    bool decodeDataV1();
    void readRawDataFromFile();
    bool writeToCSV(std::string headerLine);


    // Debug functions
    void printRawBuffer();

private:
    std::string m_fileName;
    uint32_t m_fileLength;

    // Header data
    uint32_t m_fileVersion;
    uint32_t m_sizeFileHeader;
    uint32_t m_nrDataEntriesPerRecord;
    uint32_t m_nrRecords;

    uint32_t* m_pBuffer;

    std::ifstream m_inputFileStream;

};
