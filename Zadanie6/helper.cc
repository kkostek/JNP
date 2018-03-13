#include "helper.h"

std::ostream& operator<<(std::ostream& os, Date& date) {
    int day = date.timeInMinutes / (HOURS_PER_DAY * MINUTES_PER_HOUR) + 1;
    int hour = ((date.timeInMinutes % (HOURS_PER_DAY * MINUTES_PER_HOUR))) / MINUTES_PER_HOUR;
    int minute = date.timeInMinutes % MINUTES_PER_HOUR;
    os << "December, " << day << ", " << hour << ":";

    if (minute < 10) {
        os << 0;
    }
    os << minute;
    return os;
}

