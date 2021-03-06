#include "src/Logger.hpp"
#include <sstream>
#include <thread>
#include <vector>

void logSomeMessage(int id, Logger& logger)
{
    for (int i = 0; i < 10; ++i) {
        std::stringstream ss;
        ss << "Log entry " << i << " from thread " << id;
        logger.log(ss.str());
    }
}

int main()
{
    Logger logger;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(logSomeMessage, i, std::ref(logger));
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}