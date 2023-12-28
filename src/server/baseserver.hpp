#ifndef BASE_SERVER_HPP
#define BASE_SERVER_HPP

#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <event2/event.h>
#include <netinet/in.h> // keep this, for FreeBSD
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dataconverter.hpp"
#include "../error.hpp"

#define BUFFER_SIZE 1024

class BaseServer
{
public:
    BaseServer();
    //BaseServer(IPv ip_version = BaseServer::IPv::IPv4);
    virtual ~BaseServer();

    bool setIP(std::string ip);
    bool setIP(char * ip);
    bool setPort(uint port);
    void setDataConverter(DataConverter * converter);

    virtual void run() = 0;
protected:
    char * ip_;
    uint port_;

    event_base * ev_base_;

    DataConverter * converter_;
};

#endif