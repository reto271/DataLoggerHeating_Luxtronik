#include <ctime>
#include <iostream>
#include <unistd.h>

#include "ValueResponse.hpp"
#include "SynchronizeTime.hpp"
#include "TcpConnection.hpp"
#include "Common/src/IP_AddressValidator.hpp"

/// Reset the connection at 3 in the morning
/// \param pConnection, reference to the TCP connection
/// \param pTime, reference to the current time
/// \return false if reconnection was not possible, true otherwise
bool resetConnectionAt0300(TcpConnection* pConnection, SynchronizeTime* pTime);

/// Application state, to enable some debug functions
enum class ApplicationState {
    NORMAL,
    DEBUG
};


/// Main function to setup the connection, restart it periodically, read the data and forwards them to the decoder / serializer.
int main(int argc, char* argv[])
{
    SynchronizeTime sync;
    std::time_t currentUnixTime;
    ApplicationState applState = ApplicationState::NORMAL;
    std::string ipAddr;

    std::cout << std::endl;

    switch(argc) {
        case 2:
            ipAddr = argv[1];
            break;

        case 3:
            {
                ipAddr = argv[1];
                std::string dbgSwitch = argv[2];
                if("--debug" == dbgSwitch) {
                    std::cout << "ApplicationState::DEBUG" << std::endl;
                    applState = ApplicationState::DEBUG;
                }
            }
            break;

        default:
            std::cout << std::endl << "Usage: " << argv[0] << " <ip of server>" << std::endl;
            return 1;
    }

    IP_AddressValidator validateIp(ipAddr);
    if(false == validateIp.validate()) {
        std::cout << "'" << ipAddr << "' is not a valid IP address" << std::endl;
        return 2;
    }

    TcpConnection tcpConnection(argv[1], 8889);
    if(false == tcpConnection.connectToHeating()) {
        return 3;
    }

    while(true) {
        if(ApplicationState::NORMAL == applState) {
            currentUnixTime = sync.waitForMinute();
        } else {
            currentUnixTime = std::time(nullptr);
        }
        std::cout << "------------------------------------------------------" << std::endl;
        RecDataStoragePtr receiveDataPtr = tcpConnection.requestValues();
        if(nullptr == receiveDataPtr) {
            std::cout << "Reconnecting ..." << std::endl;
            // Reconnect, it seems there is connection error
            tcpConnection.disconnectFromHeating();
            if(false == tcpConnection.connectToHeating()) {
                // Reconnect failed
                std::cout << "Reconnecting FAILED!!!" << std::endl;
                return 4;
            }
            // Try a second time...
            receiveDataPtr = tcpConnection.requestValues();
        }

        if(nullptr != receiveDataPtr) {
            ValueResponse decodeValueResp(receiveDataPtr, currentUnixTime);
            // decodeValueResp.decode();
            decodeValueResp.serialize();
        } else {
            std::cout << "Second consecutive failure" << std::endl;
            return 5;
        }
        if(false == resetConnectionAt0300(&tcpConnection, &sync)) {
            return 6;
        }
        if(ApplicationState::NORMAL == applState) {
            sleep(1);
        }else{
            sleep(5);
        }
    }

    tcpConnection.disconnectFromHeating();
    return 0;
}

bool resetConnectionAt0300(TcpConnection* pConnection, SynchronizeTime* pTime)
{
    if(true == pTime->isFullHour(false)) {
        if(3 == pTime->getHour(false)) {
            // Periodically reconnect
            pConnection->disconnectFromHeating();
            if(false == pConnection->connectToHeating()) {
                // Reconnect failed
                std::cout << "Periodic reconnecting FAILED!!!" << std::endl;
                return false;
            }
        }
    }
    return true;
}
