#include "Date.hpp"
#include "Time.hpp"
#include "Table/RegularTable.hpp"
#include "Table/VIPTable.hpp"
#include "Reservation.hpp"

int main()
{
    int passed = 0;
    int failed = 0;

    auto check = [&](bool condition, const std::string& name)
        {
            std::cout << (condition ? "[PASS] " : "[FAIL] ")
                << name << '\n';

            condition ? ++passed : ++failed;
        };

    auto expectInvalidArgument = [&](auto&& action, const std::string& name)
        {
            try {
                action();
                check(false, name);
            }
            catch (const std::invalid_argument&) {
                check(true, name);
            }
            catch (...) {
                check(false, name);
            }
        };

    std::cout << "\n========== DATE & TIME ==========\n";

    Date date1(25, 10, 2026);
    Date date2(26, 10, 2026);

    Time t1800(18, 0);
    Time t1900(19, 0);
    Time t2000(20, 0);
    Time t2100(21, 0);
    Time t2200(22, 0);

    check(date1 < date2, "Date comparison");
    check(t1800 < t2000, "Time comparison");
    check(t2000 > t1800, "Reverse time comparison");

    expectInvalidArgument([] {
        Date invalid(31, 2, 2026);
        }, "Invalid date rejected");

    std::cout << "\n========== TABLES ==========\n";

    RegularTable regular(4);
    VIPTable vip(6, 100.0);

    check(regular.GetCapacity() == 4, "Regular table capacity");
    check(vip.GetCapacity() == 6, "VIP table capacity");
    check(vip.GetExtraFee() == 100.0, "VIP extra fee");

    check(regular.GetId() != vip.GetId(), "Unique table IDs");

    check(regular.CanAccommodate(2), "Regular table accepts 2 guests");
    check(regular.CanAccommodate(4), "Regular table accepts 4 guests");
    check(!regular.CanAccommodate(5), "Regular table rejects 5 guests");
    check(!regular.CanAccommodate(0), "Regular table rejects 0 guests");

    regular.SetActive(false);
    check(!regular.CanAccommodate(2), "Inactive table unavailable");
    regular.SetActive(true);

    expectInvalidArgument([] {
        RegularTable invalid(0);
        }, "Zero table capacity rejected");

    expectInvalidArgument([] {
        VIPTable invalid(4, -50.0);
        }, "Negative VIP fee rejected");

    std::cout << "\n========== RESERVATIONS ==========\n";

    // Customer IDs and table IDs are references.
    // Their existence will be checked by ReservationService.
    const unsigned int customer1 = 1;
    const unsigned int customer2 = 2;

    Reservation r1(
        customer1, regular.GetId(),
        date1, t1800, t2000
    );

    Reservation r2(
        customer2, regular.GetId(),
        date1, t1900, t2100
    );

    Reservation r3(
        customer1, vip.GetId(),
        date1, t2000, t2200
    );

    Reservation r4(
        customer2, regular.GetId(),
        date2, t1900, t2100
    );

    check(r1.GetId() != r2.GetId(), "Unique reservation IDs");
    check(r1.GetCustomerId() == customer1, "Customer ID stored");
    check(r1.GetTableId() == regular.GetId(), "Table ID stored");
    check(r1.GetStatus() == ReservationStatus::Confirmed,
        "Default reservation status");

    std::cout << "\n========== OVERLAP TESTS ==========\n";

    check(r1.OverlapsWith(r2),
        "Overlapping intervals: 18-20 and 19-21");

    check(r2.OverlapsWith(r1),
        "Overlap is symmetric");

    check(!r1.OverlapsWith(r3),
        "Touching intervals: 18-20 and 20-22");

    check(!r1.OverlapsWith(r4),
        "Different dates do not overlap");

    check(Reservation::OverlapsWith(r1, r2),
        "Static OverlapsWith");

    // Deliberately different tables: your OverlapsWith()
    // compares only dates and times.
    Reservation differentTable(
        customer2, vip.GetId(),
        date1, t1900, t2100
    );

    check(r1.OverlapsWith(differentTable),
        "Time overlap independent of table ID");

    Reservation inside(
        customer2, vip.GetId(),
        date1, Time(18, 30), Time(19, 30)
    );

    check(r1.OverlapsWith(inside),
        "One interval fully inside another");

    std::cout << "\n========== DURATION ==========\n";

    check(r1.GetDuration() == 120, "Duration: 120 minutes");

    Reservation shortReservation(
        customer1, regular.GetId(),
        date1, Time(18, 30), Time(19, 45)
    );

    check(shortReservation.GetDuration() == 75,
        "Duration: 75 minutes");

    std::cout << "\n========== VALIDATION ==========\n";

    expectInvalidArgument([&] {
        Reservation invalid(
            0, regular.GetId(), date1, t1800, t2000
        );
        }, "Customer ID zero rejected");

    expectInvalidArgument([&] {
        Reservation invalid(
            customer1, 0, date1, t1800, t2000
        );
        }, "Table ID zero rejected");

    expectInvalidArgument([&] {
        Reservation invalid(
            customer1, regular.GetId(), date1, t2000, t1800
        );
        }, "End before start rejected");

    expectInvalidArgument([&] {
        Reservation invalid(
            customer1, regular.GetId(), date1, t2000, t2000
        );
        }, "Equal start and end rejected");

    std::cout << "\n========== SETTERS ==========\n";

    r1.SetTime(t1900, t2100);
    check(r1.GetStartTime() == t1900 &&
        r1.GetEndTime() == t2100,
        "SetTime updates interval");

    expectInvalidArgument([&] {
        r1.SetTime(t2200, t1800);
        }, "SetTime rejects invalid interval");

    check(r1.GetStartTime() == t1900 &&
        r1.GetEndTime() == t2100,
        "Failed SetTime preserves old values");

    r1.SetDate(date2);
    check(r1.GetDate() == date2, "SetDate updates date");

    r1.SetTableId(vip.GetId());
    check(r1.GetTableId() == vip.GetId(),
        "SetTableId updates table");

    r1.SetCustomerId(customer2);
    check(r1.GetCustomerId() == customer2,
        "SetCustomerId updates customer");

    std::cout << "\n========== OUTPUT ==========\n";

    std::cout << r1 << "\n\n";
    std::cout << r2 << '\n';

    std::cout << "\n========== TEST SUMMARY ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}