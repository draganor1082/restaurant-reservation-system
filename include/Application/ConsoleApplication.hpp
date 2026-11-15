#pragma once
#include "Services/ReservationService.hpp"

class ConsoleApplication
{
   private:
    ReservationService _reservationService;

    // Application control
    void RunMainMenu();

    // Menu menu
    void ShowMainMenu() const;

    // Customer management
    void ShowCustomersMenu() const;
    void RunCustomersMenu();

    // Customer operations
    void FindCustomerById();
    void AddNewCustomer();
    void ViewCustomerReservations();
    void DeleteCustomer();

    // Table management
    void ShowTablesMenu() const;
    void RunTablesMenu();

    // Reservation management
    void ShowReservationsMenu() const;
    void RunReservationsMenu();

    // Modify reservation submenu
    void ShowModifyReservationMenu() const;
    void RunModifyReservationMenu();

    // UI utilities
    void ClearScreen() const;
    void Pause() const;
    void ShowNotImplemented(const std::string& feature) const;

    // Input utilities
    bool ReadUnsignedInt(const std::string& prompt, unsigned int& value) const;
    bool ConfirmAction(const std::string& message) const;

    // Display utilities
    void PrintCustomer(const Customer& customer) const;

   public:
    void Run();
};
