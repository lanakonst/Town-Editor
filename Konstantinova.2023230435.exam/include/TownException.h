#pragma once
#include <exception>
#include <string>

using namespace std;

class TownException : public std::exception {
    std::string message;
public:
    explicit TownException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};
