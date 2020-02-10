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

class RecDataStorage
{
public:
    RecDataStorage();

    void clearBuffer();
    bool addData(uint8_t* pBuffer, size_t nrChar);

private:
    static const size_t BufferSize = 5000;
    uint8_t m_buffer[BufferSize];
};
