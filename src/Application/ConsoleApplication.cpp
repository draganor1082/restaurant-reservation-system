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

    std::cout << "    1. Find Customer by ID\n";
    std::cout << "    2. Add New Customer\n\n";

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
            FindCustomerById();
        else if (choice == "2")
            AddNewCustomer();
        else if (choice == "0")
            return;
        else
        {
            std::cout << "\nInvalid option. Please try again.\n";
            Pause();
        }
    }
}

void ConsoleApplication::FindCustomerById()
{
    ClearScreen();

    std::cout << "========== FIND CUSTOMER ==========\n\n";

    unsigned int customerId;

    if (!ReadUnsignedInt("Enter customer ID: ", customerId))
        return;

    const Customer* customer = _reservationService.GetCustomer(customerId);

    if (customer == nullptr)
    {
        std::cout << "\nCustomer not found.\n";
    }
    else
    {
        std::cout << "\nCustomer found:\n\n";
        PrintCustomer(*customer);
    }

    Pause();
}

void ConsoleApplication::AddNewCustomer()
{
    ClearScreen();

    std::cout << "========== ADD NEW CUSTOMER ==========\n\n";

    std::string name;
    std::string phone;

    std::cout << "Enter customer name: ";

    if (!std::getline(std::cin, name))
        return;

    std::cout << "Enter phone number: ";

    if (!std::getline(std::cin, phone))
        return;

    if (name.find_first_not_of(" \t") == std::string::npos || phone.find_first_not_of(" \t") == std::string::npos)
    {
        std::cout << "\nName and phone number cannot be empty.\n";
        Pause();
        return;
    }

    bool isVIP = ConfirmAction("Is this a VIP customer?");

    Customer customer(name, phone, isVIP);

    unsigned int customerId = customer.GetId();

    if (_reservationService.AddCustomer(customer))
    {
        std::cout << "\nCustomer successfully added!\n";
        std::cout << "Assigned ID: " << customerId << '\n';
    }
    else
    {
        std::cout << "\nFailed to add customer.\n";
    }

    Pause();
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

bool ConsoleApplication::ReadUnsignedInt(const std::string& prompt, unsigned int& value) const
{
    std::cout << prompt;

    std::string input;

    if (!std::getline(std::cin, input))
        return false;

    auto [ptr, error] = std::from_chars(input.data(), input.data() + input.size(), value);

    if (error != std::errc{} || ptr != input.data() + input.size())
    {
        std::cout << "\nInvalid number.\n";
        Pause();
        return false;
    }

    return true;
}

bool ConsoleApplication::ConfirmAction(const std::string& message) const
{
    while (true)
    {
        std::cout << message << " (y/n): ";

        std::string choice;

        if (!std::getline(std::cin, choice))
            return false;

        if (choice == "y" || choice == "Y")
            return true;

        if (choice == "n" || choice == "N")
            return false;

        std::cout << "Please enter y or n.\n";
    }
}

void ConsoleApplication::PrintCustomer(const Customer& customer) const
{
    std::cout << "ID: " << customer.GetId() << '\n';
    std::cout << "Name: " << customer.GetName() << '\n';
    std::cout << "Phone: " << customer.GetPhone() << '\n';
    std::cout << "VIP: " << (customer.IsVIP() ? "Yes" : "No") << '\n';
}

void ConsoleApplication::Run() { RunMainMenu(); }
