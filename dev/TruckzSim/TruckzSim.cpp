#include <iomanip>
#include <iostream>

#include "Company.h"
#include "DispatchManager.h"
#include "DispatchCenter.h"
#include "DriverManager.h"
#include "FleetManager.h"
#include "FreightManager.h"
#include "InputManager.h"


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
}


int main()
{
    Company company;

    std::cout << "========== COMPANY SETUP ==========\n";

    std::string companyName = InputManager::ReadRequiredText(
        "Enter your trucking company name: ",
        "Company name"
    );

    company.SetCompanyName(companyName);

    std::cout << "\nWelcome to " << company.GetCompanyName()
        << "!\n";
    std::cout << "===================================\n\n";

    DispatchManager dispatchManager(&company);

    int choice = -1;

    while (choice != 0)
    {
        ShowMainMenu();
        choice = InputManager::ReadInt("Select an option: ");

        std::cout << "\n";

        switch (choice)
        {
        case 1:
            DispatchCenter::Run(dispatchManager);
            break;

        case 2:
            FleetManager::Run(company);
            break;

        case 3:
            DriverManager::Run(company);
            break;

        case 4:
            FreightManager::Run(company);
            break;

        case 5:
            std::cout << "========== COMPANY OVERVIEW ==========\n";
            std::cout << "Company: " << company.GetCompanyName() << "\n";
            std::cout << "Trucks: " << company.GetTrucks().size() << "\n";
            std::cout << "Drivers: " << company.GetDrivers().size() << "\n";
            std::cout << "Loads: " << company.GetLoads().size() << "\n";
            std::cout << "Revenue: $"
                << std::fixed
                << std::setprecision(2)
                << company.GetRevenue()
                << std::defaultfloat
                << std::setprecision(6)
                << "\n";
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
            std::cout << "Invalid selection. Please enter a number from 0 to 7.\n";
            break;
        }

        std::cout << "\n";
    }

    return 0;
}
