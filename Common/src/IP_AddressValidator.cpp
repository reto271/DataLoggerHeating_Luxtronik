#include "IP_AddressValidator.hpp"
#include <iostream>


IP_AddressValidator::IP_AddressValidator(std::string ipAddr)
    : m_ipAddr(ipAddr)
{
}

IP_AddressValidator::~IP_AddressValidator()
{
}

bool IP_AddressValidator::isNumber(const std::string& str)
{
    // std::find_first_not_of searches the string for the first character
    // that does not match any of the characters specified in its arguments
    return !str.empty() &&
           (str.find_first_not_of("[0123456789]") == std::string::npos);
}

// Function to split string str using given delimiter
std::vector<std::string> IP_AddressValidator::split(const std::string& str, char delim)
{
    auto i = 0;
    std::vector<std::string> list;

    auto pos = str.find(delim);

    while(pos != std::string::npos) {
        list.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(delim, pos);
    }

    list.push_back(str.substr(i, str.length()));

    return list;
}

// Function to validate an IP address
bool IP_AddressValidator::validate()
{
    // split the string into tokens
    std::vector<std::string> list = split(m_ipAddr, '.');

    // if token size is not equal to four
    if(list.size() != 4) {
        return false;
    }

    // validate each token
    for(std::string str : list) {
        // verify that string is number or not and the numbers
        // are in the valid range
        if((false == isNumber(str)) || (stoi(str) > 255) || (stoi(str) < 0)) {
            return false;
        }
    }

    return true;
}
