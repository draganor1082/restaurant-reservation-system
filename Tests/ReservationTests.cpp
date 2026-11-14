
#include <gtest/gtest.h>

#include "Services/ReservationService.hpp"
#include "Table/RegularTable.hpp"
#include "Table/VIPTable.hpp"

#include <memory>
#include <optional>

class FindBestReservationTest : public ::testing::Test
{
   protected:
    ReservationService service;

    Date date{10, 11, 2026};

    Time requestedStart{18, 0};
    Time requestedEnd{20, 0};

    unsigned int regularCustomerId;
    unsigned int vipCustomerId;

    unsigned int AddCustomer(const std::string& name, bool isVip = false)
    {
        Customer customer(name, "+48123456789", isVip);

        EXPECT_TRUE(service.AddCustomer(customer));

        return customer.GetId();
    }

    unsigned int AddRegularTable(unsigned int capacity)
    {
        auto table = std::make_unique<RegularTable>(capacity);
        unsigned int id = table->GetId();

        EXPECT_TRUE(service.AddTable(std::move(table)));

        return id;
    }

    unsigned int AddVIPTable(unsigned int capacity)
    {
        auto table = std::make_unique<VIPTable>(capacity, 50.0);
        unsigned int id = table->GetId();

        EXPECT_TRUE(service.AddTable(std::move(table)));

        return id;
    }

    std::optional<unsigned int> Reserve(unsigned int customerId, unsigned int tableId, Time start, Time end)
    {
        return service.CreateReservation(customerId, tableId, 2, date, start, end);
    }

    void SetUp() override
    {
        regularCustomerId = AddCustomer("Alex");
        vipCustomerId = AddCustomer("John", true);
    }
};

// Exact requested time is available.

TEST_F(FindBestReservationTest, ReturnsOriginalTimeWhenAvailable)
{
    unsigned int tableId = AddRegularTable(4);

    auto result = service.FindBestReservation(regularCustomerId, 2, date, requestedStart, requestedEnd);

    ASSERT_TRUE(result.has_value());

    EXPECT_EQ(result->tableId, tableId);
    EXPECT_TRUE(result->date == date);
    EXPECT_TRUE(result->startTime == requestedStart);
    EXPECT_TRUE(result->endTime == requestedEnd);
}

// If the requested time is occupied, find a later available interval.

TEST_F(FindBestReservationTest, FindsAlternativeTimeAfterRequestedTime)
{
    unsigned int tableId = AddRegularTable(4);

    ASSERT_TRUE(Reserve(regularCustomerId, tableId, Time(18, 0), Time(20, 0)).has_value());

    auto result = service.FindBestReservation(regularCustomerId, 2, date, Time(19, 0), Time(21, 0));

    ASSERT_TRUE(result.has_value());

    EXPECT_EQ(result->tableId, tableId);
    EXPECT_TRUE(result->date == date);

    EXPECT_TRUE(result->startTime == Time(20, 0));
    EXPECT_TRUE(result->endTime == Time(22, 0));
}

// The suggested reservation must preserve the requested duration.

TEST_F(FindBestReservationTest, PreservesRequestedDuration)
{
    unsigned int tableId = AddRegularTable(4);

    ASSERT_TRUE(Reserve(regularCustomerId, tableId, Time(18, 0), Time(20, 0)).has_value());

    auto result = service.FindBestReservation(regularCustomerId, 2, date, Time(19, 0), Time(21, 0));

    ASSERT_TRUE(result.has_value());

    EXPECT_EQ(result->endTime.ToMinutes() - result->startTime.ToMinutes(),
              requestedEnd.ToMinutes() - requestedStart.ToMinutes());
}

// No suitable interval is long enough.

TEST_F(FindBestReservationTest, ReturnsNulloptWhenNoIntervalIsLongEnough)
{
    unsigned int tableId = AddRegularTable(4);

    ASSERT_TRUE(Reserve(regularCustomerId, tableId, Time(17, 0), Time(19, 30)).has_value());

    ASSERT_TRUE(Reserve(regularCustomerId, tableId, Time(20, 30), Time(23, 0)).has_value());

    auto result = service.FindBestReservation(regularCustomerId, 2, date, Time(19, 0), Time(21, 0));

    EXPECT_FALSE(result.has_value());
}

// Invalid customer ID.

TEST_F(FindBestReservationTest, ReturnsNulloptForInvalidCustomer)
{
    AddRegularTable(4);

    auto result = service.FindBestReservation(9999, 2, date, requestedStart, requestedEnd);

    EXPECT_FALSE(result.has_value());
}

// Zero guests are not allowed.

TEST_F(FindBestReservationTest, ReturnsNulloptForZeroGuests)
{
    AddRegularTable(4);

    auto result = service.FindBestReservation(regularCustomerId, 0, date, requestedStart, requestedEnd);

    EXPECT_FALSE(result.has_value());
}

// Invalid time range.

TEST_F(FindBestReservationTest, ReturnsNulloptForInvalidTimeRange)
{
    AddRegularTable(4);

    auto result = service.FindBestReservation(regularCustomerId, 2, date, Time(20, 0), Time(18, 0));

    EXPECT_FALSE(result.has_value());
}

// Regular customers cannot use VIP tables.

TEST_F(FindBestReservationTest, RegularCustomerCannotUseVIPTable)
{
    AddVIPTable(4);

    auto result = service.FindBestReservation(regularCustomerId, 2, date, requestedStart, requestedEnd);

    EXPECT_FALSE(result.has_value());
}

// VIP customers can use VIP tables.

TEST_F(FindBestReservationTest, VIPCustomerCanUseVIPTable)
{
    unsigned int tableId = AddVIPTable(4);

    auto result = service.FindBestReservation(vipCustomerId, 2, date, requestedStart, requestedEnd);

    ASSERT_TRUE(result.has_value());

    EXPECT_EQ(result->tableId, tableId);
}

// Best Fit: choose the smallest table that can accommodate the guests.

TEST_F(FindBestReservationTest, SelectsSmallestSuitableTable)
{
    AddRegularTable(8);
    unsigned int smallerTableId = AddRegularTable(4);

    auto result = service.FindBestReservation(regularCustomerId, 3, date, requestedStart, requestedEnd);

    ASSERT_TRUE(result.has_value());

    EXPECT_EQ(result->tableId, smallerTableId);
}

// A regular customer must skip VIP tables and find a regular alternative.

TEST_F(FindBestReservationTest, SkipsVIPTableAndFindsRegularTable)
{
    AddVIPTable(4);
    unsigned int regularTableId = AddRegularTable(6);

    auto result = service.FindBestReservation(regularCustomerId, 2, date, requestedStart, requestedEnd);

    ASSERT_TRUE(result.has_value());

    EXPECT_EQ(result->tableId, regularTableId);
}

// Empty schedule: a table with no reservations should have free intervals.

TEST_F(FindBestReservationTest, FindsTimeOnTableWithoutReservations)
{
    AddRegularTable(4);

    auto result = service.FindBestReservation(regularCustomerId, 2, date, requestedStart, requestedEnd);

    EXPECT_TRUE(result.has_value());
}
