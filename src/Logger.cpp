#include "Logger.hpp"
#include <ios>
#include <string>
#include <thread>


Logger::Logger() : _file("log.txt", std::ios::app){
    if (!_file.is_open()){
        throw std::runtime_error("Failed to open file logs.txt for logs");
    }
}


Logger::Logger(const std::string& file_name) : _file(file_name, std::ios::app){
    if (!_file.is_open()){
        throw std::runtime_error("Failed to open file " + file_name + " for logs");
    }
}


auto Logger::Log::operator<=>(const Log& other){
    return this->level <=> other.level;
}


void run(){
    std::thread Writer([]() {
        
    });
}
