#include "spdlog/log.hpp"

log::log(){}

void log::init(){
    try 
    {
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_st>("logs/logfile.txt", 1024 * 1024 * 5, 3));
        auto combined_logger = std::make_shared<spdlog::logger>("MainLogs", begin(sinks), end(sinks));
        //register it if you need to access it globally
        spdlog::register_logger(combined_logger);
        spdlog::set_pattern("[%Y:%m:%d] [%H:%M:%S] [%l] [%s:%#] [%!] %v");
        SPDLOG_LOGGER_INFO(combined_logger, "Log initialization");

    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}