#include "clock-screen.h"

#include "graphics.h"

//#include <ctime>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

using namespace rgb_matrix;
using namespace std;

static Font __clockFont;
static bool __didLoadFont;
static Font& _getClockFont()
{
    if (!__didLoadFont) {
        const char *fontPath = RESOURCES_DIR "/fonts/8x13B.bdf";
        __didLoadFont = __clockFont.LoadFont(fontPath);
    }

    return __clockFont;
}

ClockScreen::ClockScreen(Utils::Size size)
    : TickerScreen(size)
{
}

void ClockScreen::_UpdateTimeString()
{
    const size_t kBufferSize = 16;
    time_t now;
    struct tm *timeinfo;
    char buffer[kBufferSize];

    time(&now);
    timeinfo = localtime(&now);

    strftime(buffer, kBufferSize, "%H:%M:%S", timeinfo);
    _timeString = buffer;
}

static inline void _clock_get_monotonic_time(struct timespec *ts)
{
    #ifdef __MACH__
    // For OS X
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
    #else
    // For Linux
    clock_gettime(CLOCK_REALTIME, ts);
    #endif
}

void ClockScreen::Update(double timeDelta)
{
    _UpdateTimeString();
}

void ClockScreen::Draw(MatrixFrame *nextFrame)
{
    Font &font = _getClockFont();
    DrawText(nextFrame, font, 0, font.baseline() + 2, Color(0xFF, 0x00, 0x00), _timeString.c_str());

    struct timespec te = { 0, 0 };
    _clock_get_monotonic_time(&te);

    time_t seconds = te.tv_sec;
    long nanoseconds = fmod(te.tv_nsec, 1e+9);

    float perc = (nanoseconds / 1e+9);
    int pos = (perc * nextFrame->width());
    nextFrame->SetPixel(pos, nextFrame->height() - 1, 0xFF, 0x00, 0x00);
}
