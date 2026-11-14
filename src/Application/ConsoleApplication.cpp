#include "Application/ConsoleApplication.hpp"

void ConsoleApplication::RunMainMenu() 
{ 
    while (true)
    {
        ClearScreen();
        ShowMainMenu();

        std::string choice;

        if (!std::getline(std::cin, choice))
            break;

        if (choice == "1")
            RunCustomersMenu();
        else if (choice == "2")
            RunTablesMenu();
        else if (choice == "3")
            RunReservationsMenu();
        else if (choice == "0")
            break;
        else
        {
            std::cout << "\nInvalid option. Please try again.\n";
            Pause();
        }
    }

}

void ConsoleApplication::ShowMainMenu() const
{
    std::cout << "==================================================\n";
    std::cout << "          RESTAURANT RESERVATION SYSTEM\n";
    std::cout << "==================================================\n\n";

    std::cout << "                    MAIN MENU\n\n";

    std::cout << "             1. Customer Management\n";
    std::cout << "             2. Table Management\n";
    std::cout << "             3. Reservation Management\n\n";

    std::cout << "             0. Exit\n\n";

    std::cout << "--------------------------------------------------\n";
    std::cout << "Select an option: ";
}

void ConsoleApplication::ShowCustomersMenu() const
{
    std::cout << "==================================================\n";
    std::cout << "                CUSTOMER MANAGEMENT\n";
    std::cout << "==================================================\n\n";

    std::cout << "    1. Browse Customers\n";
    std::cout << "    2. Find Customer by ID\n";
    std::cout << "    3. Add New Customer\n";
    std::cout << "    4. View Customer Reservations\n";
    std::cout << "    5. Delete Customer\n\n";

    std::cout << "    0. Back to Main Menu\n\n";

    std::cout << "--------------------------------------------------\n";
    std::cout << "Select an option: ";
}

void ConsoleApplication::RunCustomersMenu()
{
    while (true)
    {
        ClearScreen();
        ShowCustomersMenu();

        std::string choice;

        if (!std::getline(std::cin, choice))
            return;

        if (choice == "1")
            ShowNotImplemented("Browse Customers");
        else if (choice == "2")
            ShowNotImplemented("Find Customer by ID");
        else if (choice == "3")
            ShowNotImplemented("Add New Customer");
        else if (choice == "4")
            ShowNotImplemented("View Customer Reservations");
        else if (choice == "5")
            ShowNotImplemented("Delete Customer");
        else if (choice == "0")
            return;
        else
        {
            std::cout << "\nInvalid option. Please try again.\n";
            Pause();
        }
    }
}

void ConsoleApplication::RunTablesMenu()
{
    while (true)
    {
        ClearScreen();
        ShowTablesMenu();

        std::string choice;

        if (!std::getline(std::cin, choice))
            return;

        if (choice == "1")
            ShowNotImplemented("Browse Tables");
        else if (choice == "2")
            ShowNotImplemented("Find Table by ID");
        else if (choice == "3")
            ShowNotImplemented("Add Regular Table");
        else if (choice == "4")
            ShowNotImplemented("Add VIP Table");
        else if (choice == "5")
            ShowNotImplemented("View Table Schedule");
        else if (choice == "6")
            ShowNotImplemented("View Available Time Slots");
        else if (choice == "7")
            ShowNotImplemented("Delete Table");
        else if (choice == "0")
            return;
        else
        {
            std::cout << "\nInvalid option. Please try again.\n";
            Pause();
        }
    }
}

void ConsoleApplication::ShowTablesMenu() const
{
    std::cout << "==================================================\n";
    std::cout << "                  TABLE MANAGEMENT\n";
    std::cout << "==================================================\n\n";

    std::cout << "    1. Browse Tables\n";
    std::cout << "    2. Find Table by ID\n";
    std::cout << "    3. Add Regular Table\n";
    std::cout << "    4. Add VIP Table\n";
    std::cout << "    5. View Table Schedule\n";
    std::cout << "    6. View Available Time Slots\n";
    std::cout << "    7. Delete Table\n\n";

    std::cout << "    0. Back to Main Menu\n\n";

    std::cout << "--------------------------------------------------\n";
    std::cout << "Select an option: ";
}

