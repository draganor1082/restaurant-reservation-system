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
        auto reservationIds = historyIt->second;

        for (unsigned int reservationId : reservationIds)
        {
            if (!DeleteReservation(reservationId))
                return false;
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
    _tableSchedule.try_emplace(tableId);
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
    _tableSchedule.erase(tableId);

    return true;
}

std::optional<unsigned int> ReservationService::CreateReservation(unsigned int customerId, unsigned int tableId,
                                                                  unsigned int guestCount, const Date& date,
                                                                  const Time& startTime, const Time& endTime)
{
    auto customerIt = _customers.find(customerId);
    if (customerIt == _customers.end())
        return std::nullopt;

    auto tableIt = _tables.find(tableId);
    if (tableIt == _tables.end())
        return std::nullopt;

    if (!CanCustomerUseTable(customerIt->second, *tableIt->second))
        return std::nullopt;

    if (!tableIt->second->CanAccommodate(guestCount))
        return std::nullopt;

    if (!IsTableAvaible(tableId, date, startTime, endTime))
        return std::nullopt;

    auto reservation = std::make_unique<Reservation>(customerId, tableId, guestCount, date, startTime, endTime);

    unsigned int reservationId = reservation->GetId();

    auto it = _reservations.try_emplace(reservationId, std::move(reservation));

    if (!it.second)
        return std::nullopt;

    _customerHistory[customerId].push_back(reservationId);
    _tableHistory[tableId].push_back(reservationId);
    _tableSchedule[tableId][date].push_back(reservationId);

    return reservationId;
}

bool ReservationService::CancelReservation(unsigned int reservationId)
{
    auto it = _reservations.find(reservationId);
    if (it == _reservations.end())
        return false;

    Reservation& reservation = *it->second;

    if (reservation.GetStatus() != ReservationStatus::Confirmed)
        return false;

    unsigned int tableId = reservation.GetTableId();
    const Date& date = reservation.GetDate();

    auto scheduleIt = _tableSchedule.find(tableId);
    if (scheduleIt == _tableSchedule.end())
        return false;

    auto& schedule = scheduleIt->second;

    auto dateIt = schedule.find(date);
    if (dateIt == schedule.end())
        return false;

    auto& reservationIds = dateIt->second;

    if (std::find(reservationIds.begin(), reservationIds.end(), reservationId) == reservationIds.end())
        return false;

    std::erase(reservationIds, reservationId);

    if (reservationIds.empty())
        schedule.erase(dateIt);

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

    Reservation& reservation = *it->second;

    if (!(reservation.GetStatus() == ReservationStatus::Confirmed))
        return false;

    unsigned int tableId = reservation.GetTableId();
    const Date& oldDate = reservation.GetDate();

    if (!IsTableAvaible(reservation.GetTableId(), date, startTime, endTime, reservationId))
        return false;

    auto scheduleIt = _tableSchedule.find(tableId);
    if (scheduleIt == _tableSchedule.end())
        return false;

    auto& schedule = scheduleIt->second;

    auto oldDateIt = schedule.find(oldDate);
    if (oldDateIt == schedule.end())
        return false;

    auto& reservationIds = oldDateIt->second;

    if (std::find(reservationIds.begin(), reservationIds.end(), reservationId) == reservationIds.end())
        return false;

    if (!(oldDate == date))
    {
        schedule[date].push_back(reservationId);

        std::erase(reservationIds, reservationId);

        if (reservationIds.empty())
            schedule.erase(oldDateIt);
    }

    reservation.SetDate(date);
    reservation.SetTime(startTime, endTime);
    return true;
}

bool ReservationService::DeleteReservation(unsigned int reservationId)
{
    auto reservationIt = _reservations.find(reservationId);
    if (reservationIt == _reservations.end())
        return false;

    const Date& date = reservationIt->second->GetDate();

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

    auto scheduleIt = _tableSchedule.find(tableId);
    if (scheduleIt != _tableSchedule.end())
    {
        auto& schedule = scheduleIt->second;
        auto dateIt = schedule.find(date);

        if (dateIt != schedule.end())
        {
            std::erase(dateIt->second, reservationId);

            if (dateIt->second.empty())
                schedule.erase(dateIt);
        }
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

    const Date& date = reservation.GetDate();

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

    if (std::find(oldHistory.begin(), oldHistory.end(), reservationId) == oldHistory.end())
        return false;

    bool isNewTableAvaible =
        IsTableAvaible(newTableId, reservation.GetDate(), reservation.GetStartTime(), reservation.GetEndTime());

    if (!isNewTableAvaible)
        return false;

    auto oldScheduleIt = _tableSchedule.find(oldTableId);
    if (oldScheduleIt == _tableSchedule.end())
        return false;

    auto& oldSchedule = oldScheduleIt->second;

    auto dateIt = oldSchedule.find(date);
    if (dateIt == oldSchedule.end())
        return false;

    auto& oldReservationIds = dateIt->second;

    if (std::find(oldReservationIds.begin(), oldReservationIds.end(), reservationId) == oldReservationIds.end())
        return false;

    auto newScheduleIt = _tableSchedule.find(newTableId);
    if (newScheduleIt == _tableSchedule.end())
        return false;

    newHistory.push_back(reservationId);
    newScheduleIt->second[date].push_back(reservationId);

    std::erase(oldHistory, reservationId);
    std::erase(oldReservationIds, reservationId);

    if (oldReservationIds.empty())
        oldSchedule.erase(dateIt);

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

            if (!IsTableAvaible(tableId, date, startTime, endTime) ||
                !CanCustomerUseTable(customerIt->second, *tableIt->second))
                continue;

            return tableId;
        }
    }

    return std::nullopt;
}

