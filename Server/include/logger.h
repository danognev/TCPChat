#ifndef LOGGER_H
#define LOGGER_H

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace tr = boost::log::v2_mt_posix::trivial;

class Logger {
private:
    static const u_int8_t rotation_size = 2;
    static const u_int8_t max_size_mb = 10;
    static const char* log_name;
public: 
    static void Start();
    static void Send(const tr::severity_level&, const char*);
};

#endif