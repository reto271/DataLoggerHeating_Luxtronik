#pragma once

#include <memory>

/// Command sent to the Luxtronik controller
typedef enum {
    ReadParam,  ///< Read parameters from the controller
    ReadValue   ///< Read values from the controller
} HeatControlCommand;


class RecDataStorage;
using RecDataStoragePtr = std::shared_ptr<class RecDataStorage>;

/// Collects the data from the Luxtronik controller. It assembles a whole frame, if received in
///  multiple parts.
class RecDataStorage
{
public:
    /// Creates a data receiver object.
    RecDataStorage();

    /// Clears the current buffer and initializes the internal read/write data pointers accordingly.
    void clearBuffer();

    /// Adds a number of bytes to the receive buffer. It always appends the received data. The data is copied to the
    ///  buffer, the pBuffer may be deleted afterwards.
    /// \param pBuffer points to the data to be added to the buffer.
    /// \param nrChar is the number of characters in the provided buffer (pBuffer)
    void addData(uint8_t* pBuffer, size_t nrChar);

    /// Reads a 4 byte data values from the buffer.
    /// \param index of the data filed to be read
    /// \return 4 byte value of the requested data field
    uint32_t getDataField(const uint32_t index);

    /// Provides the current command, namely whether parameters or values are currently stored in the buffer.
    /// \return Parameter or Values
    HeatControlCommand getHeatControlCommand();

    /// Prints the current buffer. Used for debugging purposes.
    void printBuffer();

    /// Prints whether the current buffer holds parameters or values.
    void printCommand();


private:
    /// Swaps he bytes to represent values in the architectures data format.
    /// \param[in] value in protocol format representation
    /// \return value in CPU architecture format, interpretable as uint32_t. Conversions to float and other data types must be done afterwards.
    uint32_t swap(uint32_t value);

    /// Size of the internal buffer.
    static const size_t BufferSize = 5000;

    /// Number of parameters in the buffer.
    static const uint32_t NrParameter = 1087;

    /// Write position in the buffer. New data provided by the 'addData' method will be appended at this position.
    size_t m_writePos;

    /// Buffer structure to interpret the data stream received from the Luxtronik controller.
    typedef struct {
        uint32_t commandResponse;         ///< Command sent to the controller
        uint32_t nrEntries;               ///< Number of data values received from the controller
        uint32_t data[NrParameter];       ///< Max number of parameters in the frame.
    } DecodedBuffer;

    /// Union of the data stream and the structure to interpret the data. The data will be places using the buffer into
    ///  the buffer. The decode is used to read the data from the buffer.
    typedef union {
        uint8_t buffer[BufferSize];    ///< Raw buffer, data to be places according the byte order in the received frame.
        DecodedBuffer decode;          ///< Data in the decodeable format.
    } DataBuffer;

    /// Stores the complete response. Namely the command and the data.
    typedef struct {
        HeatControlCommand heatControlCommand;   ///< Command sent to the controller (also read back). Is part of the response.
        DataBuffer data;                         ///< Data received from the controller
    } ResponseBuffer;

    /// Instance of the receive buffer.
    ResponseBuffer m_response;
};
