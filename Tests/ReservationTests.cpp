
#include <gtest/gtest.h>

#include "ReservationService.hpp"
#include "Table/RegularTable.hpp"

#include <memory>
#include <optional>

class TableScheduleTest : public ::testing::Test
{
   protected:
    ReservationService service;

    Date date{10, 11, 2026};
    Date nextDate{11, 11, 2026};

    Time start{18, 0};
    Time end{20, 0};

    Time laterStart{20, 0};
    Time laterEnd{22, 0};

    unsigned int AddCustomer()
    {
        Customer customer("Alex", "+48123456789");

        EXPECT_TRUE(service.AddCustomer(customer));

        return customer.GetId();
    }

    unsigned int AddTable(unsigned int capacity = 4)
    {
        auto table = std::make_unique<RegularTable>(capacity);
        unsigned int tableId = table->GetId();

        EXPECT_TRUE(service.AddTable(std::move(table)));

        return tableId;
    }

    std::optional<unsigned int> Reserve(unsigned int customerId, unsigned int tableId, const Date& reservationDate)
    {
        return service.CreateReservation(customerId, tableId, 3, reservationDate, start, end);
    }
};

// Creating a reservation blocks only its table and date.

TEST_F(TableScheduleTest, ReservationBlocksOnlyMatchingDateAndTable)
{
    auto customerId = AddCustomer();
    auto table1 = AddTable();
    auto table2 = AddTable();

    auto reservationId = Reserve(customerId, table1, date);
    ASSERT_TRUE(reservationId.has_value());

    EXPECT_FALSE(service.IsTableAvaible(table1, date, start, end));

    EXPECT_TRUE(service.IsTableAvaible(table1, nextDate, start, end));

    EXPECT_TRUE(service.IsTableAvaible(table2, date, start, end));
}

// Cancelling a reservation frees the time slot but keeps history.

TEST_F(TableScheduleTest, CancellationFreesTimeSlot)
{
    auto customerId = AddCustomer();
    auto tableId = AddTable();

    auto reservationId = Reserve(customerId, tableId, date);
    ASSERT_TRUE(reservationId.has_value());

    ASSERT_TRUE(service.CancelReservation(*reservationId));

    EXPECT_TRUE(service.IsTableAvaible(tableId, date, start, end));

    const Reservation* reservation = service.GetReservation(*reservationId);

    ASSERT_NE(reservation, nullptr);
    EXPECT_EQ(reservation->GetStatus(), ReservationStatus::Cancelled);

    auto history = service.GetTableHistory(tableId);

    ASSERT_EQ(history.size(), 1);
    EXPECT_EQ(history[0], *reservationId);

    auto newReservation = Reserve(customerId, tableId, date);

    EXPECT_TRUE(newReservation.has_value());
}

// Moving a reservation to another date updates availability.

TEST_F(TableScheduleTest, ChangingDateUpdatesSchedule)
{
    auto customerId = AddCustomer();
    auto tableId = AddTable();

    auto reservationId = Reserve(customerId, tableId, date);
    ASSERT_TRUE(reservationId.has_value());

    ASSERT_TRUE(service.ChangeReservationTime(*reservationId, nextDate, start, end));

    EXPECT_TRUE(service.IsTableAvaible(tableId, date, start, end));

    EXPECT_FALSE(service.IsTableAvaible(tableId, nextDate, start, end));

    const Reservation* reservation = service.GetReservation(*reservationId);

    ASSERT_NE(reservation, nullptr);
    EXPECT_TRUE(reservation->GetDate() == nextDate);
}

// Changing only the time must not break the date index.

TEST_F(TableScheduleTest, ChangingTimeWithinSameDate)
{
    auto customerId = AddCustomer();
    auto tableId = AddTable();

    auto reservationId = Reserve(customerId, tableId, date);
    ASSERT_TRUE(reservationId.has_value());

    ASSERT_TRUE(service.ChangeReservationTime(*reservationId, date, laterStart, laterEnd));

    EXPECT_TRUE(service.IsTableAvaible(tableId, date, start, end));

    EXPECT_FALSE(service.IsTableAvaible(tableId, date, laterStart, laterEnd));

    EXPECT_TRUE(service.IsTableAvaible(tableId, nextDate, laterStart, laterEnd));
}

