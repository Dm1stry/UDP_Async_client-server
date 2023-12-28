#ifndef BASE_CLIENT_HPP
#define BASE_CLIENT_HPP

#include <string>
#include <cstring> //strcpy
#include <iostream>

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

#include "../error.hpp"

#define BUFFER_SIZE 1024

class BaseClient
{
public:
    BaseClient();
    virtual ~BaseClient();
    void setHost(std::string ip);
    void setHostPort(uint port);
    void setPort(uint port);
    void run();
protected:
    virtual void createConnection() = 0;
    virtual void sendMessage(std::string message) = 0;

    char * host_ip_;
    uint host_port_;
    uint port_;

    sockaddr_in server_addr_;
    sockaddr_in client_addr_;

    int client_descriptor_;
    
    char buffer_[BUFFER_SIZE];
};

#endif