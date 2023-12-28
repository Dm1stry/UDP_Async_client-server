#include "baseserver.hpp"

BaseServer::BaseServer()
  : ev_base_(event_base_new())
{}

BaseServer::~BaseServer()
{
    delete[] ip_;
    ip_ = nullptr;
    delete converter_;
    converter_ = nullptr;
    event_base_free(ev_base_);
}

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

void BaseServer::setDataConverter(DataConverter * converter)
{
    converter_ = converter;
}