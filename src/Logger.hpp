#pragma once


#include <fstream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

class Logger {
private:
    enum class LogLevel : int {
        INFO,
        WARNING,
        ERROR
    };

    struct Log {
        LogLevel level;
        std::string log;

        auto operator<=> (const Log& other);
    };


    std::ofstream _file;
    
    std::priority_queue<Log> _queue;

    std::mutex mtx;

public:

    Logger();
    Logger(const std::string& file_name);

    void run();

    void logi(std::string log);
    void logw(std::string log);
    void loge(std::string log);


    ~Logger();
};
