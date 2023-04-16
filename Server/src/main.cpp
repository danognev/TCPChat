#include <iostream>
#include <boost/asio.hpp>
#include "../include/server.h"
#include "../include/logger.h"

#define ARG_NUM 2

int main(int argc,const char **argv)
{
    if(argc < ARG_NUM) {
        std::cout << "Введите: " << argv[0] << " [порт]" << std::endl;
        return 1;
    }
    try {
        Logger::Start();
        boost::asio::io_context context;
        Server server(context, std::strtoul(argv[1],NULL,10));
        server.AsyncAccept();
        context.run();
    }
    catch(const std::exception &ex) {
        Logger::Send(tr::fatal,ex.what());
    }
	return 0;
}
