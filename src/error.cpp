#include "error.hpp"

void error(std::string error_message)
{
    std::cerr << error_message << errno << '\n';
    exit(1);
}