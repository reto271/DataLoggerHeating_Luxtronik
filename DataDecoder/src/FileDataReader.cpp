#include "FileDataReader.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <assert.h>
#include <cstring>

#include "Common/src/ValueTable.hpp"
#include "Common/src/ValueTable_v1.hpp"
#include "Common/src/ValueTable_v2.hpp"
#include "Common/src/FeedbackCollector.hpp"
#include "Common/src/BitBuffer.hpp"

FileDataReader::FileDataReader(std::string fileName, IFileDataWriterCSV_SPtr csvWriter, bool enableLog)
    : m_fileName(fileName)
    , m_fileLength(0)
    , m_fileVersion(0)
    , m_sizeFileHeader(0)
    , m_nrDataEntriesPerRecord(0)
    , m_nrRecords(0)
    , m_pValueTable(nullptr)
    , m_csvWriter(csvWriter)
    , m_enableLog(enableLog)
{
    m_inputFileStream.open (m_fileName, std::ios::binary | std::ios::in);
}

FileDataReader::~FileDataReader()
{
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

    if(true == m_enableLog) {
        std::cout << "File version:       " << m_fileVersion << std::endl;
        std::cout << "Header size:        " << m_sizeFileHeader << std::endl;
        std::cout << "Entries per Record: " << m_nrDataEntriesPerRecord << std::endl;
        std::cout << "File length:        " << m_fileLength << std::endl;
    }
    // Find the file version
    determineFileVersion();

    // Some validation checks
    validateFile();

    if(nullptr == m_pValueTable) {
        std::cout << "No matching value table found. (Fnc: " << __FUNCTION__ << ", ln: " << __LINE__ << ")" << std::endl;
        return false;
    } else {
        return true;
    }
}

bool FileDataReader::decodeData()
{
    if(nullptr == m_pValueTable) {
        std::cout << "No matching value table found. (Fnc: " << __FUNCTION__ << ", ln: " << __LINE__ << ")" << std::endl;
        return false;
    }

    // Write header line
    std::string headerLine = prepareHeader();
    std::string unitLine = prepareUnits();

    switch(m_pValueTable->getFileVersion()) {
        case 1:
        case 2:
            readRawDataFromFile_v1_v2();
            break;
        default:
            readRawDataFromFile();
            break;
    }

    FeedbackCollector writeFeedback(true);
    writeFeedback.addAndFeedback(m_csvWriter->writeHeader(headerLine));
    writeFeedback.addAndFeedback(m_csvWriter->writeHeader(unitLine));
    writeFeedback.addAndFeedback(m_csvWriter->writeData(m_csvBuffer, m_pValueTable->getNrDataEntriesPerSet()));
    return writeFeedback.getFeedback();
}

void FileDataReader::readRawDataFromFile_v1_v2()
{
    uint32_t dataSizeInDoubleWords = (m_fileLength - (m_sizeFileHeader * 4)) / 4;
    m_nrRecords = dataSizeInDoubleWords / (m_pValueTable->getNrDataEntriesPerSet() + 2);                 // 2: 8bytes for the time stamp (std::time_t)
    if(true == m_enableLog) {
        std::cout << "No records : " << m_nrRecords << std::endl;
    }
    // Allocate buffer and read from file
    uint32_t bufferSize = m_nrRecords * (m_nrDataEntriesPerRecord + 2); // 2 * 4 bytes time stamp

    uint32_t* pBuffer = new uint32_t[bufferSize];

    m_inputFileStream.read(reinterpret_cast<char*>(pBuffer), 4 * bufferSize);
    m_csvBuffer = std::vector<IFileDataWriterCSV::DataEntryCSV>(bufferSize);
    uint32_t pos = 0;
    for(uint32_t recNr = 0; recNr < m_nrRecords; recNr++) {
        for(uint32_t column = 0; column < (m_nrDataEntriesPerRecord + 2); column++) {
            if(2 > column) {
                // Time information
                m_csvBuffer.at(pos).uiVal = pBuffer[pos];
                m_csvBuffer.at(pos).divisor = 1;
            } else {
                m_csvBuffer.at(pos).uiVal = pBuffer[pos];
                m_csvBuffer.at(pos).divisor = m_pValueTable->getConversionDivisor(column - 2);
            }
            pos++;
        }
    }
    delete[] pBuffer;
}

