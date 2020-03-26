#include "FileDataReader.hpp"

#include <iomanip>
#include <ctime>
#include <assert.h>

#include "Common/src/ValueTable.hpp"
#include "Common/src/FeedbackCollector.hpp"

FileDataReader::FileDataReader(std::string fileName)
    : m_fileName(fileName)
    , m_fileLength(0)
    , m_fileVersion(0)
    , m_sizeFileHeader(0)
    , m_nrDataEntriesPerRecord(0)
    , m_nrRecords(0)
    , m_pBuffer(nullptr)
{
    m_inputFileStream.open (m_fileName, std::ios::binary | std::ios::in);
}

FileDataReader::~FileDataReader()
{
    delete[] m_pBuffer;
    m_inputFileStream.close();
};

bool FileDataReader::readHeaderData()
{
    bool feedback = true;


    if (false == m_inputFileStream.is_open ()) {
        std::cout << "Could not find file: '" << m_fileName << "'" << std::endl;
        return false;
    }

    // Get length of file:
    m_inputFileStream.seekg (0, std::ios::end);
    m_fileLength = m_inputFileStream.tellg();
    m_inputFileStream.seekg (0, std::ios::beg);

    // Read the file header
    m_inputFileStream.read(reinterpret_cast<char*>(&m_fileVersion), 4);
    m_inputFileStream.read(reinterpret_cast<char*>(&m_sizeFileHeader), 4);
    m_inputFileStream.read(reinterpret_cast<char*>(&m_nrDataEntriesPerRecord), 4);

    std::cout << "File version:       " << m_fileVersion << std::endl;
    std::cout << "Header size:        " << m_sizeFileHeader << std::endl;
    std::cout << "Entries per Record: " << m_nrDataEntriesPerRecord << std::endl;
    std::cout << "File length:        " << m_fileLength << std::endl;

    // Conistency check for the detected file version
    switch(m_fileVersion)
    {
    //current file version
    case FILE_Version:
        {
            FeedbackCollector collectFeedback(feedback);
            collectFeedback.addAndFeedback(m_sizeFileHeader == FILE_SizeOfHeader);
            collectFeedback.addAndFeedback(m_nrDataEntriesPerRecord == FILE_NrDataEntries);
            feedback = collectFeedback.getFeedback();
        }
        break;

    case FILE_Version_v1:
        {
            FeedbackCollector collectFeedback(feedback);
            collectFeedback.addAndFeedback(m_sizeFileHeader == FILE_SizeOfHeader_v1);
            collectFeedback.addAndFeedback(m_nrDataEntriesPerRecord == FILE_NrDataEntries_v1);
            feedback = collectFeedback.getFeedback();
        }
        break;


    default:
        std::cout << "File version not supported" << std::endl;
        feedback = false;
        break;
    }

    return feedback;
}

bool FileDataReader::decodeData()
{
    bool feedback = false;
    switch(m_fileVersion)
    {
    //current file version
    case FILE_Version:
        feedback = decodeDataCurrent();
        break;

    case FILE_Version_v1:
        feedback = decodeDataV1();
        break;

    default:
        break;
    }
    return feedback;
}

bool FileDataReader::decodeDataCurrent()
{
    // There is no structural change, therefore the v1 may be used.
    return decodeDataV1();
}

bool FileDataReader::decodeDataV1()
{
    uint32_t dataSizeInDoubleWords = (m_fileLength - (m_sizeFileHeader * 4)) / 4;
    m_nrRecords = dataSizeInDoubleWords / (m_nrDataEntriesPerRecord + 2);         // 2: 8bytes for the time stamp (std::time_t)
    std::cout << "No records : " << m_nrRecords << std::endl;

    // Allocate buffer and read from file
    uint32_t bufferSize = m_nrRecords * (m_nrDataEntriesPerRecord + 2);
    assert(nullptr == m_pBuffer);
    m_pBuffer = new uint32_t[bufferSize];
    m_inputFileStream.read(reinterpret_cast<char*>(m_pBuffer), 4 * bufferSize);

    //printRawBuffer();

    // Prepare the header line
    std::string headerLine = "Time";
    for (uint32_t cnt = 0; cnt < m_nrDataEntriesPerRecord; cnt++) {
        headerLine += ", ";
        headerLine += ValueTableDecode_v1[cnt].description;
    }

    return writeToCSV(headerLine);
}

bool FileDataReader::writeToCSV(std::string headerLine)
{

    std::ofstream csvFile;
    std::string outputFileName = m_fileName + ".csv";

    csvFile.open(outputFileName, std::ios::out);

    // Write the header line
    csvFile << headerLine << std::endl;

    // Write data
    uint32_t arrayPos = 0;
    for(uint32_t record = 0; record < m_nrRecords; record++) {
        std::time_t time = static_cast<std::time_t>(m_pBuffer[arrayPos]);
        arrayPos++;
        time += static_cast<std::time_t>(m_pBuffer[arrayPos]) << 32;
        arrayPos++;
        csvFile << time;
        for (uint32_t cnt = 0; cnt < m_nrDataEntriesPerRecord; cnt++) {
            csvFile << ", " << std::right << std::setw(9) << *(reinterpret_cast<int32_t*>(&m_pBuffer[arrayPos]));
            arrayPos++;
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
            std::cout << "[" << record << ":" << cnt << "] "
                      << *(reinterpret_cast<int32_t*>(m_pBuffer[cnt + record * (m_nrDataEntriesPerRecord + 2)])) << ", ";
        }
        std::cout << std::endl << std::endl;
    }
}