std::optional<unsigned int> ReservationService::CreateReservationAuto(unsigned int customerId, unsigned int guestCount,
                                                                      const Date& date, const Time& startTime,
                                                                      const Time& endTime)
{
    auto tableId = FindSuitableTable(customerId, guestCount, date, startTime, endTime);

    if (!tableId.has_value())
        return std::nullopt;

    return CreateReservation(customerId, *tableId, guestCount, date, startTime, endTime);
}

std::optional<ReservationSuggestion> ReservationService::FindBestReservation(unsigned int customerId,
                                                                             unsigned int guestCount, const Date& date,
                                                                             const Time& startTime,
                                                                             const Time& endTime) const
{
    if (guestCount == 0 || !(startTime < endTime))
        return std::nullopt;

    auto customerIt = _customers.find(customerId);
    if (customerIt == _customers.end())
        return std::nullopt;

    unsigned int duration = endTime.ToMinutes() - startTime.ToMinutes();

    auto suitableTable = FindSuitableTable(customerId, guestCount, date, startTime, endTime);
    
    if (suitableTable.has_value())
    {
        return ReservationSuggestion(suitableTable.value(), date, startTime, endTime);
    }

    std::optional<ReservationSuggestion> bestSuggestion;

    unsigned int smallestTimeDifference = std::numeric_limits<unsigned int>::max();

    auto it = _tablesByCapacity.lower_bound(guestCount);

    for (; it != _tablesByCapacity.end(); ++it)
    {
        for (unsigned int tableId : it->second)
        {
            auto tableIt = _tables.find(tableId);
            if (tableIt == _tables.end())
                continue;

            if (!CanCustomerUseTable(customerIt->second, *tableIt->second))
                continue;

            auto freeIntervals = FindFreeIntervals(tableId, date);

            for (const auto& interval : freeIntervals)
            {
                const unsigned int intervalStart = interval.start.ToMinutes();
                const unsigned int intervalEnd = interval.end.ToMinutes();

                if (intervalEnd - intervalStart < duration)
                    continue;

                const unsigned int candidateStart = std::max(intervalStart, startTime.ToMinutes());

                const unsigned int candidateEnd = candidateStart + duration;

                if (candidateEnd > intervalEnd)
                    continue;

                const unsigned int difference = candidateStart - startTime.ToMinutes();

                if (difference < smallestTimeDifference)
                {
                    smallestTimeDifference = difference;

                    bestSuggestion = ReservationSuggestion(tableId, date, Time::FromMinutes(candidateStart),
                                                           Time::FromMinutes(candidateEnd));
                }
            }

        }
    }
    return bestSuggestion;
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

    auto scheduleIt = _tableSchedule.find(tableId);
    if (scheduleIt == _tableSchedule.end())
        return false;

    auto& schedule = scheduleIt->second;

    auto dateIt = schedule.find(date);
    if (dateIt == schedule.end())
        return true;

    for (unsigned int reservationId : dateIt->second)
    {
        if (excludeReservationId.has_value() && excludeReservationId == reservationId)
            continue;

        auto reservationIt = _reservations.find(reservationId);
        if (reservationIt == _reservations.end())
            return false;

        const Reservation& reservation = *reservationIt->second.get();

        if (reservation.GetStatus() != ReservationStatus::Confirmed)
            continue;
        if (!(reservation.GetDate() == date) || reservation.GetTableId() != tableId)
            return false;

        bool overlaps = startTime < reservation.GetEndTime() && reservation.GetStartTime() < endTime;

        if (overlaps)
            return false;
    }

    return true;
}

std::vector<TimeInterval> ReservationService::FindFreeIntervals(unsigned int tableId, const Date& date) const
{
    std::vector<TimeInterval> freeIntervals;

    auto tableIt = _tables.find(tableId);
    if (tableIt == _tables.end())
        return freeIntervals;

    auto scheduleIt = _tableSchedule.find(tableId);
    if (scheduleIt == _tableSchedule.end())
        return freeIntervals;

    auto dateIt = scheduleIt->second.find(date);
    if (dateIt == scheduleIt->second.end())
    {
        freeIntervals.push_back({Time(0, 0), Time(24, 00)});
        return freeIntervals;
    }

    std::vector<const Reservation*> reservations;

    for (unsigned int reservationId : dateIt->second)
    {
        auto reservationIt = _reservations.find(reservationId);
        if (reservationIt == _reservations.end())
            continue;

        const Reservation& reservation = *reservationIt->second;

        if (reservation.GetStatus() != ReservationStatus::Confirmed)
            continue;

        reservations.push_back(&reservation);
    }

    std::sort(reservations.begin(), reservations.end(), [](const Reservation* first, const Reservation* second)
              { return first->GetStartTime() < second->GetStartTime(); });

    Time currentTime(0, 0);
    Time closingTime(24, 0);

    for (const Reservation* reservation : reservations)
    {
        if (currentTime < reservation->GetStartTime())
        {
            freeIntervals.push_back({currentTime, reservation->GetStartTime()});
        }

        if (currentTime < reservation->GetEndTime())
            currentTime = reservation->GetEndTime();
    }

    if (currentTime < closingTime)
    {
        freeIntervals.push_back({currentTime, closingTime});
    }

    return freeIntervals;
}

