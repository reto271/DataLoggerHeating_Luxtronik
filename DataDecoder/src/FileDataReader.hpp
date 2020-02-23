#pragma once

#include <iostream>

class FileDataReader
{
public:
    FileDataReader(std::string fileName);

    void readFromFile();

    uint32_t getFileVersion();

private:
    std::string m_fileName;
    uint32_t m_fileLength;

    //Header data
    uint32_t m_fileVersion;
    uint32_t m_sizeFileHeader;
    uint32_t m_nrDataEntries;
};
