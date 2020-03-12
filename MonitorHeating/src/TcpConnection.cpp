#include "TcpConnection.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>

#include "ReceiveDataBuffer.hpp"

TcpConnection::TcpConnection(std::string ipAddress, uint16_t tcpPort)
    : m_ipAddress(ipAddress)
    , m_tcpPort(tcpPort)
    , m_fileDeviceId(0)
{
}

bool TcpConnection::connectToHeating()
{
    struct sockaddr_in serv_addr;

    std::cout << "connectToHeating ip addr: " << m_ipAddress << ", port: " << m_tcpPort << std::endl;

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_tcpPort);

    if ((m_fileDeviceId = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Error : Could not create socket" << std::endl;
        return false;
    }

    if (inet_pton(AF_INET, m_ipAddress.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cout << "inet_pton error occured" << std::endl;
        return false;
    }

    if (connect(m_fileDeviceId, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Error : Connect Failed" << std::endl;
        return false;
    }
    return true;
}

bool TcpConnection::disconnectFromHeating()
{
    close(m_fileDeviceId);
    return true;
}

bool TcpConnection::requestParameter()
{
    //Cmd 3003 : 0x00 0x00 0x0b 0xbb  0x00 0x00 0x00 0x00 : Read Parameter
    //static uint8_t cmdPara[8] = { 0x00, 0x00, 0x0b, 0xbb, 0x00, 0x00, 0x00, 0x00};
    //memcpy(sendData, cmdPara, 8);
    //nrExpectedResp = 5;
    return false;
}

RecDataStoragePtr TcpConnection::requestValues()
{
    //Cmd 3004 : 0x00 0x00 0x0b 0xbc  0x00 0x00 0x00 0x00 : Read Values
    static uint8_t cmdVal[8] = { 0x00, 0x00, 0x0b, 0xbc, 0x00, 0x00, 0x00, 0x00};
    RecDataStoragePtr receiveBufferPtr = std::make_shared<RecDataStorage>();

    if (true == sendRequest(cmdVal, 8)) {
        if (true == waitResponse(receiveBufferPtr, 1008)) {
            return receiveBufferPtr;
        }
    }
    return nullptr;
}

bool TcpConnection::sendRequest(uint8_t* pRequest, uint32_t length)
{
    if (send(m_fileDeviceId, pRequest, length, 0) < 0) {
        std::cout << "Error: Send failed" << std::endl;
        return false;
    }
    return true;
}

bool TcpConnection::waitResponse(RecDataStoragePtr pReceiveDataBuffer, const uint32_t expectedNrBytes)
{
    const size_t REC_BUFFER_SIZE = 10000;
    uint8_t recData[REC_BUFFER_SIZE];
    uint32_t recDataLen;
    uint32_t totalRecDataLen = 0;

    while(totalRecDataLen < expectedNrBytes) {
        memset(recData, 0x00, REC_BUFFER_SIZE);
        recDataLen = recv(m_fileDeviceId, recData, REC_BUFFER_SIZE, 0);
        if (recDataLen < 0) {
            std::cout << "Error: No data received" << std::endl;
            return false;
        } else {
            pReceiveDataBuffer->addData(recData, recDataLen);
            totalRecDataLen += recDataLen;
            std::cout << "Rec nr bytes: " << recDataLen << ", total received: " << totalRecDataLen << std::endl;
        }
    }
    return true;
}
