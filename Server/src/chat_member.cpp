#include "../include/chat_member.h"

bool ChatMember::IsAuthorized() { return is_auth; }

void ChatMember::AuthMember(const std::string &nick) {
    this->nick = nick;
    is_auth = true;
}
const std::string& ChatMember::GetNick() { return nick; }