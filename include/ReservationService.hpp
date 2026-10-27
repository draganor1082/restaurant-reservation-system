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

    // reservation history indexes
    std::unordered_map<unsigned int, std::vector<unsigned int>> _customerHistory;
    std::unordered_map<unsigned int, std::vector<unsigned int>> _tableHistory;

   public:
    // custoner manegment
    bool AddCustomer(const Customer& customer);
    const Customer* GetCustomer(unsigned int idx) const;

    // table manegment
    bool AddTable(std::unique_ptr<Table> table);
    const Table* GetTable(unsigned int idx) const;

    // reservation manegment
    bool CreateReservation(unsigned int customerId, unsigned int tableId, const Date& date, const Time& startTime,
                           const Time& endTime);
    bool CancelReservation(unsigned int reservationId);
    const Reservation* GetReservation(unsigned int reservationID) const;

    // reservation history
    std::vector<unsigned int> GetCustomerHistort(unsigned int customerId) const;
    std::vector<unsigned int> GetTableHistory(unsigned int tableId) const;

    // other methods
    bool IsTableAvaible(unsigned int tableId, const Date& date, const Time& startTime, const Time& endTime);
};
