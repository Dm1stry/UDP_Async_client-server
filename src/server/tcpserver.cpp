#include "tcpserver.hpp"

TCPServer::TCPServer()
  : BaseServer()
{}

int TCPServer::communicationType()
{
    return SOCK_STREAM;
}

void TCPServer::serverSocketManipulations()
{
    if(listen(server_data_.fd, 1000) < 0)
    {
        error("listen() failed");
    }

    struct event* accept_event = event_new(ev_base_, server_data_.fd,
        EV_READ | EV_PERSIST, invokeOnAccept, (void*)this);
    if(!accept_event)
        error("event_new() failed");

    server_data_.read_event = accept_event;

    if(event_add(accept_event, NULL) < 0)
    {
        error("event_add() failed");
    }
}

void TCPServer::invokeOnAccept(int server_descriptor, short flags, void *arg)
{
    static_cast<TCPServer*>(arg)->onAccept(server_descriptor, flags);  //Костыль, чтобы вызвать нестатический метод из статического. Коллбэк не принимает нестатические методы
}

void TCPServer::onAccept(int server_descriptor, short flags)
{
    int fd = accept(server_descriptor, 0, 0);

    SocketData client_data;

    client_data.fd = fd;

    if(fd < 0)
    {
        error("accept() failed");
    }

    if(evutil_make_socket_nonblocking(fd) < 0)
    {
        error("evutil_make_socket_nonblocking() failed");
    }

    printf("New connection! fd = %d\n", fd);

    client_data.read_event = event_new(ev_base_, fd, EV_READ | EV_PERSIST, invokeOnRead, (void*)this);

    if(!client_data.read_event)
    {
        error("event_new(... EV_READ ...) failed");
    }

    if(event_add(client_data.read_event, NULL) < 0)
    {
        error("event_add(read_event, ...) failed");
    }

    client_data.write_event = event_new(ev_base_, fd, EV_WRITE | EV_PERSIST, invokeOnWrite, (void*)this);

    if(!client_data.write_event)
    {
        error("event_new(... EV_READ ...) failed");
    }

    clients_data_[fd] = client_data;
}

void TCPServer::invokeOnRead(int client_descriptor, short flags, void *arg)
{
    static_cast<TCPServer*>(arg)->onRead(client_descriptor, flags);  //Костыль, чтобы вызвать нестатический метод из статического. Коллбэк не принимает нестатические методы
}

void TCPServer::onRead(int client_descriptor, short flags)
{
    SocketData& client_data = clients_data_[client_descriptor];
    int message_len;
    for(;;)
    {
        message_len = read(client_descriptor, client_data.buffer, BUFFER_SIZE);
        if(message_len == 0) 
        {
            clearSocketData(client_data);
            clients_data_.erase(client_descriptor);
            return;
        }
        if(message_len < 0) {
            if(errno == EINTR)
                continue;

            clearSocketData(client_data);
            clients_data_.erase(client_descriptor);
            return;
        }
        break;
    }

    std::string message(client_data.buffer);
    std::cout << "Message from client fd = " << client_data.fd << ": " << message << '\n';
    //size_t substr_len = message.find("\n") + 1;
    //message = message.substr(0, substr_len);

    client_data.message = converter_->convert(message);

    if(event_add(client_data.write_event, NULL) < 0)
    {
        error("event_add(peer->write_event, ...) failed");
    }
    bzero(client_data.buffer, BUFFER_SIZE);
}

void TCPServer::invokeOnWrite(int client_descriptor, short flags, void *arg)
{
    static_cast<TCPServer*>(arg)->onWrite(client_descriptor, flags);  //Костыль, чтобы вызвать нестатический метод из статического. Коллбэк не принимает нестатические методы
}

void TCPServer::onWrite(int client_descriptor, short flags)
{
    SocketData& client_data = clients_data_[client_descriptor];
    ssize_t bytes;
    for(;;) {
        bytes = write(client_descriptor, client_data.message.c_str(), client_data.message.length());
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
}