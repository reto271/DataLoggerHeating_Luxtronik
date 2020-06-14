#pragma once

#include <memory>

// Forward declaration
using RecDataStoragePtr = std::shared_ptr<class RecDataStorage>;

/// Handles the TCP connection to the Luxtronik heating controller. It establishes the connection, requests values or parameters,
///  collects the response and closes the connection.
class TcpConnection
{
public:
    /// Creates an object to connect to a Luxtronik heating controller.
    /// \param[in] ipAddress of the heating controller.
    /// \param[in] tcpPort of the heating controller to be connected to. Make sure to use the TCP port of the data interface.
    TcpConnection(std::string ipAddress, uint16_t tcpPort);

    /// Connects to the heating controller, namely establishes a TCP channel.
    /// \return true if a connection to the heating controller could be established, false otherwise.
    bool connectToHeating();

    /// Closes the TCP channel to the heating controller.
    /// \return true if the connection is successfully closed, false otherwise.
    bool disconnectFromHeating();

    /// Requests the parameters from the heating controller.
    /// \remark: Not yet implemented
    /// \return true if the request is successfully sent, false otherwise.
    bool requestParameter();

    /// Requests the values from the heating controller. It expects 1008 bytes to be received from the heating
    ///  controller.
    /// \return Pointer to the receive data storage. The data may be read by index from this buffer.
    RecDataStoragePtr requestValues();

private:
    /// Sends a request to the controller.
    /// \param[in] pRequest points to a request. The request is an data array.
    /// \param[in] length of the request to be sent.
    /// \return true if the request could be sent, false otherwise.
    bool sendRequest(uint8_t* pRequest, uint32_t length);

    /// Waits until the whole response is received from the heating controller.
    /// \param[out] pReceiveDataBuffer is the buffer where the response will be stored to.
    /// \param[in] expectedNrBytes is the number of bytes of a correct response.
    /// \return true if a complete response is received.
    bool waitResponse(RecDataStoragePtr pReceiveDataBuffer, const uint32_t expectedNrBytes);

    /// IP address of the heating controller
    std::string m_ipAddress;

    /// TCP port of the binary data service interface of the Luxtronik heating controller.
    uint16_t m_tcpPort;

    /// Identifier of the TCP socket device.
    int m_socketDeviceId;
};
