#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include "baseserver.hpp"
#include <unordered_map>

class UDPServer : public BaseServer
{
public:
    UDPServer();
    ~UDPServer();
    void run();

    struct SocketData
    {
        event *read_event = nullptr;
        event *write_event = nullptr;
        char buffer[BUFFER_SIZE];
        std::string message;
        sockaddr_in cliaddr;
        int fd;    
    };

private:
    static void invokeOnRead(int client_descriptor, short flags, void *arg);
    static void invokeOnWrite(int client_descriptor, short flags, void *arg);

    void onRead(int client_descriptor, short flags);
    void onWrite(int client_descriptor, short flags);

    void clearSocketData(SocketData& data);

    SocketData server_data_;
    std::unordered_map <int, SocketData> clients_data_;
};

#endif