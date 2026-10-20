#include "Time.hpp"
/// <summary>
/// Initializes a Time object with the specified hours and minutes.
/// </summary>
/// <param name="hours"></param>
/// <param name="minutes"></param>
Time::Time(unsigned int hours, unsigned int minutes) {
		if (hours >= 24 || minutes >= 60) throw std::invalid_argument("Invalid time");
		_hours = hours; _minutes = minutes;
}

/// <summary>
/// The constructor accepts a string of format HH:MM or H:MM
/// </summary>
/// <param name="time"></param>
Time::Time(const std::string& time)
{
	if (time.size() != 4 && time.size() != 5)
		throw std::invalid_argument("Invalid time format"); //format should be exactly H:HH or H:HH

	size_t colonPos = time.find(':');

	if (colonPos != time.size() - 3)
		throw std::invalid_argument("Invalid time format"); //format should be exactly H:HH or H:HH

	for (char c : time) {
		if (c == ':') continue;

		if (!std::isdigit(static_cast<unsigned char>(c)))
			throw std::invalid_argument("Invalid time format"); //input value can not contain characters but ':'
	}

	std::string hoursString = time.substr(0, colonPos);
	std::string minutesString = time.substr(colonPos+1);

	unsigned int hoursInt = std::stoi(hoursString);
	unsigned int minutesInt = std::stoi(minutesString);

	if(hoursInt >= 24 || minutesInt >= 60) 
		throw std::invalid_argument("Invalid time value");

	_hours = hoursInt; _minutes = minutesInt;
}

bool Time::operator<(const Time& other) const
{
	if (other.GetHours() > _hours) return true;
	else if (other.GetHours() == _hours) {
		return other.GetMinutes() > _minutes;
	}
	return false;
}

bool Time::operator>(const Time& other) const
{
	if (other.GetHours() < _hours) return true;
	else if (other.GetHours() == _hours) {
		return other.GetMinutes() < _minutes;
	}
	return false;
}

bool Time::operator==(const Time& other) const
{
	return other.GetHours() == _hours && other.GetMinutes() == _minutes;
}

Time& Time::operator=(const Time& other)
{
	_hours = other.GetHours(); _minutes = other.GetMinutes();
	return *this;
}

bool Time::operator<=(const Time& other) const
{
	return *this < other || *this == other;
}

bool Time::operator>=(const Time& other) const
{
	return *this > other || *this == other;
}

bool Time::operator!=(const Time& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const Time& other)
{
	out << std::setfill('0') << std::setw(2) << other.GetHours() << ":" << other.GetMinutes();
	return out;
}
