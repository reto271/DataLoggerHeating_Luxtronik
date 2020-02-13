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

RecDataStorage::RecDataStorage()
{
    clearBuffer();
}

void RecDataStorage::clearBuffer()
{
    m_writePos = 0;
}

bool RecDataStorage::addData(uint8_t* pBuffer, size_t nrChar)
{
    memcpy(&(m_response.data.buffer[m_writePos]), pBuffer, nrChar);
    m_writePos += nrChar;
    return false;
}

uint32_t RecDataStorage::getDataField(const uint32_t index)
{
    return swap(m_response.data.decode.data[index]);
}

HeatControlCommand RecDataStorage::getHeatControlCommand()
{
    return m_response.heatControlCommand;
}

void RecDataStorage::printBuffer()
{
    printf("---------------------------------\n");
    printf(" max size: %ld\n", BufferSize);
    printf(" act size: %ld\n\n", m_writePos);

//-     for (size_t cnt = 0; cnt < m_writePos; cnt++) {
//-         printf("0x%.2x ", m_response.data.buffer[cnt]);
//-         if (0 == ((cnt + 1) % 16)) {
//-             printf("\n");
//-         }
//-     }
    printf("\n---------------------------------\n");
    printf(" command:   %d / 0x%.4x\n", swap(m_response.data.decode.commandResponse), swap(m_response.data.decode.commandResponse));
    printf(" nrEntries: %d\n", swap(m_response.data.decode.nrEntries));
    printCommand();
    printf("---------------------------------\n");
    //    for (uint32_t paraNr = 0; paraNr < 10 /*NrParameter*/; paraNr++) {
    //        printf("para[%.3d] = %d / 0x%.4x\n", paraNr, swap(m_buffer.decode.data[paraNr]));
    //    }
    for (uint32_t paraCnt = 0; paraCnt < NrParameter; paraCnt++) {
        uint32_t value = swap(m_response.data.decode.data[paraCnt]);
        float* pFloat = reinterpret_cast<float*>(&value);
        //printf("para[%.4d] = %12d / 0x%.8x / %f\n", paraCnt, value, value, *pFloat);
        printf("para[%.4d] = %12d / %3.1f / 0x%.8x / %f\n", paraCnt, value, static_cast<float>(value) / 10, value, *pFloat);
    }
}

void RecDataStorage::printCommand()
{
    switch(m_response.heatControlCommand)
    {
    case ReadParam:
        printf(" request: ReadParam\n");
        break;
    case ReadValue:
        printf(" request: ReadValue\n");
        break;
    default:
        printf(" request: UNKNOWN\n");
        break;
    }
}

uint32_t RecDataStorage::swap(uint32_t value)
{
    typedef union
    {
        uint32_t value;
        uint8_t byte[4];
    }  ConvertValue;

    ConvertValue conv;
    conv.value = value;

    return conv.byte[0] << 24 | conv.byte[1] << 16 | conv.byte[2] << 8 | conv.byte[3];
}
