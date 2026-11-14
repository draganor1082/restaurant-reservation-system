#pragma once

enum class Weekday
{
    Sunday = 0,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

class Date
{
   private:
    unsigned int _day;
    unsigned int _month;
    unsigned int _year;

   public:
    Date(unsigned int day, unsigned int month, unsigned int year);
    Date(const std::string& day);

    unsigned int GetDay() const { return _day; }
    unsigned int GetMonth() const { return _month; }
    unsigned int GetYear() const { return _year; }
    Weekday GetWeekday() const;

    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Date& date);
};

const int daysInMonth[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool isLeapYear(int year);
