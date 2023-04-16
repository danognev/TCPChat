#include "../include/logger.h"

const char* Logger::log_name = "./logs/server.log";

void Logger::Start() {
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");
    logging::add_file_log(
        keywords::file_name = log_name,
        keywords::auto_flush = true,
        keywords::rotation_size = rotation_size * 1024 * 1024,
        keywords::max_size = max_size_mb * 1024 * 1024,
        keywords::open_mode = std::ios_base::app,
        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] [%ProcessID%] [%LineID%] %Message%"
    );
    logging::add_common_attributes();
}

void Logger::Send(const tr::severity_level &level, const char *message) {
    switch(level) {
        case tr::info: {
            BOOST_LOG_TRIVIAL(info) << message;
            break;
        }
        case tr::debug: {
            BOOST_LOG_TRIVIAL(debug) << message;
            break;
        }
        case tr::error: {
            BOOST_LOG_TRIVIAL(error) << message;
            break;
        }
        case tr::warning: {
            BOOST_LOG_TRIVIAL(warning) << message;
            break;
        }
        case tr::fatal: {
            BOOST_LOG_TRIVIAL(fatal) << message;
            break;
        }
    }
}