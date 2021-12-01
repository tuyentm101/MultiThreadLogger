#pragma once

#include <string>
#include <memory>

class Logger
{
        struct Impl;
        std::unique_ptr<Impl> m_pImpl;
        
    public:
        Logger();
        ~Logger();
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        void log(const std::string &entry);
};