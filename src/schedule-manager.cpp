#include "schedule-manager.h"

#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <time.h>

using namespace std;

int ScheduleManager::MinutesUntilCurfew() const
{
    time_t t = time(nullptr);
    tm *now = localtime(&t);

    int hours = _curfewTime.tm_hour - now->tm_hour;
    int mins = _curfewTime.tm_min - now->tm_min;

    return (60 * hours) + mins;
}

void ScheduleManager::SetCurfewTimeAsString(string timeString)
{
    strptime(timeString.c_str(), "%H:%M:%S", &_curfewTime);
}

tm ScheduleManager::GetCurfewTime() const
{
    return _curfewTime;
}
