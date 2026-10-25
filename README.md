# Restaurant Reservation Management System

A console-based restaurant reservation management system developed in **C++17**, with SQLite integration planned.

The project focuses on object-oriented programming, reservation management, data validation, and persistent storage. It is being developed as a practical C++ software engineering project.

## Features

### Implemented
- Date and time handling with validation and comparison operators
- Customer management with unique IDs and VIP status
- Restaurant table management using inheritance and polymorphism
- Regular and VIP tables with configurable capacity and availability status
- Reservation entities with unique IDs and status tracking
- Reservation time validation and overlap detection
- Reservation duration calculation

### Planned
- Reservation creation, modification, and cancellation through a management service
- Table availability checks and VIP eligibility validation
- Alternative time slot suggestions
- Customer search and reservation history
- SQLite database integration for persistent storage
- Console-based user interface
- Unit testing with GoogleTest

## Technology Stack

- **Language:** C++17
- **Build System:** CMake
- **Database:** SQLite (planned)
- **Testing:** GoogleTest (planned)
- **Development Tools:** Visual Studio 2026, Git, GitHub

## Project Structure

```text
restaurant-reservation-system/
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
├── tests/
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
- [ ] `ReservationService` — reservation management and availability checks
- [ ] SQLite database integration
- [ ] Console interface
- [ ] Automated unit tests with GoogleTest
- [ ] Linux build verification

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

Core domain entities are implemented, including customer, table, and reservation models.

The next development milestone is implementing `ReservationService` to connect these entities and introduce reservation management logic.

## Author

**Andrii Nesterenko**
