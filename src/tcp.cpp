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

static int getIndexPage(const char* srcIpAddr);
static int changeToLuxJs(const char* srcIpAddr);
static void receivePacket(const int sockfd);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }
    if (0 != getIndexPage(argv[1])) {
        printf("Abort!!!\n");
    }

    if (0 != changeToLuxJs(argv[1])) {
        printf("Abort!!!\n");
    }
}

static int getIndexPage(const char* srcIpAddr)
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);

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
        char sendData[] =
            "GET /Webserver/index.html HTTP/1.1\r\nHost: 192.168.1.144\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:57.0) Gecko/20100101 Firefox/57.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nReferer: http://192.168.1.144/\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nIf-Modified-Since: Tue, 21 Feb 2017 07:24:24 GMT\r\nCache-Control: max-age=0\r\n\r\n";
        printf("sendData size: %ld\n", strlen(sendData));
        if (send(sockfd, sendData, sizeof(sendData), 0) < 0) {
            puts("Send failed");
            return 1;
        }
    }

    // Receive a reply from the server
    receivePacket(sockfd);

// -     // send ws request
// -     {
// -         uint8_t sendData[] = {0x81, 0x87, 0xc3, 0x8b, 0xfe, 0x1f, 0x8f, 0xc4, 0xb9, 0x56, 0x8d, 0xb0, 0xce, 0x4c, 0x4f, 0x47, 0x49, 0x4e, 0x3b, 0x30};
// -         if( send(sockfd, sendData , sizeof(sendData) , 0) < 0) {
// -             puts("Send failed");
// -             return 1;
// -         }
// -     }

    // wait for ws response
    receivePacket(sockfd);

    //     while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
    //         recvBuff[n] = 0;
    //         if(fputs(recvBuff, stdout) == EOF) {
    //             printf("\n Error : Fputs error\n");
    //         }
    //     }

    if (n < 0) {
        printf("\n Read error \n");
    }

    close(sockfd);
    return 0;
}

static int changeToLuxJs(const char* srcIpAddr)
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);

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
        char sendData[] =
            "GET /Webserver/Lux.js HTTP/1.1\r\nHost: 192.168.1.144\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:57.0) Gecko/20100101 Firefox/57.0\r\nAccept: */*\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nReferer: http://192.168.1.144/Webserver/index.html\r\nConnection: keep-alive\r\nIf-Modified-Since: Fri, 17 Feb 2017 12:50:13 GMT\r\nCache-Control: max-age=0\r\n\r\n";
        printf("sendData size: %ld\n", strlen(sendData));
        if (send(sockfd, sendData, sizeof(sendData), 0) < 0) {
            puts("Send failed");
            return 1;
        }
    }

    // Receive a reply from the server
    {
        struct timespec timeout, dummyTime;
        timeout.tv_sec = 0;
        timeout.tv_nsec = 300000000L;
        for (int cnt = 0; cnt < 50; cnt++) {
            receivePacket(sockfd);
            nanosleep(&timeout, &dummyTime);
            printf("+");
        }
    }
    printf("\n");

    if (n < 0) {
        printf("\n Read error \n");
    }

    close(sockfd);
    return 0;
}

static void receivePacket(const int sockfd)
{
    uint8_t recData[2000];
    uint32_t recDataLen = recv(sockfd, recData, 2000, 0);
    printf("-----------------------------------------------------------------------\n");
    if (recDataLen < 0) {
        printf("Rec failed");
    } else {
        printf("Rec nr bytes: %d\n", recDataLen);

        printf("%s", recData);
        printf("---\n");
    }
}