void FileDataReader::readRawDataFromFile()
{
    uint32_t headerSize = 4 * m_pValueTable->getSizeOfHeader();
    uint32_t dataFileLength = m_fileLength - headerSize;
    uint32_t dataSetSize = m_pValueTable->getNrBytesInBufferPerSet();
    m_nrRecords = dataFileLength / dataSetSize;

    if(true == m_enableLog) {
        std::cout << "m_nrRecords: " << m_nrRecords << std::endl;
        std::cout << "m_nrDataEntriesPerRecord: " << m_nrDataEntriesPerRecord << std::endl;
        std::cout << "m_pValueTable->getNrDataEntriesPerSet(): " << m_pValueTable->getNrDataEntriesPerSet() << std::endl;
        std::cout << "m_fileLength: " << m_fileLength << std::endl;
    }
    // Prepare buffer used for writeToCSV function
    uint32_t bufferSize = m_nrRecords * (m_pValueTable->getNrDataEntriesPerSet() + 2);  // 2 * sizeof(uin32_t) for the time stamp
    if(true == m_enableLog) {
        std::cout << "bufferSize: " << bufferSize << std::endl;
    }
    m_csvBuffer.resize(bufferSize);

    // Read the whole file
    uint8_t* pU8RawData = new uint8_t[m_fileLength];
    uint8_t* pU8RawDataOri = pU8RawData;
    m_inputFileStream.read(reinterpret_cast<char*>(pU8RawData), m_fileLength);

    // std::cout << "---------------------------" << std::endl;
    // {
    //    std::ios state(nullptr);
    //    state.copyfmt(std::cout); // save current formatting
    //    std::cout << "0x0000 :";
    //    for(uint32_t i = 0; i < dataFileLength; i++) {
    //        std::cout << std::hex << " 0x" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(pU8RawData[i]);
    //        if(0 == ((i + 1) % 32)) {
    //            std::cout << std::endl << "0x" << std::setw(4) << i + 1 << " :";
    //        }
    //    }
    //    std::cout.copyfmt(state); // restore previous formatting
    // }
    // std::cout << std::endl << "---------------------------" << std::endl;

    uint32_t posInBuffer = 0;
    for(uint32_t frameCnt = 0; frameCnt < m_nrRecords; frameCnt++) {
        // -  // the iterator constructor can also be used to construct from arrays:
        // -  int myints[] = {16,2,77,29};
        // -  std::vector<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );
        std::vector<uint8_t> dataVector(pU8RawData, pU8RawData + dataSetSize);

        // Convert to data vector
        BitBuffer bitBuffer(dataVector);
        // bitBuffer.printContent();

        // Get time value
        uint32_t val;
        bitBuffer.getValue(val, 32);
        m_csvBuffer.at(posInBuffer).uiVal = val;
        m_csvBuffer.at(posInBuffer).divisor = 1;
        posInBuffer++;
        bitBuffer.getValue(val, 32);
        m_csvBuffer.at(posInBuffer).uiVal = val;
        m_csvBuffer.at(posInBuffer).divisor = 1;
        posInBuffer++;

        for(uint32_t cnt = 0; cnt < m_pValueTable->getNrDataEntriesPerSet(); cnt++) {
            switch(m_pValueTable->getDataTypeInfo(cnt)) {
                case DataTypeInfo::UNSIGNED:
                case DataTypeInfo::BOOL:
                    {
                        uint32_t val;
                        bitBuffer.getValue(val, m_pValueTable->getNrBitsInBuffer(cnt));
                        m_csvBuffer.at(posInBuffer).uiVal = val;
                        m_csvBuffer.at(posInBuffer).divisor = m_pValueTable->getConversionDivisor(cnt);
                    }
                    break;


                case DataTypeInfo::SIGNED:
                    {
                        int32_t iVal;
                        bitBuffer.getValue(iVal, m_pValueTable->getNrBitsInBuffer(cnt));
                        m_csvBuffer.at(posInBuffer).uiVal = iVal;
                        m_csvBuffer.at(posInBuffer).divisor = m_pValueTable->getConversionDivisor(cnt);
                    }
                    break;
                default:
                    assert(0);
                    break;
            }
            posInBuffer++;
        }
        pU8RawData += dataSetSize;
        // Iterate through BitBuffer
    }
    delete[] pU8RawDataOri;
}

