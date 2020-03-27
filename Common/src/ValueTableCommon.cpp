#include <iostream>
#include <assert.h>
#include "Common/src/ValueTableCommon.hpp"



ValueTableCommon::ValueTableCommon()
    : m_isInitialized(false)
{
    std::cout << "ValueTableCommon::" << __FUNCTION__ << std::endl;
}

ValueTableCommon::~ValueTableCommon()
{
    std::cout << "ValueTableCommon::" << __FUNCTION__ << std::endl;
}

uint32_t ValueTableCommon::getCommandId(uint32_t entryNr) const
{
    assert(true == m_isInitialized);
    assert(getNrDataEntriesPerSet() > entryNr);
    return m_entries[entryNr].cmdId + 1;
}

std::string ValueTableCommon::getDescription(uint32_t entryNr) const
{
    assert(true == m_isInitialized);
    assert(getNrDataEntriesPerSet() > entryNr);
    return m_entries[entryNr].description;
}

uint32_t ValueTableCommon::getConversionDivisor(uint32_t entryNr) const
{
    assert(true == m_isInitialized);
    assert(getNrDataEntriesPerSet() > entryNr);
    return m_entries[entryNr].conversionDivisor;
}

std::string ValueTableCommon::getUnit(uint32_t entryNr) const
{
    assert(true == m_isInitialized);
    assert(getNrDataEntriesPerSet() > entryNr);
    return m_entries[entryNr].unit;
}

uint32_t ValueTableCommon::getNrBitsInBuffer(uint32_t entryNr) const
{
    assert(true == m_isInitialized);
    assert(getNrDataEntriesPerSet() > entryNr);
    return m_entries[entryNr].nrBits;
}

DataTypeInfo ValueTableCommon::getDataTypeInfo(uint32_t entryNr) const
{
    assert(true == m_isInitialized);
    assert(getNrDataEntriesPerSet() > entryNr);
    return m_entries[entryNr].dataTypeInfo;
}

uint32_t ValueTableCommon::getNrDataEntriesPerSet() const
{
    assert(true == m_isInitialized);
    return m_entries.size();
}

uint32_t ValueTableCommon::getNrBitsInBufferPerSet() const
{
    assert(true == m_isInitialized);
    return m_nrBitsInBufferPerSet;

}

uint32_t ValueTableCommon::getNrBytesInBufferPerSet() const
{
    assert(true == m_isInitialized);
    return m_bytesPerSetInclTimeStamp;
}

void ValueTableCommon::initValueTable()
{
    std::cout << "ValueTableCommon::" << __FUNCTION__ << std::endl;
    assert(0);
}

void ValueTableCommon::initSizes()
{
    std::cout << "ValueTableCommon::" << __FUNCTION__ << std::endl;

    // Must be set first, otherweise the function calls 'getNrDataEntriesPerSet' and the like
    // further down will assert.
    m_isInitialized = true;

    m_nrBitsInBufferPerSet = 0;

    std::cout << "getNrDataEntriesPerSet: " << getNrDataEntriesPerSet() << std::endl;

    for(uint32_t cnt = 0; cnt < getNrDataEntriesPerSet(); cnt++) {
        m_nrBitsInBufferPerSet += getNrBitsInBuffer(cnt);
    }
    m_bytesPerSetInclTimeStamp = (((m_nrBitsInBufferPerSet - 1) / 8) + 1) + 8; // 8 is the time stamp
    std::cout << "Nr bits per data set: " << m_nrBitsInBufferPerSet
              << ", bytes per data set: " << m_bytesPerSetInclTimeStamp
              << ", number of values per data set: " << getNrDataEntriesPerSet() << std::endl;
}
