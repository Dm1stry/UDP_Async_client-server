#include <iostream>
#include <string>
#include <stdlib.h>
#include "tcpclient.hpp"
#include "udpclient.hpp"

BaseClient * create_client(std::string con_type)
{
    BaseClient * client = nullptr;

    if(con_type == "TCP")
    {
        client = new TCPClient();
    }
    else if(con_type == "UDP")
    {
        client = new UDPClient();
    }
    else
    {
        std::cerr << "Unknown protocol" << '\n';
    }

    return client;
}

int main(int argc, char * argv[])
{
    if(argc < 5)
    {
        std::cout << "Usage:\n ./Server [UDP/TCP] [ip] [dest_port] [source_port]\n";
    }
    else
    {
        BaseClient * client = create_client(argv[1]);
        std::string ip = argv[2];
        client->setHost(ip);
        uint dest_port = atoi(argv[3]);
        client->setHostPort(dest_port);
        uint source_port = atoi(argv[4]);
        client->setPort(source_port);
        client->run();
    }
}