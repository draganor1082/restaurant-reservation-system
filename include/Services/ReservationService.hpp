#pragma once
#include "Models/Customer.hpp"
#include "Models/Date.hpp"
#include "Models/Reservation.hpp"
#include "Table/Table.hpp"
#include "Models/Time.hpp"

struct TimeInterval
{
    Time start;
    Time end;
};

struct ReservationSuggestion
{
    unsigned int tableId;
    Date date;
    Time startTime;
    Time endTime;
};

class ReservationService
{
   private:
    // main storage
    std::unordered_map<unsigned int, Customer> _customers;
    std::unordered_map<unsigned int, std::unique_ptr<Table>> _tables;
    std::unordered_map<unsigned int, std::unique_ptr<Reservation>> _reservations;

    std::map<unsigned int, std::vector<unsigned int>> _tablesByCapacity;

    std::unordered_map<unsigned int, std::map<Date, std::vector<unsigned int>>> _tableSchedule;

    // reservation history indexes
    std::unordered_map<unsigned int, std::vector<unsigned int>>
        _customerHistory;                                                       // vector of customer's reservation ids
    std::unordered_map<unsigned int, std::vector<unsigned int>> _tableHistory;  // vector of table's reservation ids

    bool CanCustomerUseTable(const Customer& customer, const Table& table) const;

   public:
    // custoner manegment
    bool AddCustomer(const Customer& customer);
    const Customer* GetCustomer(unsigned int idx) const;
    bool DeleteCustomer(unsigned int customerId);

    // table manegment
    bool AddTable(std::unique_ptr<Table> table);
    const Table* GetTable(unsigned int idx) const;
    bool DeleteTable(unsigned int tableId);

    // reservation manegment
    std::optional<unsigned int> CreateReservation(unsigned int customerId, unsigned int tableId,
                                                  unsigned int guestCount, const Date& date, const Time& startTime,
                                                  const Time& endTime);
    bool CancelReservation(unsigned int reservationId);
    const Reservation* GetReservation(unsigned int reservationID) const;
    bool ChangeReservationTime(unsigned int reservationId, const Date& date, const Time& startTime,
                               const Time& endTime);
    bool DeleteReservation(unsigned int reservationId);
    bool ChangeReservationCustomer(unsigned int reservationId, unsigned int customerId);
    bool ChangeReservationTable(unsigned int reservationId, unsigned int newTableId);
    bool ChangeReservationGuestCount(unsigned int reservationId, unsigned int newGuestCount);
    std::optional<unsigned int> FindSuitableTable(unsigned int customerId, unsigned int guestCount, const Date& date,
                                                  const Time& startTime, const Time& endTime) const;
    std::optional<unsigned int> CreateReservationAuto(unsigned int customerId, unsigned int guestCount,
                                                      const Date& date, const Time& startTime, const Time& endTime);
    std::optional<ReservationSuggestion> FindBestReservation(unsigned int customerId, unsigned int guestCount,
                                                             const Date& date, const Time& startTime,
                                                             const Time& endTime) const;

    // reservation history
    std::vector<unsigned int> GetCustomerHistort(unsigned int customerId) const;
    std::vector<unsigned int> GetTableHistory(unsigned int tableId) const;

    // other methods
    bool IsTableAvaible(unsigned int tableId, const Date& date, const Time& startTime, const Time& endTime,
                        std::optional<unsigned int> excludeReservationId = std::nullopt) const;
    std::vector<TimeInterval> FindFreeIntervals(unsigned int tableId, const Date& date) const;
};
