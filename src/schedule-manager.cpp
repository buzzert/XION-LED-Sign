#include "schedule-manager.h"

#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>

using namespace std;

int ScheduleManager::MinutesUntilCurfew() const
{
    time_t t = time(nullptr);
    tm *now = localtime(&t);

    int hours = _curfewTime.tm_hour - now->tm_hour;
    int mins = _curfewTime.tm_min - now->tm_min;

    // return (60 * hours) + mins;
    return 1;
}

void ScheduleManager::SetCurfewTimeAsString(string timeString)
{
    istringstream ss;
    ss.str(timeString);

    ss >> get_time(&_curfewTime, "%H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse curfew string");
    }
}

tm ScheduleManager::GetCurfewTime() const
{
    return _curfewTime;
}
