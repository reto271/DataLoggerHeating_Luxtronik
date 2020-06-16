#pragma once

#include "Common/src/ValueTableCommon.hpp"

/// Value table version 3. This version is no longer used to actively write files. It
///  is used to convert old binary files (v1) to CSV files, Namely used by the
///  DataDecoder  program.
class ValueTable_v3 : public ValueTableCommon
{
public:
    /// Constructs the value table version v3.
    /// \param[in] enableLog if set to true, more information is written to
    ///   the STDOUT.
    ValueTable_v3(bool enableLog);

    virtual ~ValueTable_v3();

    uint32_t getFileVersion() const override;
    uint32_t getSizeOfHeader() const override;

private:
    void initValueTable() override;

    const uint32_t FILE_Version = 3;          ///< Version number of the value table
    const uint32_t FILE_SizeOfHeader = 3;     ///< The header size of value table v1
};
