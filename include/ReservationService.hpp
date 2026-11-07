#pragma once
#include "Customer.hpp"
#include "Date.hpp"
#include "Reservation.hpp"
#include "Table/Table.hpp"
#include "Time.hpp"

class ReservationService
{
   private:
    // main storage
    std::unordered_map<unsigned int, Customer> _customers;
    std::unordered_map<unsigned int, std::unique_ptr<Table>> _tables;
    std::unordered_map<unsigned int, std::unique_ptr<Reservation>> _reservations;
    std::map<unsigned int, std::vector<unsigned int>> _tablesByCapacity;

    // reservation history indexes
    std::unordered_map<unsigned int, std::vector<unsigned int>>
        _customerHistory;                                                       // vector of customer's reservation ids
    std::unordered_map<unsigned int, std::vector<unsigned int>> _tableHistory;  // vector of table's reservation ids

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
    bool CreateReservation(unsigned int customerId, unsigned int tableId, unsigned int guestCount, const Date& date,
                           const Time& startTime, const Time& endTime);
    bool CancelReservation(unsigned int reservationId);
    const Reservation* GetReservation(unsigned int reservationID) const;
    bool ChangeReservationTime(unsigned int reservationId, const Date& date, const Time& startTime,
                               const Time& endTime);
    bool DeleteReservation(unsigned int reservationId);
    bool ChangeReservationCustomer(unsigned int reservationId, unsigned int customerId);
    bool ChangeReservationTable(unsigned int reservationId, unsigned int newTableId);
    bool ChangeReservationGuestCount(unsigned int reservationId, unsigned int newGuestCount);
    std::optional<unsigned int> FindSuitableTable(unsigned int customerId, unsigned int guestCount, const Date& date,
                                                  const Time& startTime, const Time& endTime);

    // reservation history
    std::vector<unsigned int> GetCustomerHistort(unsigned int customerId) const;
    std::vector<unsigned int> GetTableHistory(unsigned int tableId) const;

    // other methods
    bool IsTableAvaible(unsigned int tableId, const Date& date, const Time& startTime, const Time& endTime,
                        std::optional<unsigned int> excludeReservationId = std::nullopt) const;
};
