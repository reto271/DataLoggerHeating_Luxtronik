#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "Common/src/IValueTable.hpp"

class ValueTableCommon : public IValueTable
{
public:
    /// Common constructor for all value tables (all versions).
    /// \param[in] enableLog if set to true, more information is written to
    ///   the STDOUT.
    ValueTableCommon(bool enableLog);

    virtual ~ValueTableCommon();

    void initialize() override;

    uint32_t getCommandId(uint32_t entryNr) const override;
    std::string getDescription(uint32_t entryNr) const override;
    uint32_t getConversionDivisor(uint32_t entryNr) const override;
    std::string getUnit(uint32_t entryNr) const override;
    uint32_t getNrBitsInBuffer(uint32_t entryNr) const override;
    DataTypeInfo getDataTypeInfo(uint32_t entryNr) const override;

    // DB Functions
    bool getWriteToDataBase(uint32_t entryNr) const override;
    uint32_t getScalingDataBase(uint32_t entryNr) const override;
    std::string getUnitNameDataBase(uint32_t entryNr) const override;

    virtual uint32_t getFileVersion() const = 0;
    uint32_t getSizeOfHeader() const = 0;
    uint32_t getNrDataEntriesPerSet() const override;
    uint32_t getNrBitsInBufferPerSet() const override;
    uint32_t getNrBytesInBufferPerSet() const override;


protected:
    /// Initializes the value table. Namely sets the m_entries to the
    ///  current table.
    virtual void initValueTable() = 0;

    /// Holds the ValueTable. All information about the active entries is written to
    /// this vector
    std::vector<ValueEntry> m_entries;

    /// Number of bits used in BitBuffer or binary file per data set. (Used to properly read
    ///  data in files)
    uint32_t m_nrBitsInBufferPerSet;

    /// Number of bytes used to store a data set including the time stamp.
    uint32_t m_bytesPerSetInclTimeStamp;

    bool m_isInitialized;   ///< True if the object is already initialized
    bool m_enableLog;       ///< True if additional log info shall be written to the STDOUT
};
