#pragma once

#include <memory>

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


typedef enum
{
    ReadParam,
    ReadValue
} HeatControlCommand;


class RecDataStorage;
using RecDataStoragePtr = std::shared_ptr<class RecDataStorage>;

class RecDataStorage
{
public:
    RecDataStorage();

    void clearBuffer();
    bool addData(uint8_t* pBuffer, size_t nrChar);

    uint32_t getDataField(const uint32_t index);
    HeatControlCommand getHeatControlCommand();

    void printBuffer();
    void printCommand();
    uint32_t swap(uint32_t value);


private:
    static const size_t BufferSize = 5000;
    static const uint32_t NrParameter = 1087;
    size_t m_writePos;

    typedef struct
    {
        uint32_t commandResponse;
        uint32_t nrEntries;
        uint32_t data[NrParameter];
    } DecodedBuffer;

    typedef union
    {
        uint8_t buffer[BufferSize];
        DecodedBuffer decode;
    } DataBuffer;

    typedef struct
    {
        HeatControlCommand heatControlCommand;
        DataBuffer data;
    } ResponseBuffer;

    ResponseBuffer m_response;
};
