#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include "baseserver.hpp"

class UDPServer : public BaseServer
{
public:
    UDPServer();

protected:
    void serverSocketManipulations();
    int communicationType();
private:
    static void invokeOnRead(int client_descriptor, short flags, void *arg);
    static void invokeOnWrite(int client_descriptor, short flags, void *arg);

    void onRead(int client_descriptor, short flags);
    void onWrite(int client_descriptor, short flags);
};

#endif