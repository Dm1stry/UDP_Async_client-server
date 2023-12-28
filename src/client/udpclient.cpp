#include "udpclient.hpp"

UDPClient::UDPClient()
  : BaseClient()
{}

void UDPClient::createConnection()
{
    client_descriptor_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_descriptor_ < 0)
    {
        error("socket() failed");
    }
    
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(host_port_);
    server_addr_.sin_addr.s_addr = inet_addr(host_ip_);

    client_addr_.sin_family = AF_INET;
    client_addr_.sin_port = htons(port_);
    client_addr_.sin_addr.s_addr = INADDR_ANY; //inet_addr(host_ip_);


    if(bind(client_descriptor_, (const struct sockaddr *)&client_addr_, sizeof(client_addr_)) < 0) 
    { 
        error("bind() failed"); 
    }

    std::cout << "Connection created. Port: " << port_ << ", Host: " << host_ip_ << ", host_port: " << host_port_ << "fd = " << client_descriptor_<< '\n';
}

void UDPClient::sendMessage(std::string message)
{
    std::fill(buffer_, buffer_ + BUFFER_SIZE, '\0');
    sendto(client_descriptor_, message.data(), message.size(), 0, (const struct sockaddr *) &server_addr_, sizeof(server_addr_));

    std::cout << "Message sent: " << message << '\n';
    socklen_t len;
    ssize_t bytes;

    bytes = recvfrom(client_descriptor_, buffer_, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr_, &len);

    std::cout << "Response received:\n" << std::string(buffer_, len) << '\n';
}