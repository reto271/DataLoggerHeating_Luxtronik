#pragma once

/// The ValueTable handles information of the data entries in the heating controller. It
///  stores scaling, data types, description, number of bits used in the compact
///  binary representation, unit.

/// Information regarding the data type.
enum class DataTypeInfo {
    UNSIGNED,    ///< Data type stores unsigned data
    SIGNED,      ///< Data type stores signed data
    BOOL         ///< Data type is boolean
};


/// Interface of the ValueTable. It provides access to the different fields in the value table.
class IValueTable
{
public:
    /// Initializes the value table to a given version. Each version must overwrite this functions
    ///  using its own table.
    virtual void initialize() = 0;

    /// Returns the command ID of the given data entry. Actually it would be more like a parameter
    ///  or value ID. In the Luxtronik docu they name it command ID, therefore this terminology
    ///  is used here too.
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return Command ID of the value.
    virtual uint32_t getCommandId(uint32_t entryNr) const = 0;

    /// Returns the description of the value.
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return Description of the value as string.
    virtual std::string getDescription(uint32_t entryNr) const = 0;

    /// The divisor of the raw value, e.g. 10. The a temperature of 198 will be divided by 10 = 19.8.
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return Divisor of the raw value.
    virtual uint32_t getConversionDivisor(uint32_t entryNr) const = 0;

    /// Unit of the value.
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return Unit of the value.
    virtual std::string getUnit(uint32_t entryNr) const = 0;

    /// Number of bits used in the BitBuffer and binary file to represent this value
    /// \remark The number of bits may changes in different file versions.
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return Number of bits to be allocated in binary representation.
    virtual uint32_t getNrBitsInBuffer(uint32_t entryNr) const = 0;

    /// Data type information (Unsigned, signed or boolean)
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return Type information
    virtual DataTypeInfo getDataTypeInfo(uint32_t entryNr) const = 0;


    // --------------------------------------------------------------------
    // Data base functions

    /// Defines whether this value is written to the data base or not.
    /// \remark DB Functions (InfluxDB)
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return true if the value shall be written to the data base
    virtual bool getWriteToDataBase(uint32_t entryNr) const = 0;

    /// Scaling for the data bases, e.g. if a raw value in seconds is not human readable it might
    ///  be written in minutes. This function would then return 60.
    /// \remark DB Functions (InfluxDB)
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return Divisor to write the value in a different scale to the DB
    virtual uint32_t getScalingDataBase(uint32_t entryNr) const = 0;

    /// If the scaling for the data bases is not 1, then the unit changes. This function returns the
    ///  unit matching the scaling for the DB, e.g. min
    /// \remark DB Functions (InfluxDB)
    /// \param[in] entryNr is the index in the value table. The range is 0 .. getNrDataEntriesPerSet()
    ///  The value getNrDataEntriesPerSet() depends on the table version.
    /// \return Unit of the representation in the DB
    virtual std::string getUnitNameDataBase(uint32_t entryNr) const = 0;


    // --------------------------------------------------------------------
    // Version and size information

    /// Returns the version of the value table.
    /// \remark Version info, general info about entries.
    /// \return returns the version of the value table
    virtual uint32_t getFileVersion() const = 0;

    /// Returns the size of the header uint32_t sizes. (Must be multiplied by 4 to get the size in bytes).
    /// \remark Version info, general info about entries.
    /// \return Returns the number of fields in the header, assuming a filed is 4 bytes.
    virtual uint32_t getSizeOfHeader() const = 0;

    /// Returns the number of data entries in a set. Each entry is 32bit.
    /// \remark Version info, general info about entries.
    /// \return Returns the number of fields in the header, assuming a filed is 4 bytes.
    virtual uint32_t getNrDataEntriesPerSet() const = 0;

    /// Returns the number of bits in the binary buffer per set. Each entry is 32bit.
    /// \remark Version info, general info about entries.
    /// \return Number of bits in the BitBuffer/Binary-file per data set.
    virtual uint32_t getNrBitsInBufferPerSet() const = 0;

    /// Returns the number of bytes in a set. No all bits might be used, the last byte might might be
    ///  partially used only.
    /// \remark Version info, general info about entries.
    /// \return Returns the bytes in the BitBuffer and binary files.
    virtual uint32_t getNrBytesInBufferPerSet() const = 0;

protected:

    /// Table entry. If this entry changes, make sure to define a new file version.
    typedef struct {
        uint32_t cmdId;                  ///< Command (value) Id of a given value
        std::string description;         ///< Description (name) of the value
        uint32_t conversionDivisor;      ///< Divisor if the raw data is fix-point scaled
        std::string unit;                ///< Unit of the value
        bool writeToDataBase;            ///< True if it shall be written to the DB (InfluxDB)
        uint32_t scalingDataBase;        ///< Scaling in the DB if different to the value
        std::string unitNameDataBase;    ///< Unit used in the DB
        uint32_t nrBits;                 ///< Number of bits used in the BitBuffer and the binary file
        DataTypeInfo dataTypeInfo;       ///< Data type information (unsigned, signed or boolean)
    } ValueEntry;
};
