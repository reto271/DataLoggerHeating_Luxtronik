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

#include "ReceiveDataBuffer.hpp"


// do no longer connect to port 80, connect to 8889

static int getSingleParameter(const char* srcIpAddr);
static void receivePacket(const int sockfd, RecDataStorage* pDataBuffer);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }
    if (0 != getSingleParameter(argv[1])) {
        printf("Abort!!!\n");
    }
}

static int getSingleParameter(const char* srcIpAddr)
{
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

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

    {
        //Cmd 3003 : 0x00 0x00 0x0b 0xbb  0x00 0x00 0x00 0x00
        unsigned char sendData[8] = { 0x00, 0x00, 0x0b, 0xbb,  0x00, 0x00, 0x00, 0x00};

        printf("sendData size: %lu\n", sizeof(sendData)/sizeof(char));
        if (send(sockfd, sendData, sizeof(sendData), 0) < 0) {
            puts("Send failed");
            return 1;
        }
    }

    RecDataStorage m_buffer;
    // Receive a reply from the server
    receivePacket(sockfd, &m_buffer);
    receivePacket(sockfd, &m_buffer);
    receivePacket(sockfd, &m_buffer);
    receivePacket(sockfd, &m_buffer);
    receivePacket(sockfd, &m_buffer);

    close(sockfd);
    return 0;
}








/// ------------------ outdated





static void receivePacket(const int sockfd,  RecDataStorage* pDataBuffer)
{
    const size_t REC_BUFFER_SIZE = 10000;
    uint8_t recData[REC_BUFFER_SIZE];

    uint32_t recDataLen = recv(sockfd, recData, REC_BUFFER_SIZE, 0);

    printf("-----------------------------------------------------------------------\n");
    if (recDataLen < 0) {
        printf("Rec failed");
    } else {
        printf("Rec nr bytes: %d\n", recDataLen);

        if (recDataLen > 0) {
            pDataBuffer->addData(recData, recDataLen);
            for (size_t cnt = 0; cnt < recDataLen; cnt++) {
                printf("0x%.2x ", recData[cnt]);
                if (0 == ((cnt + 1) % 16)) {
                    printf("\n");
                }
            }
            printf("\n");
        } else {
            printf("empty\n");
        }
    }
}
