#include <chrono>
#include <iostream>
#include <string>

class Stopwatch
{
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;

public:
    Stopwatch()
    {
        start = std::chrono::system_clock::now();
    }

    int64_t stop()
    {
        end = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
};