#include <ctime>
#include <iostream>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <stdint.h>
//#include <errno.h>
//#include <arpa/inet.h>
//#include <time.h>
//#include <assert.h>
//
//#include "ReceiveDataBuffer.hpp"
#include "ValueResponse.hpp"
#include "SynchronizeTime.hpp"
#include "TcpConnection.hpp"


int main(int argc, char* argv[])
{
    SynchronizeTime sync;

    if (argc != 2) {
        std::cout << std::endl << "Usage: " <<  argv[0] << " <ip of server>" << std::endl;
        return 1;
    }

    TcpConnection tcpConnection(argv[1], 8889);
    tcpConnection.connectToHeating();

    while(true)
    {
        std::time_t currentUnixTime = sync.waitForMinute();

        std::cout << "------------------------------------------------------" << std::endl;
        RecDataStoragePtr receiveDataPtr = tcpConnection.requestValues();

        ValueResponse decodeValueResp(receiveDataPtr, currentUnixTime);
        //decodeValueResp.decode();
        decodeValueResp.serialize();
        sleep(5);
    }

    tcpConnection.disconnectFromHeating();
    return 0;
}
