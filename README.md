# Restaurant Reservation Management System

A console-based restaurant reservation management system built with **C++20**.

The project focuses on object-oriented design, reservation scheduling, business logic, STL containers, and automated testing. Developed as a practical C++ software engineering portfolio project.

## Features

### Implemented

- Customer and table management, including VIP support
- Reservation creation, cancellation, modification, and deletion
- Reservation time validation and overlap detection
- Table availability checks and reservation rescheduling
- Customer reassignment for existing reservations
- Customer and table reservation history
- Automatic removal of associated reservations when deleting customers or tables
- In-memory data management using STL containers
- Unit testing with GoogleTest
- Automated build and testing with GitHub Actions

### Planned

- Extended VIP reservation rules
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
├── .github/workflows/
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
│   └── ReservationTests.cpp
├── database/
├── CMakeLists.txt
└── README.md
```

## Architecture

The system separates domain entities from reservation management logic.

**Core components:**

- `Customer` — customer information and VIP status
- `Table` — polymorphic base class for regular and VIP tables
- `Reservation` — reservation details, scheduling, and status
- `Date` / `Time` — date and time validation
- `ReservationService` — central service for managing customers, tables, reservations, and their relationships

The service uses `std::unordered_map` for efficient ID-based lookups, `std::vector` for reservation history indexes, and `std::unique_ptr` for object ownership.

Reservation histories reference centrally stored reservation objects by ID, avoiding data duplication.

SQLite persistence is planned to replace the current in-memory-only storage.

## Continuous Integration

GitHub Actions automatically builds the project and executes unit tests on Windows for every push and pull request.

Workflow: `.github/workflows/ci.yml`

## Status

**In Development**

Core domain models and reservation management operations are implemented.

Current priorities are expanding test coverage, refining business rules, and preparing SQLite persistence.

## Author

**Andrii Nesterenko**