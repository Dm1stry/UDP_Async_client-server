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
#include <unordered_map>

#include "dataconverter.hpp"
#include "../error.hpp"

#define BUFFER_SIZE 1024

class BaseServer
{
public:
    BaseServer();
    //BaseServer(IPv ip_version = BaseServer::IPv::IPv4);
    virtual ~BaseServer();

    struct SocketData
    {
        event *read_event = nullptr;
        event *write_event = nullptr;
        char buffer[BUFFER_SIZE];
        std::string message;
        sockaddr_in cliaddr;
        int fd;  
    };

    bool setIP(std::string ip);
    bool setIP(char * ip);
    bool setPort(uint port);
    void setDataConverter(DataConverter * converter);

    void run();
protected:
    inline virtual int communicationType() = 0;
    virtual void serverSocketManipulations() = 0;  //Код, отличающийся для UDP и TCP в методе run()
    void clearSocketData(SocketData& data);

    char * ip_;
    uint port_;

    event_base * ev_base_;
    SocketData server_data_;
    std::unordered_map <int, SocketData> clients_data_;

    DataConverter * converter_;
};



#endif