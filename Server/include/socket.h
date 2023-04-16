#ifndef SOCKET_H
#define SOCKET_H

#include <boost/asio.hpp>
#include <optional>
#include <queue>
#include <unordered_set>
#include "../include/chat_member.h"
#include "../include/logger.h"
#include <iostream>
#include <string>

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;
using namespace std::placeholders;

using message_handler = std::function<void (std::string)>;
using error_handler = std::function<void ()>;

class Socket : public std::enable_shared_from_this<Socket>
{
private:
    tcp::socket socket;
    io::streambuf streambuf;
    std::queue<std::string> outgoing;
    message_handler on_message;
    error_handler on_error;
    ChatMember member;

    void AsyncRead();
    void ReadData(error_code, std::size_t);
    void AsyncWrite();
    void WriteData(error_code, std::size_t);
public:
    Socket(tcp::socket&&);
    void Start(message_handler&&, error_handler&&);
    void Send(std::string const&);
    ChatMember* GetChatMember();
};

#endif