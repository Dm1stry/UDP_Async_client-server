#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <memory>
#include <unordered_map>

#include "baseserver.hpp"

#define BUFFER_SIZE 1024

class TCPServer : public BaseServer
{
public:
    TCPServer();
    virtual ~TCPServer();
    void run();

    struct SocketData
    {
        event *read_event = nullptr;
        event *write_event = nullptr;
        char buffer[BUFFER_SIZE];
        ssize_t message_len = 0;
        std::string message;
        int fd;    
    };

private:
    static void invokeOnAccept(int server_descriptor, short flags, void *arg);
    static void invokeOnRead(int client_descriptor, short flags, void *arg);
    static void invokeOnWrite(int client_descriptor, short flags, void *arg);

    void onAccept(int server_descriptor, short flags);
    void onRead(int client_descriptor, short flags);
    void onWrite(int client_descriptor, short flags);

    void clearSocketData(SocketData& data);

    SocketData server_data_;
    std::unordered_map <int, SocketData> clients_data_;
};

#endif