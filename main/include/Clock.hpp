#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <string>

class Clock {
public:
    static void init();                  // Setup RTC & sync SNTP
    static std::string get_time_str();   // Return HH:MM:SS formatted time
};

#endif