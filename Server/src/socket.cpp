#include "../include/socket.h"

Socket::Socket(tcp::socket&& socket) : socket(std::move(socket)) { }

void Socket::Start(message_handler&& on_message, error_handler&& on_error) {
    this->on_message = std::move(on_message);
    this->on_error = std::move(on_error);
    AsyncRead();
}

void Socket::WriteData(error_code error, std::size_t bytes_transferred) {
    if(!error)
    {
        outgoing.pop();
        if(!outgoing.empty())
            AsyncWrite();
    }
    else
    {
        Logger::Send(tr::error,error.message().c_str());
        socket.close(error);
        on_error();
    }
}

void Socket::AsyncWrite() {
    io::async_write(socket, io::buffer(outgoing.front()), std::bind(&Socket::WriteData, shared_from_this(), _1, _2));
}

void Socket::ReadData(error_code error, std::size_t bytes_transferred) {
    if(!error)
    {
        std::stringstream message;
        if(!member.IsAuthorized()) {
            std::string nick;
            std::istream(&streambuf) >> nick;
            member.AuthMember(nick);
            Logger::Send(tr::debug,"Client has been authorized");
            message << "Приветствуем в нашем чате: " << nick << std::endl; 
        }
        else
            message << member.GetNick() << ": " << std::istream(&streambuf).rdbuf();
        on_message(message.str());
        streambuf.consume(bytes_transferred);
        AsyncRead();
    }
    else
    {
        Logger::Send(tr::error,error.message().c_str());
        socket.close(error);
        on_error();
    }
}

void Socket::AsyncRead() {
    io::async_read_until(socket, streambuf, "\n", std::bind(&Socket::ReadData, shared_from_this(), _1, _2));
}
void Socket::Send(const std::string &message) {
    bool idle = outgoing.empty();
    outgoing.push(message);
    if(idle)
        AsyncWrite();
}

ChatMember* Socket::GetChatMember() { return &member; }