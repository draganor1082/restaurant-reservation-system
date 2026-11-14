#include "Date.hpp"
#include "Time.hpp"
#pragma once

enum class ReservationStatus
{
    Confirmed,
    Cancelled,
    Completed
};

class Reservation
{
   private:
    inline static unsigned int _nextID = 0;

    unsigned int _id;
    unsigned int _customerId;
    unsigned int _tableId;
    unsigned int _guestCount;

    ReservationStatus _status;

    Date _date;
    Time _startTime;
    Time _endTime;

   public:
    Reservation(unsigned int customerId, unsigned int tableId, unsigned int guestCount, const Date& date,
                const Time& startTime, const Time& endTime, ReservationStatus status = ReservationStatus::Confirmed);

    unsigned int GetId() const { return _id; }
    unsigned int GetCustomerId() const { return _customerId; }
    unsigned int GetTableId() const { return _tableId; }
    unsigned int GetGuestCount() const { return _guestCount; }

    const Date& GetDate() const { return _date; }
    const Time& GetStartTime() const { return _startTime; }
    const Time& GetEndTime() const { return _endTime; }
    const ReservationStatus GetStatus() const { return _status; }

    void SetDate(const Date& date) { _date = date; }
    void SetTime(const Time& startTime, const Time& endTime);
    void SetCustomerId(unsigned int customerId);
    void SetTableId(unsigned int tableId);
    void SetStatus(ReservationStatus status) { _status = status; }
    void SetGuestCount(unsigned int guestCount);

    bool OverlapsWith(const Reservation& other) const;
    static bool OverlapsWith(const Reservation& r1, const Reservation& r2);

    unsigned int GetDuration() const;
    static unsigned int GetDuration(const Reservation& other);

    Reservation& operator=(const Reservation& other) = delete;
    Reservation(const Reservation&) = delete;

    friend std::ostream& operator<<(std::ostream& out, const Reservation& r);
};
