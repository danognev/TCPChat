#include <gtest/gtest.h>
#include <gmock/gmock.h>

class Logger {
public:
    virtual void Start() = 0;
    virtual void Send(const u_char &,const std::string &) = 0;
};
class MockLogger : public Logger {
public:
    MOCK_METHOD(void,Start,(),(override));
    MOCK_METHOD(void,Send,(const u_char &,const std::string &), (override));
};
class LoggerTest : public ::testing::Test {
public:
    MockLogger log;
};
TEST_F(LoggerTest, InitTheLogger) {
    EXPECT_CALL(log,Start()).Times(1);
    log.Start();
}
TEST_F(LoggerTest, SendLog) {
    EXPECT_CALL(log,Send(testing::_,"Test")).Times(1);
    log.Send(1,"Test");
}