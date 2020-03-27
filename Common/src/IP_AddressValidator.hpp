#pragma once

#include <string>
#include <vector>

class IP_AddressValidator
{
public:
    IP_AddressValidator(std::string ipAddr);
    ~IP_AddressValidator();

    bool validate();
private:
    /// check if given string is a numeric string or not
    bool isNumber(const std::string& str);

    /// Function to split string str using given delimiter
    std::vector<std::string> split(const std::string& str, char delim);

    std::string m_ipAddr;
};
