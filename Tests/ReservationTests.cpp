#include <gtest/gtest.h>
#include "Reservation.hpp"

TEST(ReservationTest, SimpleCheck) { EXPECT_EQ(2 + 2, 4); }

TEST(ReservationTest, OverlappingTimeIntervals_1)
{
    Date date("26.10.2026");
    Time start1(14, 0);
    Time end1(15, 0);

    Time start2(14, 30);
    Time end2(15, 30);

    Reservation r1(1, 1, date, start1, end1);
    Reservation r2(2, 1, date, start2, end2);

    EXPECT_TRUE(Reservation::OverlapsWith(r1, r2));
}

TEST(ReservationTest, OverlappingTimeIntervals_2)
{
    Date date("26.10.2026");
    Time start1(14, 0);
    Time end1(15, 0);

    Time start2(15, 00);
    Time end2(15, 30);

    Reservation r1(1, 1, date, start1, end1);
    Reservation r2(2, 1, date, start2, end2);

    EXPECT_FALSE(Reservation::OverlapsWith(r1, r2));
}

TEST(ReservationTest, OverlappingTimeIntervals_3)
{
    Date date("26.10.2026");
    Time start1(14, 0);
    Time end1(16, 0);

    Time start2(14, 30);
    Time end2(23, 30);

    Reservation r1(1, 1, date, start1, end1);
    Reservation r2(2, 1, date, start2, end2);

    EXPECT_TRUE(Reservation::OverlapsWith(r1, r2));
}

TEST(ReservationTest, GetCustomerId)
{
    Date date("26.10.2026");
    Time start(14, 0), end(15, 0);

    Reservation r(42, 1, date, start, end);

    EXPECT_EQ(r.GetCustomerId(), 42u);
}

TEST(ReservationTest, GetTableId)
{
    Date date("26.10.2026");
    Time start(14, 0), end(15, 0);

    Reservation r(1, 7, date, start, end);

    EXPECT_EQ(r.GetTableId(), 7u);
}

TEST(ReservationTest, DefaultStatusIsConfirmed)
{
    Date date("26.10.2026");
    Time start(14, 0), end(15, 0);

    Reservation r(1, 1, date, start, end);

    EXPECT_EQ(r.GetStatus(), ReservationStatus::Confirmed);
}

TEST(ReservationTest, CustomStatusIsCancelled)
{
    Date date("26.10.2026");
    Time start(14, 0), end(15, 0);

    Reservation r(1, 1, date, start, end, ReservationStatus::Cancelled);

    EXPECT_EQ(r.GetStatus(), ReservationStatus::Cancelled);
}

TEST(ReservationTest, SetCustomerId)
{
    Date date("26.10.2026");
    Time start(14, 0), end(15, 0);

    Reservation r(1, 1, date, start, end);

    r.SetCustomerId(25);

    EXPECT_EQ(r.GetCustomerId(), 25u);
}

TEST(ReservationTest, SetTableId)
{
    Date date("26.10.2026");
    Time start(14, 0), end(15, 0);

    Reservation r(1, 1, date, start, end);

    r.SetTableId(10);

    EXPECT_EQ(r.GetTableId(), 10u);
}

TEST(ReservationTest, GetDuration)
{
    Date date("26.10.2026");
    Time start(14, 0), end(16, 30);

    Reservation r(1, 1, date, start, end);

    EXPECT_EQ(r.GetDuration(), 150u);
}
