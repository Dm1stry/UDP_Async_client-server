#include "baseserver.hpp"

BaseServer::BaseServer()
  : ev_base_(event_base_new())
{}

BaseServer::~BaseServer()
{}

bool BaseServer::setIP(char * ip)
{
    ip_ = new char[16];
    strcpy(ip_, ip);
    return true;
}

bool BaseServer::setIP(std::string ip)
{
    ip_ = new char[16];
    strcpy(ip_, ip.c_str());
    return true;
}

bool BaseServer::setPort(uint port)
{
    port_ = port;
    return true;
}