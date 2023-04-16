#ifndef CHAT_MEMBER_H
#define CHAT_MEMBER_H

#include <iostream>
#include <map>

class ChatMember {
private: 
    bool is_auth = false;
    std::string nick;
public:
    bool IsAuthorized();
    void AuthMember(const std::string &);
    const std::string& GetNick();
};

#endif