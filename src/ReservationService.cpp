#include "ReservationService.hpp"

bool ReservationService::AddCustomer(const Customer& customer)
{
    auto [it, inseted] = _customers.emplace(customer.GetId(), customer);
    return inseted;
}

const Customer* ReservationService::GetCustomer(unsigned int idx) const
{
    auto it = _customers.find(idx);
    if (it == _customers.end())
        return nullptr;
    return &it->second;
}

bool ReservationService::AddTable(std::unique_ptr<Table> table)
{
    if (table == nullptr)
        return false;

    unsigned int id = table->GetId();

    auto it = _tables.try_emplace(id, std::move(table));

    return it.second;
}

const Table* ReservationService::GetTable(unsigned int idx) const
{
    auto it = _tables.find(idx);
    if (it == _tables.end())
        return nullptr;
    return it->second.get();
}

bool ReservationService::CreateReservation(unsigned int customerId, unsigned int tableId, const Date& date,
                                           const Time& startTime, const Time& endTime)
{
    auto customerIt = _customers.find(customerId);
    if (customerIt == _customers.end())
        return false;

    auto tableIt = _tables.find(tableId);
    if (tableIt == _tables.end())
        return false;

    if (!IsTableAvaible(tableId, date, startTime, endTime))
        return false;

    auto reservation = std::make_unique<Reservation>(customerId, tableId, date, startTime, endTime);

    unsigned int reservationId = reservation->GetId();

    auto it = _reservations.try_emplace(reservationId, std::move(reservation));

    if (!it.second)
        return false;

    _customerHistory[customerId].push_back(reservationId);
    _tableHistory[tableId].push_back(reservationId);
}

bool ReservationService::CancelReservation(unsigned int reservationId)
{
    auto it = _reservations.find(reservationId);
    if (it == _reservations.end())
        return false;

    Reservation& reservation = *it->second.get();

    if (reservation.GetStatus() != ReservationStatus::Confirmed)
        return false;

    reservation.SetStatus(ReservationStatus::Cancelled);

    return true;
}

const Reservation* ReservationService::GetReservation(unsigned int reservationID) const
{
    auto it = _reservations.find(reservationID);
    if (it == _reservations.end())
        return nullptr;
    return it->second.get();
}

std::vector<unsigned int> ReservationService::GetCustomerHistort(unsigned int customerId) const
{
    auto it = _customerHistory.find(customerId);
    if (it == _customerHistory.end())
        return {};
    return it->second;
}

std::vector<unsigned int> ReservationService::GetTableHistory(unsigned int tableId) const
{
    auto it = _tableHistory.find(tableId);
    if (it == _tableHistory.end())
        return {};
    return it->second;
}

bool ReservationService::IsTableAvaible(unsigned int tableId, const Date& date, const Time& startTime,
                                        const Time& endTime)
{
    auto tableIt = _tables.find(tableId);
    if (tableIt == _tables.end())
        return false;

    if (!(startTime < endTime))
        return false;

    auto historyIt = _tableHistory.find(tableId);

    if (historyIt == _tableHistory.end())
        return true;

    for (unsigned int reservationId : historyIt->second)
    {
        auto reservationIt = _reservations.find(reservationId);
        if (reservationIt == _reservations.end())
            return false;

        const Reservation& reservation = *reservationIt->second.get();

        if (reservation.GetStatus() != ReservationStatus::Confirmed)
            continue;
        if (!(reservation.GetDate() == date))
            continue;

        bool overlaps = startTime < reservation.GetEndTime() && reservation.GetStartTime() < endTime;

        if (overlaps)
            return false;
    }

    return true;
}
