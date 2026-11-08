
#include <gtest/gtest.h>

#include "ReservationService.hpp"
#include "Table/RegularTable.hpp"
#include "Table/VIPTable.hpp"

#include <memory>

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

    unsigned int AddVIPCustomer()
    {
        Customer customer("Alex", "+48123456789", true);
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

    unsigned int AddVIPTable(unsigned int capacity)
    {
        auto table = std::make_unique<VIPTable>(capacity, 10.0);
        unsigned int id = table->GetId();

        EXPECT_TRUE(service.AddTable(std::move(table)));
        return id;
    }

    void Reserve(unsigned int customerId, unsigned int tableId, unsigned int guestCount)
    {
        ASSERT_TRUE(service.CreateReservation(customerId, tableId, guestCount, date, start, end));
    }
};

// Best-fit search

TEST_F(TableSearchTest, FindsBestFit)
{
    auto customerId = AddCustomer();

    AddTable(2);
    auto bestTableId = AddTable(4);
    AddTable(6);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, bestTableId);
}

TEST_F(TableSearchTest, SkipsOccupiedTable)
{
    auto customerId = AddCustomer();

    auto occupiedTableId = AddTable(4);
    auto availableTableId = AddTable(4);

    Reserve(customerId, occupiedTableId, 3);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, availableTableId);
}

TEST_F(TableSearchTest, FindsLargerTable)
{
    auto customerId = AddCustomer();

    AddTable(4);
    auto largerTableId = AddTable(6);

    auto result = service.FindSuitableTable(customerId, 5, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, largerTableId);
}

TEST_F(TableSearchTest, RejectsWhenAllTablesOccupied)
{
    auto customerId = AddCustomer();

    auto table1 = AddTable(4);
    auto table2 = AddTable(4);

    Reserve(customerId, table1, 3);
    Reserve(customerId, table2, 3);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    EXPECT_FALSE(result.has_value());
}

// VIP restrictions

TEST_F(TableSearchTest, RegularCustomerCannotUseVIPTable)
{
    auto customerId = AddCustomer();
    auto vipTableId = AddVIPTable(4);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    EXPECT_FALSE(result.has_value());

    EXPECT_FALSE(service.CreateReservation(customerId, vipTableId, 3, date, start, end));
}

TEST_F(TableSearchTest, VIPCustomerCanUseVIPTable)
{
    auto customerId = AddVIPCustomer();
    auto vipTableId = AddVIPTable(4);

    auto result = service.FindSuitableTable(customerId, 3, date, start, end);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, vipTableId);

    EXPECT_TRUE(service.CreateReservation(customerId, vipTableId, 3, date, start, end));
}
