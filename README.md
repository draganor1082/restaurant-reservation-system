# Restaurant Reservation Management System

A console-based restaurant reservation management system under development in **C++17**, with SQLite integration planned.

The project focuses on object-oriented programming, reservation management, data validation, STL containers, automated testing, and persistent storage. It is being developed as a practical C++ software engineering portfolio project.

## Features

### Implemented

- Date and time handling with validation and comparison operators
- Customer entities with unique IDs and VIP status
- Restaurant table management using inheritance and polymorphism
- Regular and VIP tables with configurable capacity and availability status
- Reservation entities with unique IDs and status tracking
- Reservation time validation and overlap detection
- Reservation duration calculation
- `ReservationService` structure for managing customers, tables, and reservations
- Customer and table lookup using `std::unordered_map`
- Reservation history indexing by customer ID and table ID
- Reservation cancellation logic with status tracking
- Table availability checking logic
- Unit tests using GoogleTest
- Automated build and test workflow using GitHub Actions

### In Progress

- Integration and verification of reservation creation through `ReservationService`
- Enforcement of VIP eligibility and table-specific reservation rules
- Additional unit tests for `ReservationService`

### Planned

- Reservation modification and rescheduling
- Alternative available time slot suggestions
- SQLite database integration for persistent storage
- Console-based user interface
- Customer and reservation search functionality
- Extended error handling and validation
- Linux build verification
- Automated release workflow (CD)

## Technology Stack

- **Language:** C++17
- **Standard Library:** STL (`unordered_map`, `vector`, `unique_ptr`)
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

The project includes 15 initial unit tests covering reservation time overlaps, customer and table IDs, reservation statuses, setters, and duration calculations.

Additional tests for the reservation management service are planned.

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
│   ├── ReservationService.hpp
│   └── pch.hpp
├── src/
│   ├── Table/
│   │   ├── Table.cpp
│   │   ├── RegularTable.cpp
│   │   └── VIPTable.cpp
│   ├── Date.cpp
│   ├── Time.cpp
│   ├── Reservation.cpp
│   ├── ReservationService.cpp
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
- [x] `ReservationService` class structure and STL-based storage
- [x] Customer and table registration and lookup
- [x] Customer and table reservation history indexing
- [x] Reservation cancellation logic
- [x] Table availability checking logic
- [ ] Verify and test reservation creation through `ReservationService`
- [ ] Complete VIP eligibility and reservation validation
- [ ] Reservation modification and rescheduling
- [ ] Additional unit test coverage
- [ ] SQLite database integration
- [ ] Console interface
- [ ] Linux build verification
- [ ] Automated release workflow (CD)

## Architecture

The system follows an object-oriented design, separating domain entities, reservation management logic, and future data persistence.

### Core Domain Entities

- **`Customer`** — represents a restaurant customer with a unique ID and VIP status.
- **`Table`** — abstract base class providing shared table properties and capacity validation.
- **`RegularTable`** — represents a standard restaurant table.
- **`VIPTable`** — extends `Table` with an additional reservation fee.
- **`Date` and `Time`** — provide validated date and time representations and comparison operations.
- **`Reservation`** — represents a reservation associated with a customer, table, date, time interval, and status.

### ReservationService

`ReservationService` coordinates reservation operations and manages the relationships between customers, tables, and reservations.

It uses STL containers for in-memory data management:

- `std::unordered_map` for customer, table, and reservation lookup by unique ID.
- `std::vector` for maintaining customer and table reservation history indexes.
- `std::unique_ptr` for ownership management of polymorphic table objects and reservation objects.

Reservation histories store reservation IDs rather than duplicate reservation objects.

This allows customer and table histories to reference the same reservation records while keeping reservation data in a central collection.

The service is being developed to handle:

- Customer and table registration
- Reservation creation and cancellation
- Table availability and overlapping time interval checks
- Customer and table reservation history retrieval
- VIP eligibility and other business rules

### Planned Persistence Layer

SQLite will provide persistent storage for customers, tables, and reservations.

The persistence layer will allow data to be retained between application executions instead of existing only in memory.

Customer and table reservation histories can be reconstructed from stored reservation records.

### Planned Console Interface

The console interface will provide user interaction with the reservation management system, including customer registration, table management, reservation operations, and history lookup.

## Status

**In Development**

Core domain entities, initial unit tests, and the GitHub Actions CI workflow are implemented.

The `ReservationService` component is under active development, with STL-based storage, lookup, reservation history indexing, cancellation, and availability checking logic added.

The next milestone is completing and testing reservation creation, validating business rules, and extending unit test coverage before introducing SQLite persistence.

## Author

**Andrii Nesterenko**
