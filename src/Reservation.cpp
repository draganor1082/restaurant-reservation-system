#include "Reservation.hpp"

Reservation::Reservation(unsigned int customerId, unsigned int tableId, const Date& date, const Time& startTime,
                         const Time& endTime, ReservationStatus status)
    : _customerId(customerId), _tableId(tableId), _date(date), _startTime(startTime), _endTime(endTime), _status(status)
{
    if (startTime >= endTime)
        throw std::invalid_argument("End time must be later than start time");
    else if (customerId == 0 || tableId == 0)
        throw std::invalid_argument("Invalid arguments");
    _id = ++_nextID;
}

void Reservation::SetTime(const Time& startTime, const Time& endTime)
{
    if (startTime >= endTime)
        throw std::invalid_argument("End time must be later than start time");
    else
    {
        _startTime = startTime;
        _endTime = endTime;
    }
}

void Reservation::SetCustomerId(unsigned int customerId)
{
    if (customerId == 0)
        throw std::invalid_argument("Customer id can not equal to 0");
    _customerId = customerId;
}

void Reservation::SetTableId(unsigned int tableId)
{
    if (tableId == 0)
        throw std::invalid_argument("Table id can not equal to 0");
    _tableId = tableId;
}

bool Reservation::OverlapsWith(const Reservation& other) const
{
    if (_date != other.GetDate())
        return false;
    if (other.GetStatus() == ReservationStatus::Cancelled || _status == ReservationStatus::Cancelled)
        return false;

    return _startTime < other.GetEndTime() && other.GetStartTime() < _endTime;
}

bool Reservation::OverlapsWith(const Reservation& r1, const Reservation& r2) { return r1.OverlapsWith(r2); }

unsigned int Reservation::GetDuration() const
{
    unsigned int start = _startTime.GetHours() * 60 + _startTime.GetMinutes();
    unsigned int end = _endTime.GetHours() * 60 + _endTime.GetMinutes();
    return end - start;
}

unsigned int Reservation::GetDuration(const Reservation& other) { return other.GetDuration(); }

std::ostream& operator<<(std::ostream& out, const Reservation& r)
{
    std::string status = "";
    switch (r.GetStatus())
    {
        case ReservationStatus::Confirmed:
            status = "Confirmed";
            break;
        case ReservationStatus::Completed:
            status = "Completed";
            break;
        case ReservationStatus::Cancelled:
            status = "Cancelled";
            break;
    }

    out << "Reservation #" << r.GetId() << " [" << status << "]\n";
    out << r.GetDate() << " | " << r.GetStartTime() << " - " << r.GetEndTime() << std::endl;
    out << "Table #" << r.GetTableId() << " | Customer #" << r.GetCustomerId() << " | " << r.GetDuration() << " min";

    return out;
}
