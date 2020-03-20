#include <ctime>
#include <iostream>
#include <unistd.h>

#include "ValueResponse.hpp"
#include "SynchronizeTime.hpp"
#include "TcpConnection.hpp"

/// Reset the connection at 3 in the morning
/// \param pConnection, reference to the TCP connection
/// \param pTime, reference to the current time
/// \return false if reconnection was not possible, true otherwise
bool resetConnectionAt0300(TcpConnection* pConnection, SynchronizeTime* pTime);


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
        RecDataStoragePtr receiveDataPtr = tcpConnection.requestValues();
        if (nullptr == receiveDataPtr) {
            std::cout << "Reconnecting ..." << std::endl;
            // Reconnect, it seems there is connection error
            tcpConnection.disconnectFromHeating();
            if (false == tcpConnection.connectToHeating()) {
                // Reconnect failed
                std::cout << "Reconnecting FAILED!!!" << std::endl;
                return 3;
            }
            // Try a second time...
            receiveDataPtr = tcpConnection.requestValues();
        }

        if (nullptr != receiveDataPtr) {
            ValueResponse decodeValueResp(receiveDataPtr, currentUnixTime);
            decodeValueResp.decode();
            decodeValueResp.serialize();
        } else {
            std::cout << "Second consecutive failure" << std::endl;
            return 5;
        }
        if (false == resetConnectionAt0300(&tcpConnection, &sync)) {
            return 4;
        }
        sleep(45);
    }

    tcpConnection.disconnectFromHeating();
    return 0;
}


bool resetConnectionAt0300(TcpConnection* pConnection, SynchronizeTime* pTime)
{
    if (true == pTime->isFullHour(false)) {
        if (3 == pTime->getHour(false)) {
            // Periodically reconnect
            pConnection->disconnectFromHeating();
            if (false == pConnection->connectToHeating()) {
                // Reconnect failed
                std::cout << "Periodic reconnecting FAILED!!!" << std::endl;
                return false;
            }
        }
    }
    return true;
}
