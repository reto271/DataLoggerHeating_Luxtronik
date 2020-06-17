#pragma once

#include <fstream>
#include <vector>
#include <memory>
#include <stdint.h>
#include <variant>

#include "Common/src/CommonTypeDefs.hpp"

/// Interface class for the FileDataWriterCSV. The class writes CSV files with the provided data.
class IFileDataWriterCSV
{
public:
    /// The user must provide the data to be written in a CSV file using a vector of this
    ///  struct.
    typedef struct {
        std::variant<uint32_t, int32_t> uiVal;  ///< Raw value
        uint32_t divisor;                       ///< Divisor. CSV value = (raw value) / divisor
    } DataEntryCSV;

    /// Header line to be written to the CSV.
    /// \remark The function might be called multiple times, if there shall be more than one row heading
    ///   data, e.g. the first now is the data name and the second is the units.
    /// \param[in] headerLine contains the column headings, the headings must be separated by comma.
    /// \return true if the header line could be written to the file.
    virtual bool writeHeader(std::string headerLine) = 0;

    /// Writes the data to the CSV file. It starts a new row after (1 + nrColumnExclTimeStamp).
    /// \param[in] dataVector is a Nx1 data vector of all data.
    /// \param[in] nrColumnExclTimeStamp determines after how may values from dataVector a line
    ///    break is introduced. A line break is added after (1 + nrColumnExclTimeStamp).
    /// \return true if the header line could be written to the file.
    virtual bool writeData(std::vector<DataEntryCSV> dataVector, const uint32_t nrColumnExclTimeStamp) = 0;
};

PointerDefinition(IFileDataWriterCSV);
