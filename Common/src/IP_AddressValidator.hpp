#pragma once

#include <string>
#include <vector>

/// Validates if the given string is a valid IP address. The format must suffice
///  xxx.xxx.xxx.xxx where xxx = [0..255].
class IP_AddressValidator
{
public:
    /// Constructor of the IP address validator. The user must provide the string to be validated.
    IP_AddressValidator(std::string ipAddr);

    /// Destructs the IP address validator
    ~IP_AddressValidator();

    /// Validates the IP address. There need to be four blocks separated by dots. Each block must be
    ///  the range 0..255.
    /// \return true if the IP Address is valid, false otherwise.
    bool validate();
private:
    /// check if given string is a numeric string or not
    /// \param[in] str to be validated as number
    /// \return true, if the provided string is a number.
    bool isNumber(const std::string& str);

    /// Function to split string str using given delimiter
    /// \param[in] str is a string to be split into several strings.
    /// \param[in] delim is the character used to split the string.
    /// \return vector of sub-strings
    std::vector<std::string> split(const std::string& str, char delim);

    std::string m_ipAddr;  ///< IP address to be validated as string.
};
