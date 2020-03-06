#include <ctime>
#include <iostream>
#include <unistd.h>

#include "ValueResponse.hpp"
#include "SynchronizeTime.hpp"
#include "TcpConnection.hpp"

/// Main function to setup the connection, restart it periodically, read the data and forwards them to the decoder / serializer.
int main(int argc, char* argv[])
{
    SynchronizeTime sync;

    if (argc != 2) {
        std::cout << std::endl << "Usage: " <<  argv[0] << " <ip of server>" << std::endl;
        return 1;
    }

    TcpConnection tcpConnection(argv[1], 8889);
    if (false == tcpConnection.connectToHeating()) {
        return 2;
    }

    while(true)
    {
        std::time_t currentUnixTime = sync.waitForMinute();

        std::cout << "------------------------------------------------------" << std::endl;
        if (nullptr == RecDataStoragePtr receiveDataPtr = tcpConnection.requestValues()) {
            std::cout << "Reconnecting ..." << std::endl;
            // Reconnect, it seems there is connection error
            tcpConnection.disconnectFromHeating();
            if (false == tcpConnection.connectToHeating()) {
                // Reconnect failed
                std::cout << "Reconnecting FAILED!!!" << std::endl;
                return 3;
            }
        }

        ValueResponse decodeValueResp(receiveDataPtr, currentUnixTime);
        //decodeValueResp.decode();
        decodeValueResp.serialize();
        sleep(5);
    }

    tcpConnection.disconnectFromHeating();
    return 0;
}
