#ifndef DATACONVERTER_HPP
#define DATACONVERTER_HPP

#include <string>

class DataConverter
{
public:
    virtual std::string convert(std::string data) = 0;
};

#endif