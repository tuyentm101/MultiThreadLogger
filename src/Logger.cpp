#include "Logger.hpp"
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <fstream>
#include <iostream>

struct Logger::Impl
{
    Impl()
    {
        m_Thread = std::thread{&Impl::processEntries, this};
    }
    ~Impl()
    {
        {
            std::unique_lock<std::mutex> locker(m_Mutex);
            m_Exit = true;
            m_Cond.notify_all();
        }
        m_Thread.join();
    }
    void processEntries()
    {
        std::ofstream logFile("log.txt");
        if (logFile.fail()){
            std::cerr << "Fail to open log file. \n" << std::endl;
            return;
        }

        std::unique_lock<std::mutex> locker(m_Mutex);
        while (true) {
            if (!m_Exit){
                m_Cond.wait(locker);
            }
            locker.unlock();
            while (true){
                locker.lock();
                if (m_Queue.empty()) {
                    break;
                } else {
                    logFile << m_Queue.front() << std::endl;
                    m_Queue.pop();
                }
                locker.unlock();
            }
            if (m_Exit) break;
        }
    }
    void log(const std::string& entry)
    {
        std::unique_lock<std::mutex> locker(m_Mutex);
        m_Queue.push(entry);
        m_Cond.notify_all();
    }
    std::mutex m_Mutex;
    std::condition_variable m_Cond;
    std::queue<std::string> m_Queue;
    std::thread m_Thread;
    bool m_Exit = false;
};

Logger::Logger()
{
    m_pImpl = std::make_unique<Impl>();
}

Logger::~Logger() = default;

void Logger::log(const std::string& entry)
{
    m_pImpl->log(entry);
}
