#include "../include/server.h"

Server::Server(io::io_context& context, std::uint16_t port) : io_context(context)
    , acceptor(io_context, tcp::endpoint(tcp::v4(), port)) { 
    Logger::Send(tr::info, "Server starts at port: " + port);
}

void Server::AsyncAccept() {
    socket.emplace(io_context);
    acceptor.async_accept(*socket, [&] (error_code error)
    {
        std::string members = "";
        for(auto& client : clients)
            if(!client->GetChatMember()->GetNick().empty())
                members += client->GetChatMember()->GetNick() + "; ";
        auto client = std::make_shared<Socket>(std::move(*socket));
        client->Send("Добро пожаловать!\n" +
        (members != ""? ("\nСейчас в чате: " + members + "\n") : "") + "\nВведите ваш ник: ");
        Logger::Send(tr::debug, "Client connected");
        clients.insert(client);
        client->Start
        (
            std::bind(&Server::Send, this, _1),
            [&, weak = std::weak_ptr(client)]
            {
                if(auto shared = weak.lock(); shared && clients.erase(shared)) {
                    Send("Кто-то из участников решил покинуть нас\n");
                    Logger::Send(tr::debug, "Client disconnected");
                }
            });
        AsyncAccept();
    });
}

void Server::Send(const std::string &message) {
    for(auto& client : clients)
        if(client->GetChatMember()->IsAuthorized())
            client->Send(message);
}