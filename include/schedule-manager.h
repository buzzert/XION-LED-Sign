#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include <ctime>
#include <string>

#define DDR_CURFEW_TIME "22:00:00"

class ScheduleManager
{
public:
    // Curfew
    int MinutesUntilCurfew() const;

    void SetCurfewTimeAsString(std::string timeString);
    std::tm GetCurfewTime() const;

private:
    std::tm _curfewTime;
};

#endif
