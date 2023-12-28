#include "udpserver.hpp"

UDPServer::UDPServer()
  : BaseServer()
{}

int UDPServer::communicationType()
{
    return SOCK_DGRAM;
}

void UDPServer::socketManipulations()
{
    server_data_.read_event = event_new(ev_base_, server_data_.fd, EV_READ | EV_PERSIST, invokeOnRead, (void*)this);

    if(event_add(server_data_.read_event, NULL) < 0)
    {
        error("event_add() failed");
    }
}

void UDPServer::invokeOnRead(int client_descriptor, short flags, void *arg)
{
    static_cast<UDPServer*>(arg)->onRead(client_descriptor, flags);  //Костыль, чтобы вызвать нестатический метод из статического. Коллбэк не принимает нестатические методы
}

void UDPServer::onRead(int client_descriptor, short flags)
{
    sockaddr_in cliaddr;
    socklen_t size = sizeof(struct sockaddr);
    ssize_t bytes;

    SocketData client_data;
    client_data.fd = client_descriptor;

    bytes = recvfrom(client_descriptor, client_data.buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &size);

    std::string message(client_data.buffer);
    size_t substr_len = message.find("\n");
    if(substr_len == std::string::npos)
    {
        substr_len = message.length();
    }
    else
    {
        substr_len += 1;
    }
    message = message.substr(0, substr_len);

    client_data.message = converter_->convert(message);

    client_data.write_event = event_new(ev_base_, client_data.fd, EV_WRITE | EV_PERSIST, invokeOnWrite, (void*)this);

    if(event_add(client_data.write_event, NULL) < 0)
    {
        error("event_add(peer->write_event, ...) failed");
    }
    client_data.cliaddr = cliaddr;
    clients_data_[client_descriptor] = client_data;
    bzero(client_data.buffer, BUFFER_SIZE);
}

void UDPServer::invokeOnWrite(int client_descriptor, short flags, void *arg)
{
    static_cast<UDPServer*>(arg)->onWrite(client_descriptor, flags);  //Костыль, чтобы вызвать нестатический метод из статического. Коллбэк не принимает нестатические методы
}

void UDPServer::onWrite(int client_descriptor, short flags)
{
    SocketData& client_data = clients_data_[client_descriptor];
    ssize_t bytes;
    socklen_t size = sizeof(struct sockaddr);
    for(;;) {
        bytes = sendto(client_descriptor, client_data.message.c_str(), client_data.message.length(), 0, (sockaddr *)&(client_data.cliaddr), size);
        if(bytes <= 0) {
            if(errno == EINTR)
                continue;

            clearSocketData(client_data);
            clients_data_.erase(client_descriptor);
            return;
        }

        break; // write() succeeded
    }

    if(event_del(client_data.write_event) < 0)
    {
        error("event_del() failed");
    }
    client_data.message = "";
}