void FileDataReader::printRawBuffer()
{
    for(uint32_t record = 0; record < m_nrRecords; record++) {
        for(uint32_t cnt = 0; cnt < (m_pValueTable->getNrDataEntriesPerSet() + 2); cnt++) {
            // Is it an unsigned value?
            auto* uVal = std::get_if<uint32_t>(&m_csvBuffer.at(cnt + record * (m_pValueTable->getNrDataEntriesPerSet() + 2)).uiVal);
            if (nullptr != uVal) {
                std::cout << "[" << record << ":" << cnt << "] " << *uVal << ", ";
            } else {
                // Test if it is truly a signed value
                auto* iVal = std::get_if<int32_t>(&m_csvBuffer.at(cnt + record * (m_pValueTable->getNrDataEntriesPerSet() + 2)).uiVal);
                if (nullptr != iVal) {
                    std::cout << "[" << record << ":" << cnt << "] " << *(reinterpret_cast<int32_t*>(iVal)) << ", ";
                } else {
                    assert(0);
                }
            }
        }
        std::cout << std::endl << std::endl;
    }
}

void FileDataReader::determineFileVersion()
{
    // Test if current version
    m_pValueTable = std::make_shared<ValueTable>(m_enableLog);
    if(m_fileVersion == m_pValueTable->getFileVersion()) {
        m_pValueTable->initialize();
        return;
    }

    // Test older versions
    m_pValueTable = std::make_shared<ValueTable_v2>(m_enableLog);
    if(m_fileVersion == m_pValueTable->getFileVersion()) {
        m_pValueTable->initialize();
        return;
    }

    m_pValueTable = std::make_shared<ValueTable_v1>(m_enableLog);
    if(m_fileVersion == m_pValueTable->getFileVersion()) {
        m_pValueTable->initialize();
        return;
    }

    m_pValueTable.reset();
    assert(nullptr == m_pValueTable);
}

void FileDataReader::validateFile()
{
    if(nullptr == m_pValueTable) {
        std::cout << "No matching value table found. (Fnc: " << __FUNCTION__ << ", ln: " << __LINE__ << ")" << std::endl;
        return;
    }

    // Conistency check for the detected file version
    validateHeaderSize();
    validateNrEntriesPerRecord();
    validateFileLength();
}

void FileDataReader::validateHeaderSize()
{
    if(nullptr == m_pValueTable) {
        std::cout << "No matching value table found. (Fnc: " << __FUNCTION__ << ", ln: " << __LINE__ << ")" << std::endl;
        return;
    }
    if(m_sizeFileHeader != m_pValueTable->getSizeOfHeader()) {
        std::cout << "Header size does not match: "
                  << "m_sizeFileHeader: " << m_sizeFileHeader
                  << ", m_pValueTable->getSizeOfHeader(): " << m_pValueTable->getSizeOfHeader() << std::endl;
        m_pValueTable.reset();
        assert(nullptr == m_pValueTable);
    }
}

void FileDataReader::validateNrEntriesPerRecord()
{
    if(nullptr == m_pValueTable) {
        std::cout << "No matching value table found. (Fnc: " << __FUNCTION__ << ", ln: " << __LINE__ << ")" << std::endl;
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
            }
            break;
        default:
            if(m_nrDataEntriesPerRecord != m_pValueTable->getNrBytesInBufferPerSet()) {
                std::cout << "Number entries per data set does not match: "
                          << "m_nrDataEntriesPerRecord: " << m_nrDataEntriesPerRecord
                          << ", m_pValueTable->getNrBytesInBufferPerSet(): " << m_pValueTable->getNrBytesInBufferPerSet() << std::endl;
                m_pValueTable.reset();
                assert(nullptr == m_pValueTable);
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

    if(nullptr == m_pValueTable) {
        std::cout << "No matching value table found. (Fnc: " << __FUNCTION__ << ", ln: " << __LINE__ << ")" << std::endl;
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
    if(0 != dataFileLength % dataSetSize) {
        std::cout << "Data length in file is not modulo of 'number data entreis per set'" << std::endl
                  << "  headerSize: " << headerSize << std::endl
                  << "  m_fileLength: " << m_fileLength << std::endl
                  << "  dataFileLength: " << dataFileLength << std::endl
                  << "  dataSetSize: " << dataSetSize << std::endl;
        m_pValueTable.reset();
        assert(nullptr == m_pValueTable);
    }
}

std::string FileDataReader::prepareHeader()
{
    // Prepare the header line
    std::string headerLine = "Time";
    for(uint32_t cnt = 0; cnt < m_pValueTable->getNrDataEntriesPerSet(); cnt++) {
        headerLine += ",";
        headerLine += m_pValueTable->getDescription(cnt);

    }
    return headerLine;
}

std::string FileDataReader::prepareUnits()
{
    // Prepare the header line
    std::string units = "-";
    for(uint32_t cnt = 0; cnt < m_pValueTable->getNrDataEntriesPerSet(); cnt++) {
        units += ",";
        units += m_pValueTable->getUnit(cnt);

    }
    return units;
}
