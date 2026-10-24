# Restaurant Reservation Management System

A console-based restaurant reservation system developed in **C++17**, with SQLite integration planned.

The project focuses on object-oriented design, reservation management, data validation, and persistent storage.

## Features

- Customer and restaurant table management
- Reservation creation, modification, and cancellation
- Table availability and reservation overlap detection
- Alternative time slot suggestions
- Customer search and reservation history
- SQLite database integration
- Console-based user interface
- Unit testing with GoogleTest

## Technology Stack

- **Language:** C++17
- **Build System:** CMake
- **Database:** SQLite (planned)
- **Testing:** GoogleTest (planned)
- **Development:** Visual Studio 2026, Git, GitHub

## Project Structure

```text
restaurant-reservation-system/
├── include/
│   ├── Customer.hpp
│   ├── Date.hpp
│   ├── Time.hpp
│   └── pch.hpp
├── src/
│   ├── Date.cpp
│   ├── Time.cpp
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
- [x] `Time` class
- [ ] Core entities: `Customer`, `Table`, `Reservation`
- [ ] Reservation management and availability checks
- [ ] SQLite database integration
- [ ] Console interface
- [ ] Unit tests
- [ ] Linux build verification

## Status

**In Development**

Date and time utilities are implemented. Next step: core domain classes and reservation logic.

## Author

**Andrii Nesterenko**
