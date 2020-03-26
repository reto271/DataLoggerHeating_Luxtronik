#include "ValueResponse.hpp"

#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <assert.h>

#include "Common/src/ValueTable.hpp"
#include "Common/src/BitBuffer.hpp"

float conversionFunctionDivisor(const uint32_t value, const uint32_t divisor)
{
    return (static_cast<float>(value) / static_cast<float>(divisor));
}

bool conversionFunctionBool(const uint32_t value)
{
    if(0 == value) {
        return false;
    } else {
        return true;
    }
}

// ----------------------------------------------------------------------------------

ValueResponse::ValueResponse(RecDataStoragePtr receiveDataPtr, std::time_t currentUnixTime)
    : m_responsePtr(receiveDataPtr)
    , m_currentUnixTime(currentUnixTime)
    , m_bytesPerDataSetInclTimeStamp(0)
{
    std::cout << "Current time: " << currentUnixTime << " / 0x" << std::hex << m_currentUnixTime << std::dec << " --- " << std::asctime(std::localtime(&currentUnixTime));

    // Calculate total number of bits and required bytes per data set, including the time stamp.
    uint32_t totalNrBits = 0;
    for(uint32_t cnt = 0; cnt < getNumberOfEntries(); cnt++) {
        uint32_t nrBits = ValueTableDecode[cnt].nrBits;
        totalNrBits += nrBits;
    }
    m_bytesPerDataSetInclTimeStamp = (((totalNrBits - 1) / 8) + 1) + 8; // 8 is the time stamp
    std::cout << "Nr bits per data set: " << totalNrBits << ", bytes per data set: " << m_bytesPerDataSetInclTimeStamp << ", number of values per data set: " << getNumberOfEntries() << std::endl;

    if(false == doesFileExist()) {
        std::ofstream wf(fileNameFromDate().c_str(), std::ios::out | std::ios::binary | std::ios_base::app);
        if(!wf) {
            std::cout << "Cannot open file!" << std::endl;
        } else {
            // Add header to the buffer
            std::cout << "Create header of file: '" << fileNameFromDate() << "'" << std::endl;
            writeHeaderVersion01(wf);
            wf.close();
        }
    }
}

uint32_t ValueResponse::getNumberOfEntries() const
{
    return FILE_NrDataEntries;
}

uint32_t ValueResponse::getNumberBytesPerSampleIncTimeStamp() const
{
    return m_bytesPerDataSetInclTimeStamp;
}

void ValueResponse::printRawBuffer()
{
    std::cout << "--- ValueResponse::printRawBuffer ------------" << std::endl;
    for(uint32_t cnt = 0; cnt < getNumberOfEntries(); cnt++) {
        uint32_t value = m_responsePtr->getDataField(ValueTableDecode[cnt].cmdId + 1);
        std::cout << ValueTableDecode[cnt].cmdId << ": " << ValueTableDecode[cnt].description.c_str() << ": " << value << " / 0x" << std::hex << value << std::dec << std::endl;
    }
    std::cout << "--- ValueResponse::printRawBuffer ------------" << std::endl;
}

char ValueResponse::serialize()
{
    std::string dateString;
    BitBuffer bitBuffer;

    std::ofstream wf(fileNameFromDate().c_str(), std::ios::out | std::ios::binary | std::ios_base::app);
    if(!wf) {
        std::cout << "Cannot open file!" << std::endl;
        return 1;
    }

    // Copy the unix time to the binary file
    addUnixTimeToBuffer(wf, m_currentUnixTime);

    validateBuffer();

    // Copy the heating data to the bitBuffer
    for(uint32_t cnt = 0; cnt < getNumberOfEntries(); cnt++) {
        uint32_t value = m_responsePtr->getDataField(ValueTableDecode[cnt].cmdId + 1);
        uint32_t nrBits = ValueTableDecode[cnt].nrBits;
        DataTypeInfo dataTypeInfo = ValueTableDecode[cnt].dataTypeInfo;

        switch(dataTypeInfo) {
            case DataTypeInfo::UNSIGNED:
            case DataTypeInfo::BOOL:
                bitBuffer.appendValue(value, nrBits);
                break;
            case DataTypeInfo::SIGNED:
                bitBuffer.appendValue(*(reinterpret_cast<int32_t*>(&value)), nrBits);
                break;
            default:
                assert(0); // Data type not supported
                break;
        }
    }

    // write bitBuffer to file
    uint8_t* pBitBufferData;
    uint32_t nrBytesInBuffer;
    pBitBufferData = bitBuffer.getReferenceToBuffer(nrBytesInBuffer);
    wf.write(reinterpret_cast<const char*>(pBitBufferData), nrBytesInBuffer );
    wf.close();
    return 0;
}

void ValueResponse::addUnixTimeToBuffer(std::ofstream& wf, std::time_t unixTime)
{
    if(8 == sizeof(std::time_t)) {
        wf.write(reinterpret_cast<char*>(&unixTime), sizeof(std::time_t));
    } else if(4 == sizeof(std::time_t)) {
        uint32_t dummy = 0;
        wf.write(reinterpret_cast<char*>(&unixTime), sizeof(std::time_t));
        wf.write(reinterpret_cast<char*>(&dummy), 4);
    } else {
        assert(0);
    }
}

std::string ValueResponse::fileNameFromDate()
{
    struct tm* timeInfo;
    std::stringstream fileNameStream;

    timeInfo = std::localtime(&m_currentUnixTime);

    fileNameStream << "HeatingData/"
                   << std::setw(4) << std::setfill('0') << 1900 + timeInfo->tm_year << "_"
                   << std::setw(2) << std::setfill('0') << 1 + timeInfo->tm_mon << "_"
                   << std::setw(2) << std::setfill('0') << timeInfo->tm_mday << ".dat";
    std::string fileName = fileNameStream.str();
    return fileName;
}

bool ValueResponse::doesFileExist()
{
    std::ifstream infile(fileNameFromDate().c_str());
    return infile.good();
}

void ValueResponse::writeHeaderVersion01(std::ofstream& wf)
{
    wf.write(reinterpret_cast<const char*>(&FILE_Version), 4);
    wf.write(reinterpret_cast<const char*>(&FILE_SizeOfHeader), 4);
    uint32_t bytesPerSample = getNumberBytesPerSampleIncTimeStamp();
    wf.write(reinterpret_cast<const char*>(&bytesPerSample), 4);
}

void ValueResponse::validateBuffer()
{
    for(uint32_t cnt = 0; cnt < getNumberOfEntries(); cnt++) {
        uint32_t value = m_responsePtr->getDataField(ValueTableDecode[cnt].cmdId + 1);

        // It is known that: Ruecklauf-Soll Heizkreis = 50 deg
        if(ValueTableDecode[cnt].cmdId + 1 == 19) {
            if(value != 500) {
                std::cout << "There is an issue in the raw data, value is: " << value << std::endl;
                printRawBuffer();
                m_responsePtr->printBuffer();
                assert(0);
            }
        }
    }
}
