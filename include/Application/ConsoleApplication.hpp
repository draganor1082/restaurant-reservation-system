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

   public:

    void Run();

    
};
