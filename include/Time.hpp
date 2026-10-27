#pragma once

class Time
{
   private:
    unsigned int _hours;
    unsigned int _minutes;

   public:
    Time(unsigned int hours = 0, unsigned int minutes = 0);
    Time(const std::string& time);

    unsigned int GetHours() const { return _hours; }
    unsigned int GetMinutes() const { return _minutes; }

    bool operator<(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator==(const Time& other) const;
    Time& operator=(const Time& other);

    bool operator<=(const Time& other) const;
    bool operator>=(const Time& other) const;
    bool operator!=(const Time& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Time& other);
};
