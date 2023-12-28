 
#include <iostream>
#include <string>
#include <stdlib.h>
#include "tcpserver.hpp"
#include "udpserver.hpp"
#include "numbersgetter.hpp"
#include "../error.hpp"


BaseServer * create_server(std::string con_type)
{
    BaseServer * server = nullptr;

    if(con_type == "TCP")
    {
        server = new TCPServer();
    }
    else if(con_type == "UDP")
    {
        server = new UDPServer();
    }
    else
    {
        std::cerr << "Unknown protocol" << '\n';
    }

    return server;
}

int main(int argc, char * argv[])
{
    if(argc < 4)
    {
        std::cout << "Usage:\n ./Server [UDP/TCP] [ip] [port]\n";
    }

    BaseServer * server = create_server(argv[0]);
    //char ip[16] = "127.0.0.1";
    std::string ip = argv[1];
    server->setIP(ip);
    uint port = atoi(argv[2]);
    server->setPort(port);
    server->setDataConverter(new NumbersGetter);
    server->run();
}