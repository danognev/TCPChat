#include <gtest/gtest.h>
#include <gmock/gmock.h>


using ::testing::Exactly;

class Socket {
public: 
  virtual void Send(const std::string &message) { };
  virtual void AsyncRead() = 0;
};

class MockSocket : public Socket {
public:
  MOCK_METHOD(void, Send, (const std::string &message), (override));
  MOCK_METHOD(void, AsyncRead,(), (override));
};

TEST(SocketTest, SendDataToClient) {
  MockSocket socket;
  EXPECT_CALL(socket,Send(::testing::_)).Times(Exactly(1));
  socket.Send("test");
}
TEST(SocketTest, AsyncReads) {
  MockSocket socket;
  EXPECT_CALL(socket,AsyncRead()).Times(Exactly(10));
  for(int i = 0; i < 10; i++) {
    socket.AsyncRead();
  }
}