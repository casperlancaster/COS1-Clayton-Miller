#include <iostream>
#include <string>
#include <limits>

#include "Truck.h"
#include "Driver.h"
#include "Load.h"
#include "Company.h"
#include "DispatchManager.h"

void ShowMainMenu()
{
    std::cout << "=============================================\n";
    std::cout << "               T R U C K Z S I M             \n";
    std::cout << "=============================================\n";
    std::cout << "         Trucking Company Simulator           \n";
    std::cout << "---------------------------------------------\n";
    std::cout << "1. Dispatch Center\n";
    std::cout << "2. Fleet Management\n";
    std::cout << "3. Driver Management\n";
    std::cout << "4. Freight Market\n";
    std::cout << "5. Company Overview\n";
    std::cout << "6. Banking\n";
    std::cout << "7. Maintenance\n";
    std::cout << "0. Exit\n";
    std::cout << "---------------------------------------------\n";
    std::cout << "Select an option: ";
}

int main()
{
    Company company;
    company.SetCompanyName("TruckzSim Logistics");

    DispatchManager dispatchManager(&company);

    int choice = -1;

    while (choice != 0)
    {
        ShowMainMenu();

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

            choice = -1;

            std::cout << "\nInvalid input. Please enter a number from 0 to 7.\n\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "\n";

        switch (choice)
        {
        case 1:
            if (dispatchManager.HasCompany())
            {
                std::cout
                    << "Dispatch Center connected to "
                    << dispatchManager.GetCompany()->GetCompanyName()
                    << ".\n";
            }
            else
            {
                std::cout << "Dispatch Center is not connected to a company.\n";
            }
            break;
            

        case 2:
            std::cout << "Fleet Management is under development.\n";
            break;

        case 3:
            std::cout << "Driver Management is under development.\n";
            break;

        case 4:
            std::cout << "Freight Market is under development.\n";
            break;

        case 5:
            std::cout << "========== COMPANY OVERVIEW ==========\n";
            std::cout << "Company: " << company.GetCompanyName() << "\n";
            std::cout << "Trucks: " << company.GetTrucks().size() << "\n";
            std::cout << "Drivers: " << company.GetDrivers().size() << "\n";
            std::cout << "Loads: " << company.GetLoads().size() << "\n";
            std::cout << "========================================\n";
            break;

        case 6:
            std::cout << "Banking is under development.\n";
            break;

        case 7:
            std::cout << "Maintenance is under development.\n";
            break;

        case 0:
            std::cout << "Exiting TruckzSim...\n";
            break;

        default:
            std::cout << "Invalid selection. Please try again.\n";
            break;
        }

        std::cout << "\n";
    }

    return 0;
}