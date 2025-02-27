#include <iostream>
#include <chrono>

class Stopwatch {
private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    
public:
    Stopwatch() {
        startTime = std::chrono::steady_clock::now();
    }

    int GetElapsedTime() {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    }
};

