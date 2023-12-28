 
#include <iostream>
#include "tcpserver.hpp"
#include "udpserver.hpp"
#include "numbersgetter.hpp"

int main() //int argc, char * argv[])
{
    TCPServer server;
    //char ip[16] = "127.0.0.1";
    std::string ip = "127.0.0.1";
    server.setIP(ip);
    server.setPort(49160);
    server.setDataConverter(new NumbersGetter);
    std::cout << "Server started!" << '\n';
    server.run();
}