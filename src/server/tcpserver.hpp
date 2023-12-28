#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <memory>
#include <unordered_map>

#include "baseserver.hpp"

class TCPServer : public BaseServer
{
public:
    TCPServer();

protected:
    void serverSocketManipulations();
    inline virtual int communicationType();

private:
    static void invokeOnAccept(int server_descriptor, short flags, void *arg);
    static void invokeOnRead(int client_descriptor, short flags, void *arg);
    static void invokeOnWrite(int client_descriptor, short flags, void *arg);

    void onAccept(int server_descriptor, short flags);
    void onRead(int client_descriptor, short flags);
    void onWrite(int client_descriptor, short flags);
};

#endif