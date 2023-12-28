#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "baseclient.hpp"

class TCPClient : public BaseClient
{
public:
    TCPClient();
protected:
    void createConnection();
    void sendMessage(std::string message);
};

#endif