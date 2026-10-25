#include "Date.hpp"
#include "Time.hpp"
#pragma once

enum class ReservationStatus {
	Confirmed,
	Cancelled,
	Completed
};

class Reservation {
private:
	inline static unsigned int _nextID = 0;

	unsigned int _id;
	unsigned int _customerId;
	unsigned int _tableId;

	ReservationStatus _status;

	Date _date;
	Time _startTime;
	Time _endTime;

public:
	Reservation(unsigned int customerId, unsigned int tableId, Date& date, Time& startTime, 
		Time& endTime, ReservationStatus status = ReservationStatus::Confirmed);

	unsigned int GetId() const { return _id; }
	unsigned int GetCustomerId() const { return _customerId; }
	unsigned int GetTableId() const { return _tableId; }

	const Date& GetDate() const { return _date; }
	const Time& GetStartTime() const { return _startTime; }
	const Time& GetEndTime() const { return _endTime; }
	const ReservationStatus GetStatus() const { return _status; }

	void SetDate(const Date& date) { _date = date; }
	void SetTime(const Time& startTime, const Time& endTime);
	void SetCustomerId(unsigned int customerId);
	void SetTableId(unsigned int tableId);

	bool OverlapsWith(const Reservation& other) const;
	static bool OverlapsWith(const Reservation& r1, const Reservation& r2);

	unsigned int GetDuration() const;
	static unsigned int GetDuration(const Reservation& other);

	Reservation& operator=(const Reservation& other) = delete;
	Reservation(const Reservation& other) = delete;

	friend std::ostream& operator<<(std::ostream& out, const Reservation& r);
};