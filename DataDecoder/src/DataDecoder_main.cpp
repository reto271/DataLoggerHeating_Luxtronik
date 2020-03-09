#include <iostream>

#include "FileDataReader.hpp"


/// Main function to setup the connection, restart it periodically, read the data and forwards them to the decoder / serializer.
int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << std::endl << "Usage: " <<  argv[0] << " <FileName>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];

    FileDataReader fileDataReader(fileName);
    if (false == fileDataReader.readFromFile()) {
        return 1;
    }

    if (false == fileDataReader.decodeBufferV1()) {
        return 1;
    }

    if (false == fileDataReader.writeToCSV()) {
        return 1;
    }
    return 0;
}
