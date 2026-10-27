#include "Date.hpp"

// constructor implementation

/**
 * @brief Constructs a Date object from a given triple of uint's.
 * @param Three uint values representing day, month and year.
 * @throws std::invalid_argument If the date format or values are invalid.
 */
Date::Date(unsigned int day, unsigned int month, unsigned int year) : _year(year), _month(month), _day(day)
{
    if (day > 31 || day == 0 || month > 12 || month == 0 || year < 1)
        throw std::invalid_argument("Invalid arguments");
    else if (day > daysInMonth[month - 1])
        throw std::invalid_argument("Invalid arguments");
    else if (!isLeapYear(year) && month == 2 && day == 29)
        throw std::invalid_argument("Invalid arguments");
}

/**
 * @brief Constructs a Date object from a string in DD.MM.YYYY format.
 * @param day String representing the date.
 * @throws std::invalid_argument If the date format or values are invalid.
 */
Date::Date(const std::string& day)
{
    if (day.length() != 10 || day[2] != '.' || day[5] != '.')
        throw std::invalid_argument("Invalid arguments");

    for (char c : day)
    {
        if (c == '.')
            continue;

        if (!std::isdigit(static_cast<unsigned char>(c)))
            throw std::invalid_argument("Invalid date format");
    }

    unsigned int dayInt = std::stoi(day.substr(0, 2));
    unsigned int monthInt = std::stoi(day.substr(3, 2));
    unsigned int yearInt = std::stoi(day.substr(6, 4));

    Date temp(dayInt, monthInt, yearInt);
    _day = temp.GetDay(), _month = temp.GetMonth(), _year = temp.GetYear();
}

// operator overloading

bool Date::operator<(const Date& other) const
{
    if (_year < other.GetYear())
        return true;
    if (_year == other.GetYear() && _month < other.GetMonth())
        return true;
    if (_year == other.GetYear() && _month == other.GetMonth() && _day < other.GetDay())
        return true;
    return false;
}

bool Date::operator<=(const Date& other) const { return *this < other || *this == other; }

bool Date::operator>=(const Date& other) const { return *this > other || *this == other; }

bool Date::operator>(const Date& other) const
{
    if (_year > other.GetYear())
        return true;
    if (_year == other.GetYear() && _month > other.GetMonth())
        return true;
    if (_year == other.GetYear() && _month == other.GetMonth() && _day > other.GetDay())
        return true;
    return false;
}

bool Date::operator==(const Date& other) const
{
    if (_year == other.GetYear() && _month == other.GetMonth() && _day == other.GetDay())
        return true;
    return false;
}

bool Date::operator!=(const Date& other) const { return !(*this == other); }

std::ostream& operator<<(std::ostream& out, const Date& date)
{
    out << date.GetDay() << "." << date.GetMonth() << "." << date.GetYear();
    return out;
}

// helper function implementation

/**
 * @brief Checks whether a year is a leap year.
 * @param year Year to check.
 * @return true if the year is a leap year, false otherwise.
 */
bool isLeapYear(int year)
{
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        return true;
    return false;
}

/**
 * @brief Gets the day of the week for the stored date.
 * @return Weekday enum value representing the day of the week.
 */
Weekday Date::GetWeekday() const
{
    static constexpr int offsets[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    int year = static_cast<int>(_year);

    if (_month < 3)
        --year;

    int result = (year + year / 4 - year / 100 + year / 400 + offsets[_month - 1] + _day) % 7;

    return static_cast<Weekday>(result);
}