// Moving a reservation to another table updates both schedules.

TEST_F(TableScheduleTest, ChangingTableUpdatesBothSchedules)
{
    auto customerId = AddCustomer();
    auto oldTableId = AddTable();
    auto newTableId = AddTable();

    auto reservationId = Reserve(customerId, oldTableId, date);
    ASSERT_TRUE(reservationId.has_value());

    ASSERT_TRUE(service.ChangeReservationTable(*reservationId, newTableId));

    EXPECT_TRUE(service.IsTableAvaible(oldTableId, date, start, end));

    EXPECT_FALSE(service.IsTableAvaible(newTableId, date, start, end));

    const Reservation* reservation = service.GetReservation(*reservationId);

    ASSERT_NE(reservation, nullptr);
    EXPECT_EQ(reservation->GetTableId(), newTableId);
}

// Deleting a reservation removes it from schedule and history.

TEST_F(TableScheduleTest, DeletingReservationFreesTimeSlot)
{
    auto customerId = AddCustomer();
    auto tableId = AddTable();

    auto reservationId = Reserve(customerId, tableId, date);
    ASSERT_TRUE(reservationId.has_value());

    ASSERT_TRUE(service.DeleteReservation(*reservationId));

    EXPECT_EQ(service.GetReservation(*reservationId), nullptr);

    EXPECT_TRUE(service.IsTableAvaible(tableId, date, start, end));

    EXPECT_TRUE(service.GetTableHistory(tableId).empty());

    EXPECT_TRUE(service.GetCustomerHistort(customerId).empty());

    auto newReservation = Reserve(customerId, tableId, date);

    EXPECT_TRUE(newReservation.has_value());
}

// Deleting a customer removes all associated reservations.

TEST_F(TableScheduleTest, DeletingCustomerClearsMultipleDates)
{
    auto customerId = AddCustomer();
    auto table1 = AddTable();
    auto table2 = AddTable();

    auto reservation1 = Reserve(customerId, table1, date);
    auto reservation2 = Reserve(customerId, table1, nextDate);
    auto reservation3 = Reserve(customerId, table2, date);

    ASSERT_TRUE(reservation1.has_value());
    ASSERT_TRUE(reservation2.has_value());
    ASSERT_TRUE(reservation3.has_value());

    ASSERT_TRUE(service.DeleteCustomer(customerId));

    EXPECT_EQ(service.GetReservation(*reservation1), nullptr);
    EXPECT_EQ(service.GetReservation(*reservation2), nullptr);
    EXPECT_EQ(service.GetReservation(*reservation3), nullptr);

    EXPECT_TRUE(service.IsTableAvaible(table1, date, start, end));
    EXPECT_TRUE(service.IsTableAvaible(table1, nextDate, start, end));
    EXPECT_TRUE(service.IsTableAvaible(table2, date, start, end));

    EXPECT_TRUE(service.GetTableHistory(table1).empty());
    EXPECT_TRUE(service.GetTableHistory(table2).empty());
}

// Deleting a table removes its reservations from all dates.

TEST_F(TableScheduleTest, DeletingTableClearsMultipleDates)
{
    auto customerId = AddCustomer();
    auto tableId = AddTable();

    auto reservation1 = Reserve(customerId, tableId, date);
    auto reservation2 = Reserve(customerId, tableId, nextDate);

    ASSERT_TRUE(reservation1.has_value());
    ASSERT_TRUE(reservation2.has_value());

    ASSERT_TRUE(service.DeleteTable(tableId));

    EXPECT_EQ(service.GetTable(tableId), nullptr);

    EXPECT_EQ(service.GetReservation(*reservation1), nullptr);
    EXPECT_EQ(service.GetReservation(*reservation2), nullptr);

    EXPECT_TRUE(service.GetCustomerHistort(customerId).empty());

    EXPECT_FALSE(service.IsTableAvaible(tableId, date, start, end));
}
