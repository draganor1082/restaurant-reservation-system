#include "ReservationService.hpp"

bool ReservationService::CanCustomerUseTable(const Customer& customer, const Table& table) const
{
    if (!customer.IsVIP() && table.IsVip())
        return false;

    return true;
}

bool ReservationService::AddCustomer(const Customer& customer)
{
    auto [it, inserted] = _customers.emplace(customer.GetId(), customer);

    if (!inserted)
        return false;

    _customerHistory.try_emplace(customer.GetId());

    return true;
}

const Customer* ReservationService::GetCustomer(unsigned int idx) const
{
    auto it = _customers.find(idx);
    if (it == _customers.end())
        return nullptr;
    return &it->second;
}

bool ReservationService::DeleteCustomer(unsigned int customerId)
{
    auto customerIt = _customers.find(customerId);
    if (customerIt == _customers.end())
        return false;

    auto historyIt = _customerHistory.find(customerId);

    if (historyIt != _customerHistory.end())
    {
        for (unsigned int reservationId : historyIt->second)
        {
            DeleteReservation(reservationId);
        }

        _customerHistory.erase(historyIt);
    }

    _customers.erase(customerIt);

    return true;
}

bool ReservationService::AddTable(std::unique_ptr<Table> table)
{
    if (!table)
        return false;

    unsigned int tableId = table->GetId();
    unsigned int capacity = table->GetCapacity();

    if (_tables.contains(tableId))
        return false;

    auto [it, inserted] = _tables.emplace(tableId, std::move(table));

    if (!inserted)
        return false;

    _tableHistory.try_emplace(tableId);
    _tablesByCapacity[capacity].push_back(tableId);

    return true;
}

const Table* ReservationService::GetTable(unsigned int idx) const
{
    auto it = _tables.find(idx);
    if (it == _tables.end())
        return nullptr;
    return it->second.get();
}

bool ReservationService::DeleteTable(unsigned int tableId)
{
    auto tableIt = _tables.find(tableId);
    if (tableIt == _tables.end())
        return false;

    auto historyIt = _tableHistory.find(tableId);
    if (historyIt != _tableHistory.end())
    {
        auto reservationIds = historyIt->second;

        for (unsigned int reservationId : reservationIds)
        {
            if (!DeleteReservation(reservationId))
                return false;
        }
    }

    unsigned int capacity = tableIt->second->GetCapacity();

    auto capacityIt = _tablesByCapacity.find(capacity);

    if (capacityIt != _tablesByCapacity.end())
    {
        std::erase(capacityIt->second, tableId);

        if (capacityIt->second.empty())
            _tablesByCapacity.erase(capacityIt);
    }

    _tableHistory.erase(tableId);
    _tables.erase(tableIt);

    return true;
}