void ConsoleApplication::RunReservationsMenu()
{
    while (true)
    {
        ClearScreen();
        ShowReservationsMenu();

        std::string choice;

        if (!std::getline(std::cin, choice))
            return;

        if (choice == "1")
            ShowNotImplemented("Browse Reservations");
        else if (choice == "2")
            ShowNotImplemented("Find Reservation by ID");
        else if (choice == "3")
            ShowNotImplemented("Search Reservations by Date");
        else if (choice == "4")
            ShowNotImplemented("Search Reservations by Customer");
        else if (choice == "5")
            ShowNotImplemented("Search Reservations by Table");
        else if (choice == "6")
            ShowNotImplemented("Create Reservation");
        else if (choice == "7")
            ShowNotImplemented("Create Reservation Automatically");
        else if (choice == "8")
            ShowNotImplemented("Find Alternative Reservation Time");
        else if (choice == "9")
            RunModifyReservationMenu();
        else if (choice == "10")
            ShowNotImplemented("Cancel Reservation");
        else if (choice == "11")
            ShowNotImplemented("Delete Reservation");
        else if (choice == "0")
            return;
        else
        {
            std::cout << "\nInvalid option. Please try again.\n";
            Pause();
        }
    }
}

void ConsoleApplication::ShowReservationsMenu() const
{
    std::cout << "==================================================\n";
    std::cout << "               RESERVATION MANAGEMENT\n";
    std::cout << "==================================================\n\n";

    std::cout << "    1. Browse Reservations\n";
    std::cout << "    2. Find Reservation by ID\n";
    std::cout << "    3. Search Reservations by Date\n";
    std::cout << "    4. Search Reservations by Customer\n";
    std::cout << "    5. Search Reservations by Table\n\n";

    std::cout << "    6. Create Reservation\n";
    std::cout << "    7. Create Reservation Automatically\n";
    std::cout << "    8. Find Alternative Reservation Time\n\n";

    std::cout << "    9. Modify Reservation\n";
    std::cout << "    10. Cancel Reservation\n";
    std::cout << "    11. Delete Reservation\n\n";

    std::cout << "    0. Back to Main Menu\n\n";

    std::cout << "--------------------------------------------------\n";
    std::cout << "Select an option: ";
}

void ConsoleApplication::ShowModifyReservationMenu() const
{
    std::cout << "==================================================\n";
    std::cout << "                MODIFY RESERVATION\n";
    std::cout << "==================================================\n\n";

    std::cout << "    1. Change Date and Time\n";
    std::cout << "    2. Change Customer\n";
    std::cout << "    3. Change Table\n";
    std::cout << "    4. Change Guest Count\n\n";

    std::cout << "    0. Back to Reservations Menu\n\n";

    std::cout << "--------------------------------------------------\n";
    std::cout << "Select an option: ";
}

void ConsoleApplication::RunModifyReservationMenu()
{
    while (true)
    {
        ClearScreen();
        ShowModifyReservationMenu();

        std::string choice;

        if (!std::getline(std::cin, choice))
            return;

        if (choice == "1")
            ShowNotImplemented("Change Reservation Date and Time");
        else if (choice == "2")
            ShowNotImplemented("Change Reservation Customer");
        else if (choice == "3")
            ShowNotImplemented("Change Reservation Table");
        else if (choice == "4")
            ShowNotImplemented("Change Reservation Guest Count");
        else if (choice == "0")
            return;
        else
        {
            std::cout << "\nInvalid option. Please try again.\n";
            Pause();
        }
    }
}

void ConsoleApplication::ClearScreen() const
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void ConsoleApplication::Pause() const
{
    std::cout << "\nPress Enter to continue...";

    std::string input;
    std::getline(std::cin, input);
}

void ConsoleApplication::ShowNotImplemented(const std::string& feature) const
{
    std::cout << "\n" << feature << " is not implemented yet.\n";
    Pause();
}

void ConsoleApplication::Run() 
{ 
    RunMainMenu(); 
}
