#include "baseclient.hpp"

BaseClient::BaseClient()
  : buffer_("")
{
    
}

BaseClient::~BaseClient()
{
    close(client_descriptor_);
}

void BaseClient::setHost(std::string host_ip)
{
    if(!host_ip_)
    {
        host_ip_ = new char[16];
    }
    strcpy(host_ip_, host_ip.c_str());
}

void BaseClient::setHostPort(uint port)
{
    host_port_ = port;
}

void BaseClient::setPort(uint port)
{
    port_ = port;
}

void BaseClient::run()
{
    createConnection();
    std::string message;
    while(getline(std::cin, message, '\n'))
    {
        sendMessage(message);
        message = "";
    }
}