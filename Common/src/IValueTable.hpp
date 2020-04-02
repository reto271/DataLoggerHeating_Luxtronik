#pragma once


enum class DataTypeInfo {
    UNSIGNED,
    SIGNED,
    BOOL
};


class IValueTable
{
public:
    virtual void initialize() = 0;

    virtual uint32_t getCommandId(uint32_t entryNr) const = 0;
    virtual std::string getDescription(uint32_t entryNr) const = 0;
    virtual uint32_t getConversionDivisor(uint32_t entryNr) const = 0;
    virtual std::string getUnit(uint32_t entryNr) const = 0;
    virtual uint32_t getNrBitsInBuffer(uint32_t entryNr) const = 0;
    virtual DataTypeInfo getDataTypeInfo(uint32_t entryNr) const = 0;

    virtual uint32_t getFileVersion() const = 0;
    virtual uint32_t getSizeOfHeader() const = 0;
    virtual uint32_t getNrDataEntriesPerSet() const = 0;
    virtual uint32_t getNrBitsInBufferPerSet() const = 0;
    virtual uint32_t getNrBytesInBufferPerSet() const = 0;

protected:
    typedef struct {
        uint32_t cmdId;
        // identifier is not used
        std::string description;
        uint32_t conversionDivisor;
        std::string unit;
        uint32_t nrBits;
        DataTypeInfo dataTypeInfo;
    } ValueEntry;
};
