#ifndef SERVER_H
#define SERVER_H

#include "../include/socket.h"

class Server
{
private:
    io::io_context& io_context;
    tcp::acceptor acceptor;
    std::optional<tcp::socket> socket;
    std::unordered_set<std::shared_ptr<Socket>> clients;
public:
    Server(io::io_context&, std::uint16_t);
    void AsyncAccept();
    void Send(const std::string &);
};

#endif