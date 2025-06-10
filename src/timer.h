#pragma once

#include <chrono> 

// simply for timing the raytracer 
// courtesy of learncpp.com (https://www.learncpp.com/cpp-tutorial/timing-your-code/)
class Timer
{
private:
    using clock = std::chrono::steady_clock;
    using second = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<clock> m_beg{ clock::now() };

public:
    void reset()
    {
        m_beg = clock::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<second>(clock::now() - m_beg).count();
    }
};