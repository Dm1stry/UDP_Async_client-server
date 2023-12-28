#ifndef NUMBERS_GETTER_HPP
#define NUMBERS_GETTER_HPP

#include <vector>
#include <algorithm>
#include "dataconverter.hpp"
#include "../../libs/LNL.h"

class NumbersGetter : public DataConverter
{
public:
    std::string convert(std::string message);
};

#endif