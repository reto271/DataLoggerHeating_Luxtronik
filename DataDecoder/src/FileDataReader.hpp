#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include "IFileDataWriterCSV.hpp"
#include "Common/src/CommonTypeDefs.hpp"

class IValueTable;

/// Reads a binary files. These binary files are created by the MonitorHeating application. First
///  the reader finds the appropriate file version of the binary file. In a second step it runs
///  a few checks on the binary file. If these checks are successful the data is read an stored
///  in a CSV file. The CSV file is named as the input file and '.csv' as extension is appended.
class FileDataReader
{
public:
    /// Constructs a file data reader
    /// \param[in] inputFileName is the file name of the binary file to be converted
    /// \param[in] csvWriter is a reference to a object capable of writing CSV files.
    /// \param[in] enableLog set it to true, if additional output on the STDOUT is required.
    FileDataReader(std::string inputFileName, IFileDataWriterCSV_SPtr csvWriter, bool enableLog);

    /// Destructs the file data reader and closes the binary file.
    virtual ~FileDataReader();

    /// Reads the file and does some validation. It finds the file version and runs some checks.
    /// \return true if the file is valid. The file is not valid if the file version is not supported
    ///  its length is not dividable by the size of a data set or the header is not readable.
    bool readHeaderData();

    /// Reads the data from the binary files, converts it to 'IFileDataWriterCSV::DataEntryCSV' and
    ///  hand-over the data to the csvWriter.
    bool decodeData();

protected:
    /// Reads binary files of version v1 & v2. These file versions were not using the BitBuffer,
    ///  therefore consuming more space on disk.
    void readRawDataFromFile_v1_v2();

    /// Reads binary files of version v3 and newer. Since v3 the BitBuffer is used to compact the
    ///  data. Therefore all file versions v3 and newer may be read in the same manner.
    void readRawDataFromFile();

    /// Debug function, print the raw data read from the file.
    void printRawBuffer();

private:
    /// Finds the file version in the header of the binary files and checks if the file version
    ///  is supported. It initializes the value table to the found file version. If the file
    ///  version is not found the value table remains uninitialized. The successive function
    ///  will not find an initialized value table.
    /// \ref m_pValueTable "See member m_pValueTable"
    void determineFileVersion();

    /// Validates the file, by using validateHeaderSize(), validateNrEntriesPerRecord() and
    ///  validateFileLength().
    /// \remark If the file is not valid the value table pointer is set to Null.
    /// \ref m_pValueTable "See member m_pValueTable"
    void validateFile();

    /// Compares the header size filed in the binary file with the header files size stored in the
    ///  value table of the given version.
    /// \remark If the file is not valid the value table pointer is set to Null.
    /// \ref m_pValueTable "See member m_pValueTable"
    void validateHeaderSize();

    /// Compares the number of fields a data set has. This information is stored in the value
    ///  table and in the header of the binary file.
    /// \remark If number of values per set does not match the value table pointer is set to Null.
    /// \ref m_pValueTable "See member m_pValueTable"
    void validateNrEntriesPerRecord();

    /// Reads the file size, subtracts the header size. Then divides the remaining size by
    ///  the size of a single data set. If the reminder of this division is zero, the file
    ///  is potentially valid.
    /// \remark If the reminder of the division is not 0  the value table pointer is set to Null.
    /// \ref m_pValueTable "See member m_pValueTable"
    void validateFileLength();

    /// Prepares the header line for the CSV file. The column headers are separated by comma.
    /// \return The comma separated header row.
    std::string prepareHeader();

    /// Below the headers of the columns a row containing the units of the values is written. This
    ///  function prepares this line. The units are also comma separated.
    /// \return The comma separated unit row.
    std::string prepareUnits();


    std::string m_inputFileName;           ///< File name of the input file, typically a .dat file.
    std::ifstream m_inputFileStream;       ///< Input stream of the binary file.
    uint32_t m_totalFileLength;            ///< Length of the file including header and data.

    // Header data
    uint32_t m_fileVersion;                ///< File version found in the header of the file.
    uint32_t m_sizeFileHeader;             ///< Header size in NR of 32bit fields found in the file.
    uint32_t m_nrDataEntriesPerRecord;     ///< Number of values per data set found in the header of the file.

    /// Number of records in the file ((filesize - headersize) / nrDataEntriesPerRecord)
    uint32_t m_nrRecords;

    /// Data read from the file. This array is used to pass the data to the csvWriter.
    std::vector<IFileDataWriterCSV::DataEntryCSV> m_csvBuffer;

    /// Points to the value table corresponding to the given file version. Null if the file version is not supported or not yet initialized.
    std::shared_ptr<IValueTable> m_pValueTable;

    IFileDataWriterCSV_SPtr m_csvWriter;   ///< Pass the data to the writer if successfully read from the file.
    bool m_enableLog;                      ///< Write more output to the STDOUT if true.
};

PointerDefinition(FileDataReader);
