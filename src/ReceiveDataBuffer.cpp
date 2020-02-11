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
    memcpy(&(m_buffer.buffer[m_writePos]), pBuffer, nrChar);
    m_writePos += nrChar;
    return false;
}

void RecDataStorage::printBuffer()
{
    printf("---------------------------------\n");
    printf(" max size: %ld\n", BufferSize);
    printf(" act size: %ld\n\n", m_writePos);

    for (size_t cnt = 0; cnt < m_writePos; cnt++) {
        printf("0x%.2x ", m_buffer.buffer[cnt]);
        if (0 == ((cnt + 1) % 16)) {
            printf("\n");
        }
    }
    printf("\n---------------------------------\n");
    printf(" command:   %d / 0x%.4x\n", swap(m_buffer.decode.command), swap(m_buffer.decode.command));
    printf(" nrEntries: %d\n", swap(m_buffer.decode.nrEntries));
    printf("---------------------------------\n");
    //    for (uint32_t paraNr = 0; paraNr < 10 /*NrParameter*/; paraNr++) {
    //        printf("para[%.3d] = %d / 0x%.4x\n", paraNr, swap(m_buffer.decode.data[paraNr]));
    //    }
    for (uint32_t paraCnt = 0; paraCnt < NrParameter; paraCnt++) {
        printf("para[%.4d] = %d\n", paraCnt, swap(m_buffer.decode.data[paraCnt]));
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
