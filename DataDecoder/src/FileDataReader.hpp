#pragma once

#include <iostream>

class FileDataReader
{
public:
    FileDataReader(std::string fileName);
    virtual ~FileDataReader();

    bool readFromFile();
    bool decodeBufferV1();
    uint32_t getFileVersion();

    bool writeToCSV();


    // Debug functions
    void printRawBuffer();

private:
    std::string m_fileName;
    uint32_t m_fileLength;

    //Header data
    uint32_t m_fileVersion;
    uint32_t m_sizeFileHeader;
    uint32_t m_nrDataEntriesPerRecord;
    uint32_t m_nrRecords;

    uint32_t* m_pBuffer;
};
