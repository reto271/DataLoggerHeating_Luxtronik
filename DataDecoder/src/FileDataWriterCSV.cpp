#include "FileDataWriterCSV.hpp"

#include <iomanip>
#include <assert.h>
#include <cstring>
#include <iostream>

FileDataWriterCSV::FileDataWriterCSV(std::string fileName)
    : m_fileName(fileName)
    , m_fileIsEmpty(true)
{
    std::cout << "FileDataWriterCSV::FileDataWriterCSV" << std::endl;
    m_csvFile.open(m_fileName, std::ios::out);
}

FileDataWriterCSV::~FileDataWriterCSV()
{
    if (false == m_fileIsEmpty) {
        std::cout << "File '" << m_fileName << "' successfully written" << std::endl;
    } else {
        std::cout << "ERROR: File '" << m_fileName << "' is empty, due conversion error." << std::endl;
    }
    m_csvFile.close();
}

bool FileDataWriterCSV::writeHeader(std::string headerLine)
{
    if(false == m_csvFile.is_open ()) {
        std::cout << "Could not find file: '" << m_fileName << "'" << std::endl;
        return false;
    }

    // Write the header line
    m_csvFile << headerLine << std::endl;
    return true;
}

bool FileDataWriterCSV::writeData(std::vector<DataEntryCSV> dataVector, const uint32_t nrColumnExclTimeStamp)
{
    std::cout << "FileDataWriterCSV::writeData" << std::endl;

    uint32_t nrRows = 0;
    uint32_t nrColInclTimeStamp = 2 + nrColumnExclTimeStamp;

    if(false == m_csvFile.is_open ()) {
        std::cout << "Could not find file: '" << m_fileName << "'" << std::endl;
        return false;
    }

    if(0 == (dataVector.size() % (nrColInclTimeStamp))) {
        nrRows = dataVector.size() / (nrColInclTimeStamp);
    } else {
        std::cout << "vector size is not dividable by number of columns" << std::endl;
        std::cout << "Write total byes " << dataVector.size() << " in " << nrColInclTimeStamp << " columns" << std::endl;
        return false;
    }

    std::cout << "Write total byes " << dataVector.size() << " in " << nrColInclTimeStamp << " columns, gives " << nrRows << " rows" << std::endl;

    // Write data
    uint32_t arrayPos = 0;
    for(uint32_t record = 0; record < nrRows; record++) {
        std::time_t time = static_cast<std::time_t>(std::get<uint32_t>(dataVector[arrayPos].uiVal));
        arrayPos++;
        time += static_cast<std::time_t>(std::get<uint32_t>(dataVector[arrayPos].uiVal)) << 32;
        arrayPos++;
        m_csvFile << time;
        for(uint32_t cnt = 0; cnt < nrColumnExclTimeStamp; cnt++) {
            double value = 0;
            // Is it an unsigned value?
            auto* uVal = std::get_if<uint32_t>(&(dataVector.at(arrayPos).uiVal));
            if(nullptr != uVal) {
                value = static_cast<double>(*uVal) / static_cast<double>(dataVector.at(arrayPos).divisor);
            } else {
                // else test that it is truely a signed value
                auto* iVal = std::get_if<int32_t>(&(dataVector.at(arrayPos).uiVal));
                if(nullptr != iVal) {
                    value = static_cast<double>(*(reinterpret_cast<int32_t*>(iVal))) / static_cast<double>(dataVector.at(arrayPos).divisor);
                } else {
                    assert(0);
                }
            }
            m_csvFile << ", " << std::right << std::setw(9) << value;
            arrayPos++;
        }
        m_csvFile << std::endl;
    }
    m_fileIsEmpty = false;
    return true;
}
