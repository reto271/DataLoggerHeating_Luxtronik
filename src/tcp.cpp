#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include <assert.h>

#include "ReceiveDataBuffer.hpp"


// do no longer connect to port 80, connect to 8889
typedef enum
{
    ReadParam,
    ReadValue
} Command;


//Cmd 3003 : 0x00 0x00 0x0b 0xbb  0x00 0x00 0x00 0x00 : Read Parameter
static uint8_t cmdPara[8] = { 0x00, 0x00, 0x0b, 0xbb, 0x00, 0x00, 0x00, 0x00};

//Cmd 3004 : 0x00 0x00 0x0b 0xbc  0x00 0x00 0x00 0x00 : Read Values
static uint8_t cmdVal[8] = { 0x00, 0x00, 0x0b, 0xbc, 0x00, 0x00, 0x00, 0x00};


static int getSingleParameter(const char* srcIpAddr, const Command cmd);
static void receivePacket(const int sockfd, RecDataStorage* pDataBuffer);

int main(int argc, char* argv[])
{
    Command cmd;
    cmd = ReadParam;
    cmd = ReadValue;

    if (argc != 2) {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }
    if (0 != getSingleParameter(argv[1], cmd)) {
        printf("Abort!!!\n");
    }
}

static int getSingleParameter(const char* srcIpAddr, const Command cmd)
{
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;
    RecDataStorage m_buffer;
    uint8_t nrExpectedResp = 0;
    unsigned char sendData[8];

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8889);

    if (inet_pton(AF_INET, srcIpAddr, &serv_addr.sin_addr) <= 0) {
        printf("\n inet_pton error occured\n");
        return 1;
    } else {
        printf("inet_pton worked\n");
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Error : Connect Failed \n");
        return 1;
    } else {
        printf("connect worked\n");
    }

    switch (cmd) {
    case ReadParam:
        memcpy(sendData, cmdPara, 8);
        nrExpectedResp = 5;
        break;

    case ReadValue:
        memcpy(sendData, cmdVal, 8);
        nrExpectedResp = 2;
        break;

    default:
        assert(0);
        break;
    }

    printf("sendData size: %lu\n", sizeof(sendData) / sizeof(char));
    if (send(sockfd, sendData, sizeof(sendData), 0) < 0) {
        puts("Send failed");
        return 1;
    }

    // Receive a reply from the server
    for (uint8_t cntReply = 0; cntReply < nrExpectedResp; cntReply++) {
        receivePacket(sockfd, &m_buffer);
    }

    m_buffer.printBuffer();

    close(sockfd);
    return 0;
}

static void receivePacket(const int sockfd, RecDataStorage* pDataBuffer)
{
    const size_t REC_BUFFER_SIZE = 10000;
    uint8_t recData[REC_BUFFER_SIZE];

    uint32_t recDataLen = recv(sockfd, recData, REC_BUFFER_SIZE, 0);

    printf("--- ");
    if (recDataLen < 0) {
        printf("Rec failed");
    } else {
        printf("Rec nr bytes: %d\n", recDataLen);

        if (recDataLen > 0) {
            pDataBuffer->addData(recData, recDataLen);
        } else {
            printf("empty\n");
        }
    }
}
