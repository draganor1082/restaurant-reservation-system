# Restaurant Reservation Management System

A console-oriented restaurant reservation management system built with **C++20**.

The project focuses on object-oriented design, reservation scheduling, business logic, STL containers, efficient table selection, and automated testing. Developed as a practical C++ software engineering portfolio project.

## Features

### Implemented

- Customer and table management, including VIP customer and table types
- Reservation creation, cancellation, modification, and deletion
- Reservation time validation and overlap detection
- Table availability checks and reservation rescheduling
- Customer reassignment and table changes for existing reservations
- Guest count validation and modification based on table capacity
- **Best Fit table search** — selects the smallest available table with sufficient capacity
- Capacity-based table indexing using `std::map` and `lower_bound()`
- Customer and table reservation history
- Automatic removal of associated reservations when deleting customers or tables
- In-memory data management using STL containers
- Unit testing with GoogleTest
- Automated build and testing with GitHub Actions

### Planned

- VIP reservation eligibility rules
- Automatic reservation creation using Best Fit table selection
- Alternative available time slot suggestions
- SQLite integration for persistent storage
- Console-based user interface
- Additional unit tests and edge-case handling
- Linux build verification

## Technology Stack

- **Language:** C++20
- **Build System:** CMake
- **Testing:** GoogleTest, CTest
- **CI:** GitHub Actions (Windows)
- **Database:** SQLite (planned)
- **Tools:** Visual Studio 2026, Git, GitHub, clang-format

## Build and Test

### Requirements

- C++20-compatible compiler
- CMake
- Git and internet access for fetching GoogleTest

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
├── .github/
│   └── workflows/
│       └── ci.yml
├── include/
│   ├── Table/
│   ├── Customer.hpp
│   ├── Date.hpp
│   ├── Time.hpp
│   ├── Reservation.hpp
│   └── ReservationService.hpp
├── src/
│   ├── Table/
│   ├── ReservationService.cpp
│   └── ...
├── Tests/
│   ├── ReservationTests.cpp
│   └── TableSearchTests.cpp
├── database/
├── CMakeLists.txt
└── README.md
```

## Architecture

The system separates domain entities from reservation management logic.

**Core components:**

- `Customer` — customer information and VIP status
- `Table` — polymorphic base class for regular and VIP tables
- `Reservation` — reservation details, guest count, scheduling, and status
- `Date` / `Time` — date and time validation
- `ReservationService` — central service for managing customers, tables, reservations, and their relationships

The service uses `std::unordered_map` for efficient ID-based lookups, `std::vector` for reservation history indexes, and `std::unique_ptr` for object ownership.

Reservation histories reference centrally stored reservation objects by ID, avoiding data duplication.

### Table Selection Algorithm

The system implements a **Best Fit** strategy to find the smallest available table capable of accommodating a requested number of guests.

Tables are indexed by capacity using:

```cpp
std::map<unsigned int, std::vector<unsigned int>> _tablesByCapacity;
```

The algorithm uses `lower_bound()` to locate the smallest sufficient capacity, then checks the corresponding tables for availability. If none are available, it continues to larger capacities.

The initial capacity lookup takes **O(log k)**, where `k` is the number of distinct table capacities. Availability checks depend on the number of candidate tables and their reservation histories.

The capacity index is maintained when tables are added or deleted.

### Data Persistence

The current implementation stores data in memory. SQLite integration is planned to provide persistent storage.

## Continuous Integration

GitHub Actions automatically builds the project and executes unit tests on Windows for every push and pull request.

Workflow: `.github/workflows/ci.yml`

## Status

**In Development**

Core reservation management operations, guest count validation, table reassignment, and capacity-based table search are implemented and covered by automated tests.

Current priorities are implementing VIP eligibility rules, integrating automatic table selection into reservation creation, and preparing SQLite persistence.

## Author

**Andrii Nesterenko**