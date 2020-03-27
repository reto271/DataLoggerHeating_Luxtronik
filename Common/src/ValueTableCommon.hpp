#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "Common/src/IValueTable.hpp"

class ValueTableCommon : public IValueTable
{
public:

    ValueTableCommon();

    virtual ~ValueTableCommon();

    void initialize() override;

    uint32_t getCommandId(uint32_t entryNr) const override;
    std::string getDescription(uint32_t entryNr) const override;
    uint32_t getConversionDivisor(uint32_t entryNr) const override;
    std::string getUnit(uint32_t entryNr) const override;
    uint32_t getNrBitsInBuffer(uint32_t entryNr) const override;
    DataTypeInfo getDataTypeInfo(uint32_t entryNr) const override;

    virtual uint32_t getFileVersion() const = 0;
    uint32_t getSizeOfHeader() const = 0;
    uint32_t getNrDataEntriesPerSet() const override;
    uint32_t getNrBitsInBufferPerSet() const override;
    uint32_t getNrBytesInBufferPerSet() const override;


protected:
    virtual void initValueTable() = 0;

    std::vector<ValueEntry> m_entries;

    uint32_t m_nrBitsInBufferPerSet;

    /// Number of bytes used to store a data set including the time stamp.
    uint32_t m_bytesPerSetInclTimeStamp;

    bool m_isInitialized;
};
