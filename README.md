# Restaurant Reservation Management System

A console-based restaurant reservation management application built with **C++17** and **SQLite**.

The project focuses on designing a reliable reservation system with table availability checks, booking management, persistent data storage, and well-structured object-oriented C++ code.

## Project Goals

The main goal is to build a practical C++ application that demonstrates software engineering fundamentals, including:

- Object-oriented design and separation of responsibilities
- Modern C++17 and the Standard Template Library (STL)
- Data structures, algorithms, and input validation
- SQL database integration and persistent storage
- Error handling and edge-case management
- Unit testing and debugging
- Cross-platform builds using CMake

The application is intended to simulate core reservation management operations used by restaurant staff.

## Planned Features

### 1. Table Management

- Maintain a collection of restaurant tables with unique identifiers and seating capacities.
- Find tables that can accommodate a specified number of guests.
- Check table availability for a requested date and time.
- Prevent multiple reservations from occupying the same table during overlapping time intervals.

### 2. Reservation Management

- Create reservations for a selected date, time, and party size.
- Assign an available table with sufficient seating capacity.
- Modify existing reservations, including date, time, party size, or assigned table.
- Cancel reservations.
- Assign unique identifiers to reservations.
- Validate reservation details before saving changes.

### 3. Customer Management

- Store basic customer information associated with reservations.
- Search for customers and their reservations by name or phone number.
- Display current and past reservations for a customer.

### 4. Availability and Scheduling

- Detect overlapping reservations using their start and end times.
- Account for reservation duration when checking availability.
- Suggest alternative available tables or time slots when the requested reservation cannot be accommodated.
- Prioritize suitable alternatives on the requested date before suggesting another date.

### 5. Persistent Data Storage

- Use SQLite to store customers, tables, and reservations.
- Design a relational database schema with appropriate primary and foreign keys.
- Implement basic CRUD operations.
- Preserve reservation data between application runs.
- Use SQL transactions where multiple database changes must succeed together.

### 6. Console Interface

- Provide a menu-driven command-line interface.
- Allow users to manage reservations, customers, and table availability.
- Display clear confirmation messages and useful error descriptions.
- Handle invalid user input without unexpectedly terminating the application.

### 7. Testing and Reliability

- Write unit tests using GoogleTest.
- Test table capacity validation and reservation overlap detection.
- Cover edge cases such as invalid dates, unavailable tables, and conflicting reservations.
- Keep business logic separate from the console interface and database access.

## Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Core application and business logic |
| STL | Containers, algorithms, and utilities |
| SQLite | Relational database and persistent storage |
| SQL | Database queries and data manipulation |
| CMake | Project configuration and builds |
| GoogleTest | Unit testing |
| Git & GitHub | Version control |
| Visual Studio | Development and debugging |
| Linux | Cross-platform build and testing |

## Planned Project Structure

```text
restaurant-reservation-system/
├── include/             # Class declarations and interfaces
├── src/                 # C++ implementation files
│   └── main.cpp
├── tests/               # Unit tests
├── database/            # Database schema and related SQL files
├── CMakeLists.txt       # Build configuration
├── .gitignore
└── README.md
```

The structure will evolve as the application is developed.

## Core Domain Model

The initial design is based on three main entities:

**Customer**
- Unique customer ID
- Name
- Phone number

**Table**
- Unique table ID
- Seating capacity

**Reservation**
- Unique reservation ID
- Customer ID
- Table ID
- Reservation date and time
- Duration
- Number of guests
- Reservation status

Reservation-related business rules will be handled separately from database access and user interaction.

## Development Roadmap

- [x] Initialize the GitHub repository
- [x] Configure the C++17 project with CMake
- [ ] Define the core domain classes
- [ ] Implement reservation validation and overlap detection
- [ ] Implement table availability and allocation
- [ ] Add reservation creation, modification, and cancellation
- [ ] Implement customer search and reservation history
- [ ] Design and integrate the SQLite database
- [ ] Add a menu-driven console interface
- [ ] Implement unit tests with GoogleTest
- [ ] Verify the build on Linux
- [ ] Finalize documentation and test coverage

## Project Status

**In Development**

The repository is currently in its initial setup stage. Features will be implemented incrementally, with an emphasis on maintainable code, correctness, and testability.

## Author

**Andrii Nesterenko**

Computer Science and Information Systems student at Warsaw University of Technology.

[LinkedIn](https://www.linkedin.com/in/andrii-nesterenko-621923408)
