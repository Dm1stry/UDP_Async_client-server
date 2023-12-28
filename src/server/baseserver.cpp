#include "baseserver.hpp"

BaseServer::BaseServer()
  : ev_base_(event_base_new()),
    ip_(new char[16])
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
    strcpy(ip_, ip);
    return true;
}

bool BaseServer::setIP(std::string ip)
{
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

void BaseServer::run()
{
    server_data_.fd = socket(AF_INET, communicationType(), 0);

    if(server_data_.fd < 0)
    {
        error("socket() failed");
    }

    if(evutil_make_socket_nonblocking(server_data_.fd) < 0)
    {
        error("evutil_make_socket_nonblocking() failed");
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port_);
    sin.sin_addr.s_addr = inet_addr(ip_);

    if(bind(server_data_.fd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        error("bind() failed");
    }

    if(!ev_base_)
    {
        error("empty event_base");
    }

    serverSocketManipulations();  //Protocol-Specific socket manipulations

    std::cout << "Server started!\nIP: " << ip_ << "\nPort: " << port_ << '\n';

    if(event_base_dispatch(ev_base_) < 0)
    {
        error("event_base_dispatch() failed");
    }

    clearSocketData(server_data_);
}

void BaseServer::clearSocketData(SocketData& data)
{
    if(data.read_event)
    {
        event_del(data.read_event);
        event_free(data.read_event);
        data.read_event = nullptr;
    }

    if(data.write_event) {
        event_del(data.write_event);
        event_free(data.write_event);
        data.write_event = nullptr;
    }
    data.message = "";
    close(data.fd);
}