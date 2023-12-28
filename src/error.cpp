#include "error.hpp"

void error(std::string error_message)
{
    std::cerr << error_message << " Error: " << errno << '\n';
    exit(1);
}