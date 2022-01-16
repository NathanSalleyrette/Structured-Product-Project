#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include <iostream>

using namespace std;

class log
{
private:
    /* data */
public:
    log(/* args */);
    ~log();
    /**
     * @brief initialize the main logger
     * 
     */
    static void init();
};