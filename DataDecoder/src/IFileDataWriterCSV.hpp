#pragma once

#include <fstream>
#include <vector>
#include <memory>
#include "Common/src/CommonTypeDefs.hpp"

class IFileDataWriterCSV
{
public:
    typedef struct {
        uint32_t value;
        uint32_t divisor;
    } DataEntryCSV;

//    virtual ~IFileDataWriterCSV(){}
    virtual bool writeHeader(std::string headerLine) = 0;
    virtual bool writeData(std::vector<DataEntryCSV> dataVector, const uint32_t nrColumnExclTimeStamp) = 0;
};

PointerDefinition(IFileDataWriterCSV);
