#include "FileDataReader.hpp"

#include <iostream>
#include <fstream>
#include <ctime>

FileDataReader::FileDataReader(std::string fileName)
    : m_fileName(fileName)
    , m_fileLength(0)
    , m_fileVersion(0)
    , m_sizeFileHeader(0)
    , m_nrDataEntriesPerRecord(0)
    , m_nrRecords(0)
    , m_pBuffer(nullptr)
{
}

FileDataReader::~FileDataReader()
{
    std::cout << "Delete buffer" << std::endl;
    delete[] m_pBuffer;
};

bool FileDataReader::readFromFile()
{
    bool feedback = false;
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
        is.read(reinterpret_cast<char*>(&m_nrDataEntriesPerRecord), 4);

        std::cout << "Header size: " << m_sizeFileHeader << std::endl;
        std::cout << "Nr data entris per record: " << m_nrDataEntriesPerRecord << std::endl;

        // File version: 1
        // File length: 2284
        // Header size: 3
        // Size single data record: 69

        uint32_t dataSizeInDoubleWords = (m_fileLength - (m_sizeFileHeader * 4)) / 4;
        m_nrRecords = dataSizeInDoubleWords / (m_nrDataEntriesPerRecord + 2);   // 2: 8bytes for the time stamp (std::time_t)
        std::cout << "Nr records : " << m_nrRecords << std::endl;

        // Allocate buffer
        uint32_t bufferSize = m_nrRecords * (m_nrDataEntriesPerRecord + 2);
        m_pBuffer = new uint32_t[bufferSize];
        is.read(reinterpret_cast<char*>(m_pBuffer), 4*bufferSize);

        for(uint32_t record = 0; record < m_nrRecords; record++) {
            for(uint32_t cnt = 0; cnt < (m_nrDataEntriesPerRecord + 2); cnt++) {
                std::cout << "[" << record << ":" <<cnt << "] " << m_pBuffer[cnt + record * (m_nrDataEntriesPerRecord + 2)] << ", ";
            }
            std::cout << std::endl << std::endl;
        }
        feedback = true;
    }
        break;
    default:
        std::cout << "File version not supported" << std::endl;
    }
    is.close();
    return feedback;
}

bool FileDataReader::decodeBufferV1()
{
    typedef struct
    {
        std::time_t sampleTime;
        int32_t data[69];
    } DataRecord;

    DataRecord* pRec = reinterpret_cast<DataRecord*>(m_pBuffer);


//    struct tm* currentLocalTime;
//    std::time_t currentUnixTime = std::time(nullptr);
//    currentLocalTime = std::localtime(&currentUnixTime);


    std::cout << "Creation Time: " << std::asctime(std::localtime(&pRec->sampleTime)) << pRec->sampleTime << " seconds since the Epoch\n";
    std::cout << "Hex time: 0x" << std::hex << pRec->sampleTime << std::dec << " seconds since the Epoch\n";


}


uint32_t FileDataReader::getFileVersion()
{
    return m_fileVersion;
}
