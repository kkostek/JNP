#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <random>

const int HOURS_PER_DAY = 24;
const int MINUTES_PER_HOUR = 60;
const int DAYS_IN_DECEMBER = 31;

class Price {
    
private:
    int sum;
    
public:
    Price(int sum = 0) {
        this->sum = sum;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Price& price) {
        os << price.sum << " PLN";
        return os;
    }
    
    Price operator +(Price price) {
        return Price(sum + price.sum);
    }
    
    void operator -=(Price price) {
        sum -= price.sum;
    }
    
    bool operator <(Price price) {
        return sum < price.sum;
    }
};

class Date {
    
private:
    
    int timeInMinutes;
    
public:
    
    Date(int day = 0, int hour = 0, int minute = 0) {
        timeInMinutes = (day-1) * HOURS_PER_DAY * MINUTES_PER_HOUR + hour * MINUTES_PER_HOUR + minute;
    }
    
    void operator ++() {
        ++timeInMinutes;
    }
    
    int operator -(Date& date) {
        return timeInMinutes - date.timeInMinutes;
    }
    
    bool operator <(Date&& date) {
        return timeInMinutes < date.timeInMinutes;
    }
    
    bool operator <(Date& date) {
        return timeInMinutes < date.timeInMinutes;
    }

    friend std::ostream& operator<<(std::ostream& os, Date& date);
    
};

static Date current = Date(18, 16, 0);

extern inline Date& currentDate() {
    return current;
}

static std::mt19937 generator(0);
static std::uniform_int_distribution<int> dis(0, std::numeric_limits<int>::max());

extern inline int randomNumber() {
    return dis(generator);
}

#endif /* HELPER_H */
