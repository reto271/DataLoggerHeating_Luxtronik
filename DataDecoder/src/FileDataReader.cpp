#include "FileDataReader.hpp"

#include <iostream>
#include <fstream>

FileDataReader::FileDataReader(std::string fileName)
    : m_fileName(fileName)
    , m_fileLength(0)
    , m_fileVersion(0)
    , m_sizeFileHeader(0)
    , m_nrDataEntries(0)
{
}

void FileDataReader::readFromFile()
{
    std::ifstream is;
    is.open (m_fileName, std::ios::binary | std::ios::in);

    // Get length of file:
    is.seekg (0, std::ios::end);
    m_fileLength = is.tellg();
    is.seekg (0, std::ios::beg);

    // Read the file version
    is.read(reinterpret_cast<char*>(&m_fileVersion), 4);
    std::cout << "File version: " << m_fileVersion << std::endl;
    std::cout << "File length: " << m_fileLength << std::endl;

    switch(m_fileVersion)
    {
    case 1:
    {
        //wf.write(reinterpret_cast<char*>(&header.Version), 4);
        //wf.write(reinterpret_cast<char*>(&header.SizeOfHeader), 4);
        //wf.write(reinterpret_cast<char*>(&header.NrDataEntries), 4);
        is.read(reinterpret_cast<char*>(&m_sizeFileHeader), 4);
        is.read(reinterpret_cast<char*>(&m_nrDataEntries), 4);

        std::cout << "Header size: " << m_sizeFileHeader << std::endl;
        std::cout << "Size single data record: " << m_nrDataEntries << std::endl;

        // File version: 1
        // File length: 2284
        // Header size: 3
        // Size single data record: 69

        uint32_t dataSizeInDoubleWords = (m_fileLength - (m_sizeFileHeader * 4)) / 4;
        uint32_t nrDataRecords = dataSizeInDoubleWords / (m_nrDataEntries + 2);   // 2: 8bytes for the time stamp (std::time_t)
        std::cout << "Nr data records : " << nrDataRecords << std::endl;
    }
        break;
    default:
        std::cout << "File version not supported" << std::endl;
    }
//    // Allocate memory
//    buffer = new char[length];
//
//    // read data as a block:
//    is.read (buffer, length);
    is.close();
}

uint32_t FileDataReader::getFileVersion()
{
    return 0;
}
