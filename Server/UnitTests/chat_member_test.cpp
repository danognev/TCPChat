#include <gtest/gtest.h>
#include "../include/chat_member.h"

class ChatMemberTest : public ::testing::Test {
protected:
  ChatMember member;
};

TEST_F(ChatMemberTest, ChatMemberShouldBeNotAuthorized) {
  EXPECT_FALSE(member.IsAuthorized());
}

TEST_F(ChatMemberTest, ChatMemberSetAuthorizedAndGetNick) {
  std::string nick = "Test";
  member.AuthMember(nick);
  EXPECT_EQ(member.GetNick(),nick);
}
