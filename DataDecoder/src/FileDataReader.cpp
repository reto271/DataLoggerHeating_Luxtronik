#include "FileDataReader.hpp"

#include <iomanip>
#include <ctime>
#include <assert.h>

#include "Common/src/ValueTable.hpp"
#include "Common/src/ValueTable_v1.hpp"
#include "Common/src/ValueTable_v2.hpp"
#include "Common/src/FeedbackCollector.hpp"

FileDataReader::FileDataReader(std::string fileName)
    : m_fileName(fileName)
    , m_fileLength(0)
    , m_fileVersion(0)
    , m_sizeFileHeader(0)
    , m_nrDataEntriesPerRecord(0)
    , m_nrRecords(0)
    , m_pBuffer(nullptr)
    , m_pValueTable(nullptr)
{
    m_inputFileStream.open (m_fileName, std::ios::binary | std::ios::in);
}

FileDataReader::~FileDataReader()
{
    delete[] m_pBuffer;
    m_inputFileStream.close();
}

bool FileDataReader::readHeaderData()
{
    if(false == m_inputFileStream.is_open ()) {
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

    determineFileVersion();


    validateFile();


    if(nullptr == m_pValueTable) {
    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        return false;
    } else {
    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        return true;
    }
}

bool FileDataReader::decodeData()
{
    if(nullptr == m_pValueTable) {
    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        return false;
    }

    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
    std::string headerLine = prepareHeader();

    switch(m_pValueTable->getFileVersion()) {
        case 1:
        case 2:
            readRawDataFromFile_v1_v2();
            break;
    default:
            readRawDataFromFile();
        break;
    }

    return writeToCSV(headerLine);
}



void FileDataReader::readRawDataFromFile_v1_v2()
{
    uint32_t dataSizeInDoubleWords = (m_fileLength - (m_sizeFileHeader * 4)) / 4;
    m_nrRecords = dataSizeInDoubleWords / (m_nrDataEntriesPerRecord + 2);                 // 2: 8bytes for the time stamp (std::time_t)
    std::cout << "No records : " << m_nrRecords << std::endl;

    // Allocate buffer and read from file
    uint32_t bufferSize = m_nrRecords * (m_nrDataEntriesPerRecord + 2);
    assert(nullptr == m_pBuffer);
    m_pBuffer = new uint32_t[bufferSize];
    m_inputFileStream.read(reinterpret_cast<char*>(m_pBuffer), 4 * bufferSize);
}

void FileDataReader::readRawDataFromFile()
{
    assert(0); // not yet implemented
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
        for(uint32_t cnt = 0; cnt < m_nrDataEntriesPerRecord; cnt++) {
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

void FileDataReader::determineFileVersion()
{
    // Test if current version
    m_pValueTable = std::make_shared<ValueTable>();
    if(m_fileVersion == m_pValueTable->getFileVersion()) {
        std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        m_pValueTable->initialize();
        return;
    }

    // Test older versions
    m_pValueTable = std::make_shared<ValueTable_v2>();
    if(m_fileVersion == m_pValueTable->getFileVersion()) {
        std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        m_pValueTable->initialize();
        return;
    }

    m_pValueTable = std::make_shared<ValueTable_v1>();
    if(m_fileVersion == m_pValueTable->getFileVersion()) {
        std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        m_pValueTable->initialize();
        return;
    }

    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
    m_pValueTable.reset();
    assert(nullptr == m_pValueTable);
}

void FileDataReader::validateFile()
{
    if(nullptr == m_pValueTable) {
        std::cout << "No table found in FileDataReader::" << __FUNCTION__ << std::endl;
        return;
    }

    // Conistency check for the detected file version
    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
    validateHeaderSize();
    validateNrEntriesPerRecord();
    validateFileLength();
}

#if 0
switch(m_fileVersion) {
    // current file version
    case FILE_Version:
        {
            FeedbackCollector collectFeedback(feedback);
            std::cout << "fb: " << collectFeedback.getFeedback() << ", @ ln: " << __LINE__ << std::endl;
            collectFeedback.addAndFeedback(m_sizeFileHeader == FILE_SizeOfHeader);
            std::cout << "fb: " << collectFeedback.getFeedback() << ", @ ln: " << __LINE__ << std::endl;

            std::cout << "m_nrDataEntriesPerRecord " << m_nrDataEntriesPerRecord << " == FILE_NrDataEntries " << FILE_NrDataEntries << std::endl;

            collectFeedback.addAndFeedback(m_nrDataEntriesPerRecord == FILE_NrDataEntries);
            std::cout << "fb: " << collectFeedback.getFeedback() << ", @ ln: " << __LINE__ << std::endl;
            feedback = collectFeedback.getFeedback();
            std::cout << "fb: " << collectFeedback.getFeedback() << ", @ ln: " << __LINE__ << std::endl;
        }
        break;

    case FILE_Version_v2:
        {
            FeedbackCollector collectFeedback(feedback);
            collectFeedback.addAndFeedback(m_sizeFileHeader == FILE_SizeOfHeader_v2);
            collectFeedback.addAndFeedback(m_nrDataEntriesPerRecord == FILE_NrDataEntries_v2);
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
        std::cout << "-> File version not supported" << std::endl;
        feedback = false;
        break;
}
}
#endif

void FileDataReader::validateHeaderSize()
{
    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;    if(nullptr == m_pValueTable) {
        std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        return;
    }
    if(m_sizeFileHeader != m_pValueTable->getSizeOfHeader()) {
        std::cout << "Header size does not match: "
                  << "m_sizeFileHeader: " << m_sizeFileHeader
                  << ", m_pValueTable->getSizeOfHeader(): " << m_pValueTable->getSizeOfHeader() << std::endl;
        m_pValueTable.reset();
        assert(nullptr == m_pValueTable);
        return;
    } else {
        std::cout << "works: @ ln" << __LINE__ << std::endl;
    }
}

void FileDataReader::validateNrEntriesPerRecord()
{
    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
    if(nullptr == m_pValueTable) {
        std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        return;
    }

    switch(m_pValueTable->getFileVersion()) {
        case 1:
        case 2:

            if(m_nrDataEntriesPerRecord != m_pValueTable->getNrDataEntriesPerSet()) {
                std::cout << "Number entries per data set does not match: "
                          << "m_nrDataEntriesPerRecord: " << m_nrDataEntriesPerRecord
                          << ", m_pValueTable->getNrDataEntriesPerSet(): " << m_pValueTable->getNrDataEntriesPerSet() << std::endl;
                m_pValueTable.reset();
                assert(nullptr == m_pValueTable);
                return;
            } else {
                std::cout << "works: @ ln" << __LINE__ << std::endl;
            }
            break;
        default:
            if(m_nrDataEntriesPerRecord != m_pValueTable->getNrBytesInBufferPerSet()) {
                std::cout << "Number entries per data set does not match: "
                          << "m_nrDataEntriesPerRecord: " << m_nrDataEntriesPerRecord
                          << ", m_pValueTable->getNrBytesInBufferPerSet(): " << m_pValueTable->getNrBytesInBufferPerSet() << std::endl;
                m_pValueTable.reset();
                assert(nullptr == m_pValueTable);
                return;
            } else {
                std::cout << "works: @ ln" << __LINE__ << std::endl;
            }
            break;
    }

}

void FileDataReader::validateFileLength()
{
    // Validate file size: ALL CALCULATIONS IN BYTES !!!

    uint32_t headerSize;
    uint32_t dataFileLength;
    uint32_t dataSetSize;

    std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
    if(nullptr == m_pValueTable) {
        std::cout << "- FileDataReader::" << __FUNCTION__ << ", ln: " << __LINE__ << std::endl;
        return;
    }

    switch(m_pValueTable->getFileVersion()) {
        case 1:
        case 2:
            headerSize = 4 * m_pValueTable->getSizeOfHeader();
            dataFileLength = m_fileLength - headerSize;
            dataSetSize = 4 * m_pValueTable->getNrDataEntriesPerSet() + 8;
            break;
        default:

            headerSize = 4 * m_pValueTable->getSizeOfHeader();
            dataFileLength = m_fileLength - headerSize;
            dataSetSize = m_pValueTable->getNrBytesInBufferPerSet();
            break;
    }
    if (0 != dataFileLength % dataSetSize) {

        std::cout << "Data length in file is not modulo of 'number data entreis per set'" << std::endl
                  << "  headerSize: " << headerSize << std::endl
                  << "  m_fileLength: " << m_fileLength << std::endl
                  << "  dataFileLength: " << dataFileLength << std::endl
                  << "  dataSetSize: " << dataSetSize << std::endl;
        m_pValueTable.reset();
        assert(nullptr == m_pValueTable);
        return;
    } else {
        std::cout << "works: @ ln" << __LINE__ << std::endl;
    }
}

std::string FileDataReader::prepareHeader()
{
    // Prepare the header line
    std::string headerLine = "Time";
    for(uint32_t cnt = 0; cnt < m_pValueTable->getNrDataEntriesPerSet(); cnt++) {
        headerLine += ", ";
        headerLine += m_pValueTable->getDescription(cnt);

    }
    return headerLine;
}
