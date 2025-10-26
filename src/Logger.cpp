#include "Logger.hpp"
#include <ios>
#include <stdexcept>
#include <string>
#include <thread>
#include <mutex>
#include <format>
#include <chrono>

Logger::Logger() : _file("log.txt", std::ios::app){
    if (!_file.is_open()){
        throw std::runtime_error("Failed to open file logs.txt for logs");
    }

    this->run();
}


Logger::Logger(const std::string& file_name) : _file(file_name, std::ios::app){
    if (!_file.is_open()){
        throw std::runtime_error("Failed to open file " + file_name + " for logs");
    }
    this->run();
}

void Logger::write_log(Log log){
    const auto now = std::chrono::system_clock::now(); 
    std::string formatted_time = std::format("{:%d.%m.%Y %H:%M:%S}", now);
    
    switch(log.level){
        case Logger::LogLevel::ERROR:
            this->_file << formatted_time << "\tERROR\t" << log.log << std::endl;
            break;

        case Logger::LogLevel::WARNING:
            this->_file << formatted_time << "\tWARN\t" << log.log << std::endl;
            break;

        case Logger::LogLevel::INFO:
            this->_file << formatted_time << "\tINFO\t" << log.log << std::endl;
            break;

        default:
            this->_file << formatted_time << "\tNONE\t" << log.log << std::endl;
            break;
    }
}


//TODO: Здесь может быть гонка данных, необходимо переписать так, чтобы все последние данные успели обработаться
void Logger::run(){
    worker_run = true;

    this->worker = std::thread([this]() {
        
        std::unique_lock<std::mutex> lock (this->mtx);
        while (this->worker_run){
            cv.wait(lock, [this](){
                return !this->_queue.empty() or !worker_run;
            });

            if (!this->_queue.empty()){
                std::queue<Logger::Log> local_queue;
                this->_queue.swap(local_queue);
                lock.unlock();

                while (!local_queue.empty()){
                    auto log = local_queue.front();
                    local_queue.pop();
                    this->write_log(log);
                }

            }
            lock.lock();

        }
        if (!this->_queue.empty()){
            std::queue<Logger::Log> local_queue;
            this->_queue.swap(local_queue);
            lock.unlock();
            while(!local_queue.empty()){
                this->write_log(local_queue.front());
                local_queue.pop();
            }
        }
    });


}

Logger::~Logger(){
    worker_run = false;
    cv.notify_one();
    worker.join();
}
