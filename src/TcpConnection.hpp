#pragma once

#include <memory>

// Forward declaration
using RecDataStoragePtr = std::shared_ptr<class RecDataStorage>;


class TcpConnection
{
public:
    TcpConnection(std::string ipAddress, uint16_t tcpPort);

    bool connectToHeating();
    bool disconnectFromHeating();

    bool requestParameter();
    RecDataStoragePtr requestValues();

private:
    bool sendRequest(uint8_t* pRequest, uint32_t length);
    bool waitResponse(RecDataStoragePtr pReceiveDataBuffer, const uint32_t expectedNrResponseFrames);

    std::string m_ipAddress;
    uint16_t m_tcpPort;
    int m_fileDeviceId;
};
