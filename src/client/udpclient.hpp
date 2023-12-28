#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include "baseclient.hpp"
#include <algorithm>

class UDPClient : public BaseClient
{
public:
    UDPClient();
    void createConnection();
    void sendMessage(std::string message);
};

#endif