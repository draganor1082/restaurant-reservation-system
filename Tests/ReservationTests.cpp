
#include <gtest/gtest.h>
#include <memory>
#include <optional>

#include "Customer.hpp"
#include "ReservationService.hpp"
#include "Table/RegularTable.hpp"

class TableSearchTest : public ::testing::Test
{
   protected:
    ReservationService service;

    Date date{10, 11, 2026};
    Time start{18, 0};
    Time end{20, 0};

    unsigned int AddCustomer()
    {
        Customer customer("Alex", "+48123456789");
        EXPECT_TRUE(service.AddCustomer(customer));
        return customer.GetId();
    }

    unsigned int AddTable(unsigned int capacity)
    {
        auto table = std::make_unique<RegularTable>(capacity);
        unsigned int id = table->GetId();

        EXPECT_TRUE(service.AddTable(std::move(table)));
        return id;
    }

    void Reserve(unsigned int customerId, unsigned int tableId, unsigned int guestCount)
    {
        ASSERT_TRUE(service.CreateReservation(customerId, tableId, guestCount, date, start, end));
    }
};

// Finds the smallest table with sufficient capacity
TEST_F(TableSearchTest, FindsBestFit)
{
    auto customerId = AddCustomer();

    AddTable(8);
    auto bestTable = AddTable(4);
    AddTable(6);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, bestTable);
}

// Skips an occupied table of the same capacity
TEST_F(TableSearchTest, SkipsOccupiedTable)
{
    auto customerId = AddCustomer();

    auto occupiedTable = AddTable(4);
    auto freeTable = AddTable(4);

    Reserve(customerId, occupiedTable, 2);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, freeTable);
}

// Moves to the next capacity when smaller tables are occupied
TEST_F(TableSearchTest, FindsLargerTable)
{
    auto customerId = AddCustomer();

    auto smallTable = AddTable(4);
    auto largeTable = AddTable(6);

    Reserve(customerId, smallTable, 3);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, largeTable);
}

// No table has enough capacity
TEST_F(TableSearchTest, RejectsInsufficientCapacity)
{
    auto customerId = AddCustomer();

    AddTable(2);
    AddTable(4);

    auto result = service.FindSuitableTable(customerId, 5, date, start, end);

    EXPECT_FALSE(result.has_value());
}

// All suitable tables are occupied
TEST_F(TableSearchTest, RejectsWhenAllTablesOccupied)
{
    auto customerId = AddCustomer();

    auto table1 = AddTable(4);
    auto table2 = AddTable(6);

    Reserve(customerId, table1, 3);
    Reserve(customerId, table2, 3);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    EXPECT_FALSE(result.has_value());
}

// Deleting a table updates the capacity index
TEST_F(TableSearchTest, UpdatesIndexAfterTableDeletion)
{
    auto customerId = AddCustomer();

    auto table1 = AddTable(4);
    auto table2 = AddTable(6);

    ASSERT_TRUE(service.DeleteTable(table1));

    EXPECT_EQ(service.GetTable(table1), nullptr);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, table2);
}

// Deleting an occupied table also deletes its reservations
TEST_F(TableSearchTest, DeletesTableWithReservations)
{
    auto customerId = AddCustomer();

    auto table1 = AddTable(4);
    auto table2 = AddTable(6);

    Reserve(customerId, table1, 3);

    auto history = service.GetTableHistory(table1);
    ASSERT_EQ(history.size(), 1u);

    unsigned int reservationId = history.front();

    ASSERT_TRUE(service.DeleteTable(table1));

    EXPECT_EQ(service.GetTable(table1), nullptr);
    EXPECT_EQ(service.GetReservation(reservationId), nullptr);
    EXPECT_TRUE(service.GetTableHistory(table1).empty());

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, table2);
}
