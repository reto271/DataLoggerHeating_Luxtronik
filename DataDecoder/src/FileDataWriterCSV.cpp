#include "FileDataWriterCSV.hpp"

#include <iomanip>
#include <assert.h>
#include <cstring>
#include <iostream>

FileDataWriterCSV::FileDataWriterCSV(std::string fileName)
    : m_fileName(fileName)
{
    std::cout << "FileDataWriterCSV::FileDataWriterCSV" << std::endl;
    m_csvFile.open(m_fileName, std::ios::out);
}

FileDataWriterCSV::~FileDataWriterCSV()
{
    std::cout << "FileDataWriterCSV::~FileDataWriterCSV" << std::endl;
    std::cout << "File '" << m_fileName << "' successfully written" << std::endl;
    m_csvFile.close();
}

bool FileDataWriterCSV::writeHeader(std::string headerLine)
{
    std::cout << "FileDataWriterCSV::writeHeader" << std::endl;
    if(false == m_csvFile.is_open ()) {
        std::cout << "Could not find file: '" << m_fileName << "'" << std::endl;
        return false;
    }

    // Write the header line
    m_csvFile << headerLine << std::endl;
}

bool FileDataWriterCSV::writeData(std::vector<std::uint32_t> dataVector, const uint32_t nrColumnExclTimeStamp)
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
        std::time_t time = static_cast<std::time_t>(dataVector[arrayPos]);
        arrayPos++;
        time += static_cast<std::time_t>(dataVector[arrayPos]) << 32;
        arrayPos++;
        m_csvFile << time;
        for(uint32_t cnt = 0; cnt < nrColumnExclTimeStamp; cnt++) {
            m_csvFile << ", " << std::right << std::setw(9) << *(reinterpret_cast<int32_t*>(&dataVector[arrayPos]));
            arrayPos++;
        }
        m_csvFile << std::endl;
    }

    return true;
}
