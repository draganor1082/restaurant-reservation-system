#include "Time.hpp"
#include "Customer.hpp"
#include "Date.hpp"


int main() {
    Date test("23.09.2026");
    Weekday day = test.GetWeekday();

    std::cout << (int)day;

    return 0;
}