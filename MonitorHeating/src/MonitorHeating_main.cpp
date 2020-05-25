#include <ctime>
#include <iostream>
#include <unistd.h>

#include "ValueResponse.hpp"
#include "SynchronizeTime.hpp"
#include "TcpConnection.hpp"
#include "Common/src/IP_AddressValidator.hpp"
#include "Common/src/ProgramVersion.hpp"

/// Reset the connection at 3 in the morning
/// \param pConnection, reference to the TCP connection
/// \param pTime, reference to the current time
void resetConnectionAt0300(TcpConnection& tcpConnection, SynchronizeTime* pTime);

/// Connect or reconnect to the heating controller. Keeps trying every minute
///  until success.
void connectToHeatingController(TcpConnection& tcpConnection);

/// Application state, to enable some debug functions
enum class ApplicationState {
    NORMAL,
    DEBUG
};


/// Main function to setup the connection, restart it periodically, read the data and forwards them to the decoder / serializer.
int main(int argc, char* argv[])
{
    ProgramVersion progVersion;
    progVersion.printProgramInfo("MonitorHeating");

    SynchronizeTime sync;
    std::time_t currentUnixTime;
    ApplicationState applState = ApplicationState::NORMAL;
    std::string ipAddr;

    std::cout << "Application started" << std::endl;

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
    // Connect to the controller, keeps trying...
    connectToHeatingController(tcpConnection);

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
            // Connect to the controller, keeps trying...
            connectToHeatingController(tcpConnection);
            // Try a second time to get the data...
            receiveDataPtr = tcpConnection.requestValues();
        }

        if(nullptr != receiveDataPtr) {
            ValueResponse decodeValueResp(receiveDataPtr, currentUnixTime, true);
            decodeValueResp.serialize();
        } else {
            std::cout << "Second consecutive failure" << std::endl;
            return 5;
        }
        // Reset the connection every 24h
        resetConnectionAt0300(tcpConnection, &sync);
        if(ApplicationState::NORMAL == applState) {
            sleep(1);
        } else {
            sleep(5);
        }
    }

    tcpConnection.disconnectFromHeating();
    return 0;
}

void resetConnectionAt0300(TcpConnection& tcpConnection, SynchronizeTime* pTime)
{
    if(true == pTime->isFullHour(false)) {
        if(3 == pTime->getHour(false)) {
            // Periodically reconnect
            tcpConnection.disconnectFromHeating();
            // Connect to the controller, keeps trying...
            connectToHeatingController(tcpConnection);
        }
    }
}

void connectToHeatingController(TcpConnection& tcpConnection)
{
    bool isConnected = false;
    std::time_t currentUnixTime = std::time(nullptr);
    SynchronizeTime syncTime;
    do {
        isConnected = tcpConnection.connectToHeating();
        if(false == isConnected) {
            std::cout << "Could not establish connection, try again in a minute. Current time: " << std::asctime(std::localtime(&currentUnixTime));
            currentUnixTime = syncTime.waitForMinute();
        }

    } while(false == isConnected);
    std::cout << "Got connection at: " << std::asctime(std::localtime(&currentUnixTime)) << std::flush;
}
