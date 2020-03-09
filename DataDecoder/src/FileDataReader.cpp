#include "FileDataReader.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#include "Common/src/ValueTable.hpp"

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
    delete[] m_pBuffer;
};

bool FileDataReader::readFromFile()
{
    bool feedback = false;
    std::ifstream is;

    is.open (m_fileName, std::ios::binary | std::ios::in);
//    if (false == is.open (m_fileName, std::ios::binary | std::ios::in)) {
//        return false;
//    }

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
            // Read header data
            is.read(reinterpret_cast<char*>(&m_sizeFileHeader), 4);
            is.read(reinterpret_cast<char*>(&m_nrDataEntriesPerRecord), 4);

            std::cout << "Header size: " << m_sizeFileHeader << std::endl;
            std::cout << "No data entries per record: " << m_nrDataEntriesPerRecord << std::endl;

            uint32_t dataSizeInDoubleWords = (m_fileLength - (m_sizeFileHeader * 4)) / 4;
            m_nrRecords = dataSizeInDoubleWords / (m_nrDataEntriesPerRecord + 2); // 2: 8bytes for the time stamp (std::time_t)
            std::cout << "No records : " << m_nrRecords << std::endl;

            // Allocate buffer and read from file
            uint32_t bufferSize = m_nrRecords * (m_nrDataEntriesPerRecord + 2);
            m_pBuffer = new uint32_t[bufferSize];
            is.read(reinterpret_cast<char*>(m_pBuffer), 4*bufferSize);

            //printRawBuffer();

            feedback = true;
        }
        break;
    default:
        std::cout << "File version not supported" << std::endl;
        break;
    }
    is.close();
    return feedback;
}

bool FileDataReader::decodeBufferV1()
{
    #if 0
    for(uint32_t record = 0; record < m_nrRecords; record++) {
        BufferDataRecord* pRec = reinterpret_cast<BufferDataRecord*>(&m_pBuffer[record * (m_nrDataEntriesPerRecord + 2)]);
        std::cout << pRec->timeData.sampleTime << " seconds since the Epoch, Hex time: 0x" << std::hex << pRec->timeData.sampleTime << std::dec
                  << " seconds since the Epoch, Creation Time: " << std::asctime(std::localtime(&pRec->timeData.sampleTime));
        for (uint32_t cnt = 0; cnt < m_nrDataEntriesPerRecord; cnt++) {
            std::cout << cnt << " : " << pRec->timeData.data[cnt] << ", " << ValueTableDecode[cnt].description << std::endl;
        }
    }
    #endif
    return true;
}


uint32_t FileDataReader::getFileVersion()
{
    return m_fileVersion;
}

bool FileDataReader::writeToCSV()
{
    std::ofstream csvFile;
    std::string outputFileName = m_fileName + ".csv";

    csvFile.open(outputFileName, std::ios::out);

    // Write the header line
    csvFile << "Time";
    for (uint32_t cnt = 0; cnt < m_nrDataEntriesPerRecord; cnt++) {
        csvFile << ", " << ValueTableDecode[cnt].description;
    }
    csvFile << std::endl;

    // Write data
    for(uint32_t record = 0; record < m_nrRecords; record++) {
        BufferDataRecord* pRec = reinterpret_cast<BufferDataRecord*>(&m_pBuffer[record * (m_nrDataEntriesPerRecord + 2)]);
        csvFile << pRec->timeData.sampleTime;
        for (uint32_t cnt = 0; cnt < m_nrDataEntriesPerRecord; cnt++) {
            csvFile << ", " << std::right << std::setw(9) << pRec->timeData.data[cnt];
        }
        csvFile << std::endl;
    }
    csvFile.close();
    std::cout << "File '" << outputFileName << "' successfully written" << std::endl;
    return true;
}

void FileDataReader::printRawBuffer()
{
    for(uint32_t record = 0; record < m_nrRecords; record++) {
        for(uint32_t cnt = 0; cnt < (m_nrDataEntriesPerRecord + 2); cnt++) {
            std::cout << "[" << record << ":" <<cnt << "] " << m_pBuffer[cnt + record * (m_nrDataEntriesPerRecord + 2)] << ", ";
        }
        std::cout << std::endl << std::endl;
    }
}
