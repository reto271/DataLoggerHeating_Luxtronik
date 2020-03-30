#pragma once

#include <fstream>
#include <vector>
#include <memory>
#include "Common/src/CommonTypeDefs.hpp"

class IFileDataWriterCSV
{
public:
//    virtual ~IFileDataWriterCSV(){}
    virtual bool writeHeader(std::string headerLine) = 0;
    virtual bool writeData(std::vector<std::uint32_t> dataVector, const uint32_t nrColumnExclTimeStamp) = 0;
};

PointerDefinition(IFileDataWriterCSV);
