#pragma once

#include <fstream>
#include <vector>
#include <memory>
#include <stdint.h>
#include <variant>

#include "Common/src/CommonTypeDefs.hpp"

class IFileDataWriterCSV
{
public:
    typedef struct {
        std::variant<uint32_t, int32_t> uiVal;
        uint32_t divisor;
    } DataEntryCSV;

//    virtual ~IFileDataWriterCSV(){}
    virtual bool writeHeader(std::string headerLine) = 0;
    virtual bool writeData(std::vector<DataEntryCSV> dataVector, const uint32_t nrColumnExclTimeStamp) = 0;
};

PointerDefinition(IFileDataWriterCSV);
