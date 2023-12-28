#include <iostream>

#include "tcpclient.hpp"

int main(int argc, char * argv[])
{
    TCPClient client;
    client.setHost("127.0.0.1");
    client.setHostPort(49160);
    client.setPort(49161);
    std::cout << "Client started!\n";
    client.run();
}