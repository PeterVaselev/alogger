#pragma once

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>


//TODO: подумать про вариативное использование priority_queue для различных кейсов
//Так же сделать вариант исопльзования собственных format для времени и даты
//Так же сделать дополнительное поле TAG для более информативных логов

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

    };

    
    std::ofstream _file;
    
    std::queue<Log> _queue;

    std::condition_variable cv;

    std::mutex mtx;

    std::thread worker;
    bool worker_run;

    void write_log(Log log);

    void run();


public:

    Logger();
    Logger(const std::string& file_name);

    //TODO: проверить на оптимизацию с rvalue lvalue и текущим вариантом. 
    //По предположению в данном случае компилятор сам выберет оптимальный вариант
    void logi(std::string log);
    void logw(std::string log);
    void loge(std::string log);


    ~Logger();
};
