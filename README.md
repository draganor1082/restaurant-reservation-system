# Restaurant Reservation Management System

A console-based restaurant reservation management system being developed in **C++17**, with SQLite integration planned.

The project focuses on object-oriented programming, reservation management, data validation, automated testing, and persistent storage. It is being developed as a practical C++ software engineering portfolio project.

## Features

### Implemented
- Date and time handling with validation and comparison operators
- Customer management with unique IDs and VIP status
- Restaurant table management using inheritance and polymorphism
- Regular and VIP tables with configurable capacity and availability status
- Reservation entities with unique IDs and status tracking
- Reservation time validation and overlap detection
- Reservation duration calculation
- Unit tests using GoogleTest
- Automated build and test workflow using GitHub Actions

### Planned
- Reservation creation, modification, and cancellation through a management service
- Table availability checks and VIP eligibility validation
- Alternative time slot suggestions
- Customer search and reservation history
- SQLite database integration for persistent storage
- Console-based user interface
- Additional unit tests for new components

## Technology Stack

- **Language:** C++17
- **Build System:** CMake
- **Database:** SQLite (planned)
- **Testing:** GoogleTest, CTest
- **CI:** GitHub Actions (Windows)
- **Development Tools:** Visual Studio 2026, Git, GitHub

## Building the Project

### Requirements
- C++17-compatible compiler
- CMake
- Git and an internet connection for fetching GoogleTest during configuration

### Build

```bash
cmake -S . -B build
cmake --build build --config Debug
```

### Running Unit Tests

```bash
ctest --test-dir build -C Debug --output-on-failure
```

The project currently contains 15 unit tests covering reservation time overlaps, customer and table IDs, reservation statuses, setters, and duration calculations.

## Continuous Integration

The project uses **GitHub Actions** to automatically build and test the application.

The CI workflow is triggered by:
- Pushes to the repository
- Pull requests

The pipeline runs on a Windows runner and performs the following steps:

1. Checks out the repository.
2. Configures the project using CMake.
3. Builds the application and test executable.
4. Runs unit tests through CTest and GoogleTest.

The workflow configuration is located in `.github/workflows/ci.yml`.

## Project Structure

```text
restaurant-reservation-system/
├── .github/
│   └── workflows/
│       └── ci.yml
├── include/
│   ├── Table/
│   │   ├── Table.hpp
│   │   ├── RegularTable.hpp
│   │   └── VIPTable.hpp
│   ├── Customer.hpp
│   ├── Date.hpp
│   ├── Time.hpp
│   ├── Reservation.hpp
│   └── pch.hpp
├── src/
│   ├── Table/
│   │   ├── Table.cpp
│   │   ├── RegularTable.cpp
│   │   └── VIPTable.cpp
│   ├── Date.cpp
│   ├── Time.cpp
│   ├── Reservation.cpp
│   └── main.cpp
├── Tests/
│   └── ReservationTests.cpp
├── database/
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Development Progress

- [x] Project setup with CMake and C++17
- [x] Precompiled headers configuration
- [x] `Date` class — validation, comparisons, leap years, weekday calculation
- [x] `Time` class — validation and comparisons
- [x] `Customer` class — unique IDs and VIP status
- [x] `Table` abstract base class
- [x] `RegularTable` and `VIPTable` derived classes
- [x] `Reservation` class — validation, status, duration, and overlap detection
- [x] Initial functionality testing through `main.cpp`
- [x] GoogleTest integration and initial unit tests
- [x] GitHub Actions CI workflow
- [ ] `ReservationService` — reservation management and availability checks
- [ ] SQLite database integration
- [ ] Console interface
- [ ] Additional unit test coverage
- [ ] Linux build verification
- [ ] Automated release workflow (CD)

## Architecture

The system follows an object-oriented design with a separation between domain entities and business logic.

**Core domain entities:**
- `Customer` — represents a restaurant customer and their VIP status.
- `Table` — abstract base class providing shared table properties and capacity validation.
- `RegularTable` — represents a standard restaurant table.
- `VIPTable` — extends `Table` with an additional reservation fee.
- `Reservation` — represents a reservation associated with a customer, table, date, time interval, and status.

**Planned components:**
- `ReservationService` — coordinates reservation operations, checks table availability, and enforces business rules.
- SQLite persistence layer — stores and retrieves restaurant data.
- Console interface — provides interaction with the reservation management system.

## Status

**In Development**

Core domain entities, initial unit tests, and the CI workflow are implemented.

The next development milestone is implementing `ReservationService` to connect the existing entities and introduce reservation management logic.

## Author

**Andrii Nesterenko**