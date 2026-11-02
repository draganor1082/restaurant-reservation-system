#include <gtest/gtest.h>
#include "ReservationService.hpp"

// ============================================================
// DeleteReservation
// ============================================================

TEST(ReservationServiceDeleteTest, DeleteNonExistingReservation)
{
    ReservationService service;

    EXPECT_FALSE(service.DeleteReservation(999));
    EXPECT_EQ(service.GetReservation(999), nullptr);
}

TEST(ReservationServiceDeleteTest, DeleteReservationFromEmptyService)
{
    ReservationService service;

    EXPECT_FALSE(service.DeleteReservation(100));
}

// ============================================================
// DeleteCustomer
// ============================================================

TEST(ReservationServiceDeleteTest, DeleteNonExistingCustomer)
{
    ReservationService service;

    EXPECT_FALSE(service.DeleteCustomer(999));
    EXPECT_EQ(service.GetCustomer(999), nullptr);
}

TEST(ReservationServiceDeleteTest, DeleteCustomerFromEmptyService)
{
    ReservationService service;

    EXPECT_FALSE(service.DeleteCustomer(1));
}

// ============================================================
// DeleteTable
// ============================================================

TEST(ReservationServiceDeleteTest, DeleteNonExistingTable)
{
    ReservationService service;

    EXPECT_FALSE(service.DeleteTable(999));
    EXPECT_EQ(service.GetTable(999), nullptr);
}

TEST(ReservationServiceDeleteTest, DeleteTableFromEmptyService)
{
    ReservationService service;

    EXPECT_FALSE(service.DeleteTable(10));
}

// ============================================================
// Check that failed deletions don't affect other empty indexes
// ============================================================

TEST(ReservationServiceDeleteTest, FailedDeletionKeepsServiceEmpty)
{
    ReservationService service;

    EXPECT_FALSE(service.DeleteReservation(100));
    EXPECT_FALSE(service.DeleteCustomer(1));
    EXPECT_FALSE(service.DeleteTable(10));

    EXPECT_EQ(service.GetReservation(100), nullptr);
    EXPECT_EQ(service.GetCustomer(1), nullptr);
    EXPECT_EQ(service.GetTable(10), nullptr);
}
