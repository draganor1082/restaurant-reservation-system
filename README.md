
# Restaurant Reservation Management System

A restaurant reservation management system built with **C++20**. The project focuses on reservation scheduling, business rules, efficient table selection, and automated testing.

## Features

### Implemented

- Customer management, including VIP status
- Regular and VIP tables with capacity and access restrictions
- Reservation creation, cancellation, modification, and deletion
- Date/time validation and prevention of overlapping reservations
- Reservation reassignment to another customer or table, and changes to guest count
- **Best Fit** table selection and automatic reservation creation
- Customer and table reservation histories
- Automatic cleanup of associated reservations when deleting customers or tables
- Date-based reservation indexing for faster availability checks
- Free time interval search for individual tables
- Alternative reservation time suggestions based on requested duration and table availability
- Unit tests with GoogleTest and Windows CI with GitHub Actions

### Planned

- SQLite persistence
- Console user interface
- Additional edge-case tests and Linux build verification

## Technology Stack

- **Language:** C++20
- **Build:** CMake
- **Testing:** GoogleTest, CTest
- **CI:** GitHub Actions (Windows)
- **Storage:** STL containers (SQLite planned)
- **Development:** Visual Studio, Git, GitHub, clang-format

## Build and Test

### Requirements

- C++20-compatible compiler
- CMake
- Git and internet access to fetch GoogleTest

### Build

```bash
cmake -S . -B build
cmake --build build --config Debug
```

### Run Tests

```bash
ctest --test-dir build -C Debug --output-on-failure
```

## Project Structure

```text
restaurant-reservation-system/
├── .github/workflows/    # Windows CI
├── include/
│   ├── Models/           # Customer, Reservation, Date, Time
│   ├── Services/         # ReservationService interface
│   ├── Table/            # Table, RegularTable, VIPTable interfaces
│   └── pch.hpp           # Precompiled header
├── src/
│   ├── Models/           # Model implementations
│   ├── Services/         # ReservationService implementation
│   ├── Table/            # Table implementations
│   └── main.cpp          # Application entry point
├── Tests/                # GoogleTest test suites
├── database/             # Reserved for persistence
├── CMakeLists.txt
└── README.md
```

## Architecture

`ReservationService` coordinates the main domain models:

- `Customer` — customer details and VIP status
- `Table` — polymorphic base for regular and VIP table types
- `Reservation` — table, customer, date, time, guest count, and status
- `Date` / `Time` — scheduling value types

Reservations are owned centrally with `std::unique_ptr` and looked up by ID using `std::unordered_map`. History indexes store reservation IDs rather than duplicate objects.

### Table Selection Algorithm

**Best Fit** selects the smallest available table that accommodates the requested guest count and satisfies VIP restrictions.

```cpp
std::map<unsigned int, std::vector<unsigned int>> _tablesByCapacity;
```

`lower_bound()` locates the first sufficient capacity in **O(log C)**, where `C` is the number of distinct capacities. Candidate tables are then checked for availability.

### Reservation Schedule Index

Confirmed reservations are indexed by table and date:

```cpp
std::unordered_map<unsigned int,
    std::map<Date, std::vector<unsigned int>>> _tableSchedule;
```

Availability checks examine only reservations for the requested table and date, rather than its entire history. The lookup takes **O(log D + K)** on average, where `D` is the number of scheduled dates for a table and `K` is the number of reservations on the selected date.

Cancelled reservations are removed from the active schedule but remain in reservation history.

### Free Time Intervals and Reservation Suggestions

`FindFreeIntervals()` calculates available time intervals for a table on a selected date using its reservation schedule.

`FindBestReservation()` searches for a suitable table and available time interval that can accommodate the requested reservation duration. If the requested time is unavailable, it returns an alternative suggestion without creating a reservation.

### Data Persistence

Data is currently stored in memory. SQLite support is planned.

## Continuous Integration

GitHub Actions builds and runs tests on Windows for pushes and pull requests.

Workflow: `.github/workflows/ci.yml`

## Status

**In Development.** Core reservation management, VIP rules, automatic table selection, date-based scheduling, free time interval search, and alternative reservation suggestions are implemented. Next priorities: SQLite persistence and a console interface.

## Author

**Andrii Nesterenko**
