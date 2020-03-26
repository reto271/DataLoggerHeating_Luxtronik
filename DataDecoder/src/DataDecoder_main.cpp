#include <iostream>

#include "FileDataReader.hpp"


/// Main function to setup the connection, restart it periodically, read the data and forwards them to the decoder / serializer.
int main(int argc, char* argv[])
{
    if(argc != 2) {
        std::cout << std::endl << "Usage: " << argv[0] << " <FileName>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::cout << "Processing: " << fileName << std::endl;

    FileDataReader fileDataReader(fileName);
    if(false == fileDataReader.readHeaderData()) {
        return 2;
    }

    if(false == fileDataReader.decodeData()) {
        return 3;
    }
    return 0;
}
