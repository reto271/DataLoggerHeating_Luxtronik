#include "ValueResponse.hpp"

#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <assert.h>

#include "influxdb.hpp"
#include "Common/src/BitBuffer.hpp"

// float conversionFunctionDivisor(const uint32_t value, const uint32_t divisor)
// {
//    return (static_cast<float>(value) / static_cast<float>(divisor));
// }
//
// bool conversionFunctionBool(const uint32_t value)
// {
//    if(0 == value) {
//        return false;
//    } else {
//        return true;
//    }
// }

// ----------------------------------------------------------------------------------

ValueResponse::ValueResponse(RecDataStoragePtr receiveDataPtr, std::time_t currentUnixTime, bool enableLog)
    : m_responsePtr(receiveDataPtr)
    , m_currentUnixTime(currentUnixTime)
    , m_valueTable(enableLog)
{
    std::cout << "Current time: 0x" << std::hex << m_currentUnixTime << std::dec
              << " --- " << std::asctime(std::localtime(&currentUnixTime));

    m_valueTable.initialize();

    if(false == doesFileExist()) {
        std::string fileName(fileNameFromDate());
        std::ofstream wf(fileName.c_str(), std::ios::out | std::ios::binary | std::ios_base::app);
        if(!wf) {
            std::cout << "Cannot open file '" << fileName << "'!" << std::endl;
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
    return m_valueTable.getNrDataEntriesPerSet();
}

uint32_t ValueResponse::getNumberBytesPerSampleIncTimeStamp() const
{
    return m_valueTable.getNrBytesInBufferPerSet();
}

void ValueResponse::printRawBuffer()
{
    std::cout << "--- ValueResponse::printRawBuffer ------------" << std::endl;
    for(uint32_t cnt = 0; cnt < getNumberOfEntries(); cnt++) {
        uint32_t value = m_responsePtr->getDataField(m_valueTable.getCommandId(cnt));
        std::cout << m_valueTable.getCommandId(cnt) << ": " << m_valueTable.getDescription(cnt).c_str() << ": " << value << " / 0x"
                  << std::hex << value << std::dec << std::endl;
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
        uint32_t value = m_responsePtr->getDataField(m_valueTable.getCommandId(cnt));
        uint32_t nrBits = m_valueTable.getNrBitsInBuffer(cnt);
        DataTypeInfo dataTypeInfo = m_valueTable.getDataTypeInfo(cnt);

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
    wf.write(reinterpret_cast<const char*>(pBitBufferData), nrBytesInBuffer);
    wf.close();
    return 0;
}

void ValueResponse::writeToDB()
{
    std::string dateString;
    BitBuffer bitBuffer;
    influxdb_cpp::server_info si("192.168.1.100", 8086, "heatingdb", "", "");

    validateBuffer();

    // Copy the heating data to the bitBuffer
    for(uint32_t cnt = 0; cnt < getNumberOfEntries(); cnt++) {
        if(true == m_valueTable.getWriteToDataBase(cnt)) {

            // Get common data
            uint32_t rawValue = m_responsePtr->getDataField(m_valueTable.getCommandId(cnt));
            std::string description = m_valueTable.getDescription(cnt);
            std::string rawUnit = m_valueTable.getUnit(cnt);

            // Time of the data base entry
            // Force the computation to be executed in 64-bit, otherwise it does not work on raspi
            uint64_t time_ns = static_cast<uint64_t>(1000000000) * m_currentUnixTime;
            // std::cout << "m_currentUnixTime: " << m_currentUnixTime << std::endl;
            // std::cout << "time_ns:           " << time_ns << std::hex << ", 0x" << time_ns << std::dec << std::endl;
            // std::cout << "sizeof(uint64_t):  " << sizeof(uint64_t) << std::endl;

            // Is there an extra unit for the DB entry?
            std::string finalUnit;
            std::string dbUnit = m_valueTable.getUnitNameDataBase(cnt);
            if("" == dbUnit) {
                finalUnit = rawUnit;
            } else {
                finalUnit = m_valueTable.getUnitNameDataBase(cnt);
            }

            // Threat enum and bools different, do not use double interface to the DB
            if(("enum" == rawUnit) || ("bool" == rawUnit)) {
                // Special fields:
                uint16_t intFinalValue = rawValue;
                if(("EVU Sperre" == description) || ("EVU Sperre2" == description)) {
                    intFinalValue = -intFinalValue + 1;
                }

                influxdb_cpp::builder().meas("heating_data").tag("unit", finalUnit).field(description, intFinalValue).timestamp(time_ns).post_http(si);
                std::cout << "Write int: " << description << " : " << rawValue << ", " << intFinalValue << " " << finalUnit << std::endl;
            } else {
                double divisor = m_valueTable.getConversionDivisor(cnt);
                double scaledValue = static_cast<double>(rawValue) / divisor;
                double dblFinalValue;

                // Is there an extra scaling for the DB entry
                uint32_t dbScaling = m_valueTable.getScalingDataBase(cnt);
                if(1 == dbScaling) {
                    dblFinalValue = scaledValue;
                } else {
                    dblFinalValue = scaledValue / dbScaling;
                }

                // Special fields:
                if("Wärmepumpe läuft seit" == description) {
                    if(1 == rawValue) {
                        dblFinalValue = 0; // There is an error, at standstill the run time is 1s. Fix it here.
                    }
                }

                influxdb_cpp::builder().meas("heating_data").tag("unit", finalUnit).field(description, dblFinalValue, 5).timestamp(time_ns).post_http(si);
                std::cout << "Write dbl: " << description << " : " << rawValue << ", " << dblFinalValue << " " << finalUnit << std::endl;
            }
            // } else {
            //     std::cout << "No not write to db: " << m_valueTable.getDescription(cnt) << std::endl;
        }
    }
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
    uint32_t fileVersion = m_valueTable.getFileVersion();
    wf.write(reinterpret_cast<const char*>(&fileVersion), 4);

    uint32_t headerVersion = m_valueTable.getSizeOfHeader();
    wf.write(reinterpret_cast<const char*>(&headerVersion), 4);

    uint32_t bytesPerSample = getNumberBytesPerSampleIncTimeStamp();
    wf.write(reinterpret_cast<const char*>(&bytesPerSample), 4);
}

void ValueResponse::validateBuffer()
{
    for(uint32_t cnt = 0; cnt < getNumberOfEntries(); cnt++) {
        uint32_t value = m_responsePtr->getDataField(m_valueTable.getCommandId(cnt));

        // It is known that: Ruecklauf-Soll Heizkreis = 50 deg
        if(m_valueTable.getCommandId(cnt) == 19) {
            if(value != 500) {
                std::cout << "There is an issue in the raw data, value is: " << value << std::endl;
                printRawBuffer();
                m_responsePtr->printBuffer();
                assert(0);
            }
        }
    }
}
