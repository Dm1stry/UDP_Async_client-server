#include "tcpclient.hpp"

TCPClient::TCPClient()
  : BaseClient()
{

}

void TCPClient::createConnection()
{
    client_descriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    if(client_descriptor_ < 0)
    {
        error("socket() failed");
    }
    
    bzero(&server_addr_, sizeof(server_addr_));

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(host_port_);
    server_addr_.sin_addr.s_addr = inet_addr(host_ip_);

    bzero(&client_addr_, sizeof(client_addr_));

    client_addr_.sin_family = AF_INET;
    client_addr_.sin_port = htons(port_);
    client_addr_.sin_addr.s_addr = INADDR_ANY; //inet_addr(host_ip_);

    /*if(bind(client_descriptor_, (const struct sockaddr *)&client_addr_, sizeof(client_addr_)) < 0) 
    { 
        error("bind() failed"); 
    }*/

    if (connect(client_descriptor_, (sockaddr *)&server_addr_, sizeof(server_addr_)) != 0)
    {
        error("connection with the server failed");
    }

    std::cout << "Connection created. Port: " << port_ << ", Host: " << host_ip_ << ", host_port: " << host_port_ << "fd = " << client_descriptor_<< '\n';
}

void TCPClient::sendMessage(std::string message)
{
    //ssize_t l = recv(client_descriptor_, buffer_, BUFFER_SIZE, 0);

    const char* buffer = (message + '\0').c_str();
    ssize_t bytes = send(client_descriptor_, message.data(), message.size(), 0);
    std::cout << bytes;
    std::cout << "Message sent: " << message << '\n';
    bzero(buffer_, BUFFER_SIZE);
    
    ssize_t len = recv(client_descriptor_, buffer_, BUFFER_SIZE, 0);
    buffer_[len] = '\0';
    std::cout << "Response received:\n" << std::string(buffer_, len) << '\n';
}