bool ReservationService::CreateReservation(unsigned int customerId, unsigned int tableId, unsigned int guestCount,
                                           const Date& date, const Time& startTime, const Time& endTime)
{
    auto customerIt = _customers.find(customerId);
    if (customerIt == _customers.end())
        return false;

    auto tableIt = _tables.find(tableId);
    if (tableIt == _tables.end())
        return false;

    if (!CanCustomerUseTable(customerIt->second, *tableIt->second))
        return false;

    if (!tableIt->second->CanAccommodate(guestCount))
        return false;

    if (!IsTableAvaible(tableId, date, startTime, endTime))
        return false;

    auto reservation = std::make_unique<Reservation>(customerId, tableId, guestCount, date, startTime, endTime);

    unsigned int reservationId = reservation->GetId();

    auto it = _reservations.try_emplace(reservationId, std::move(reservation));

    if (!it.second)
        return false;

    _customerHistory[customerId].push_back(reservationId);
    _tableHistory[tableId].push_back(reservationId);

    return true;
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

bool ReservationService::ChangeReservationTime(unsigned int reservationId, const Date& date, const Time& startTime,
                                               const Time& endTime)
{
    if (!(startTime < endTime))
        return false;

    auto it = _reservations.find(reservationId);
    if (it == _reservations.end())
        return false;

    Reservation& current = *it->second;

    if (!(current.GetStatus() == ReservationStatus::Confirmed))
        return false;

    if (!IsTableAvaible(current.GetTableId(), date, startTime, endTime, reservationId))
        return false;

    current.SetDate(date);
    current.SetTime(startTime, endTime);
    return true;
}

bool ReservationService::DeleteReservation(unsigned int reservationId)
{
    auto reservationIt = _reservations.find(reservationId);
    if (reservationIt == _reservations.end())
        return false;

    unsigned int customerId = reservationIt->second->GetCustomerId();
    unsigned int tableId = reservationIt->second->GetTableId();

    auto customerHistoryIt = _customerHistory.find(customerId);
    if (customerHistoryIt != _customerHistory.end())
    {
        std::erase(customerHistoryIt->second, reservationId);
    }

    auto tableHistoryIt = _tableHistory.find(tableId);
    if (tableHistoryIt != _tableHistory.end())
    {
        std::erase(tableHistoryIt->second, reservationId);
    }

    _reservations.erase(reservationIt);

    return true;
}

bool ReservationService::ChangeReservationCustomer(unsigned int reservationId, unsigned int newCustomerId)
{
    auto newCustomerIt = _customers.find(newCustomerId);
    if (newCustomerIt == _customers.end())
        return false;

    auto reservationIt = _reservations.find(reservationId);
    if (reservationIt == _reservations.end())
        return false;

    if (reservationIt->second->GetStatus() != ReservationStatus::Confirmed)
        return false;

    auto tableIt = _tables.find(reservationIt->second->GetTableId());
    if (tableIt == _tables.end())
        return false;

    if (!CanCustomerUseTable(newCustomerIt->second, *tableIt->second))
        return false;

    if (reservationIt->second->GetCustomerId() == newCustomerId)
        return true;

    auto oldCustomerHistoryIt = _customerHistory.find(reservationIt->second->GetCustomerId());
    if (oldCustomerHistoryIt == _customerHistory.end())
        return false;

    auto newCustomerHistoryIt = _customerHistory.find(newCustomerId);
    if (newCustomerHistoryIt == _customerHistory.end())
        return false;

    newCustomerHistoryIt->second.push_back(reservationId);

    std::erase(oldCustomerHistoryIt->second, reservationId);

    reservationIt->second->SetCustomerId(newCustomerIt->second.GetId());

    return true;
}

bool ReservationService::ChangeReservationTable(unsigned int reservationId, unsigned int newTableId)
{
    auto reservationIt = _reservations.find(reservationId);
    if (reservationIt == _reservations.end())
        return false;

    Reservation& reservation = *reservationIt->second;

    if (reservation.GetStatus() != ReservationStatus::Confirmed)
        return false;

    auto newTableIt = _tables.find(newTableId);
    if (newTableIt == _tables.end())
        return false;

    auto customerIt = _customers.find(reservationIt->second->GetCustomerId());
    if (customerIt == _customers.end())
        return false;

    if (!CanCustomerUseTable(customerIt->second, *newTableIt->second))
        return false;

    auto oldTableId = reservation.GetTableId();

    auto oldTableIt = _tables.find(oldTableId);
    if (oldTableIt == _tables.end())
        return false;

    if (oldTableId == newTableId)
        return true;

    if (!newTableIt->second->CanAccommodate(reservation.GetGuestCount()))
        return false;

    auto oldTableHistoryIt = _tableHistory.find(oldTableId);
    if (oldTableHistoryIt == _tableHistory.end())
        return false;

    auto newTableHistoryIt = _tableHistory.find(newTableId);
    if (newTableHistoryIt == _tableHistory.end())
        return false;

    auto& oldHistory = oldTableHistoryIt->second;
    auto& newHistory = newTableHistoryIt->second;

    bool isNewTableAvaible =
        IsTableAvaible(newTableId, reservation.GetDate(), reservation.GetStartTime(), reservation.GetEndTime());

    if (!isNewTableAvaible)
        return false;

    newHistory.push_back(reservationId);
    std::erase(oldHistory, reservationId);

    reservation.SetTableId(newTableId);

    return true;
}

bool ReservationService::ChangeReservationGuestCount(unsigned int reservationId, unsigned int newGuestCount)
{
    auto reservationIt = _reservations.find(reservationId);
    if (reservationIt == _reservations.end())
        return false;

    Reservation& reservation = *reservationIt->second;

    if (reservation.GetStatus() != ReservationStatus::Confirmed)
        return false;

    if (newGuestCount == 0)
        return false;

    auto tableIt = _tables.find(reservation.GetTableId());
    if (tableIt == _tables.end())
        return false;

    if (newGuestCount > tableIt->second->GetCapacity())
        return false;

    if (reservation.GetGuestCount() == newGuestCount)
        return true;

    reservation.SetGuestCount(newGuestCount);

    return true;
}

std::optional<unsigned int> ReservationService::FindSuitableTable(unsigned int customerId, unsigned int guestCount,
                                                                  const Date& date, const Time& startTime,
                                                                  const Time& endTime) const
{
    if (guestCount == 0)
        return std::nullopt;

    auto customerIt = _customers.find(customerId);
    if (customerIt == _customers.end())
        return std::nullopt;

    bool isCustomerVip = customerIt->second.IsVIP();

    auto it = _tablesByCapacity.lower_bound(guestCount);

    for (; it != _tablesByCapacity.end(); ++it)
    {
        for (unsigned int tableId : it->second)
        {
            auto tableIt = _tables.find(tableId);

            if (tableIt == _tables.end())
                continue;

            if (!IsTableAvaible(tableId, date, startTime, endTime) || !CanCustomerUseTable(customerIt->second, *tableIt->second))
                continue;

            return tableId;
        }
    }

    return std::nullopt;
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
                                        const Time& endTime, std::optional<unsigned int> excludeReservationId) const
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
        if (excludeReservationId.has_value() && excludeReservationId == reservationId)
            continue;

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
