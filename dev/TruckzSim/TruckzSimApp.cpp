#include "TruckzSimApp.h"

#include <iomanip>
#include <iostream>
#include <string>

#include "BankingManager.h"
#include "DispatchCenter.h"
#include "DriverManager.h"
#include "FleetManager.h"
#include "FreightManager.h"
#include "InputManager.h"
#include "MaintenanceManager.h"

TruckzSimApp::TruckzSimApp()
    : dispatchManager(&company)
{
}

void TruckzSimApp::SetupCompany()
{
    std::cout << "========== COMPANY SETUP ==========\n";

    std::string companyName = InputManager::ReadRequiredText(
        "Enter your trucking company name: ",
        "Company name"
    );

    company.SetCompanyName(companyName);

    std::cout << "\nWelcome to " << company.GetCompanyName()
        << "!\n";
    std::cout << "===================================\n\n";
}

void TruckzSimApp::ShowMainMenu() const
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

void TruckzSimApp::ShowCompanyOverview() const
{
    std::cout << "========== COMPANY OVERVIEW ==========\n";
    std::cout << "Company: " << company.GetCompanyName() << "\n";
    std::cout << "Trucks: " << company.GetTrucks().size() << "\n";
    std::cout << "Drivers: " << company.GetDrivers().size() << "\n";
    std::cout << "Loads: " << company.GetLoads().size() << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Revenue: $" << company.GetRevenue() << "\n";
    std::cout << "Expenses: $" << company.GetExpenses() << "\n";
    std::cout << "Balance: $" << company.GetBalance() << "\n";
    std::cout << std::defaultfloat << std::setprecision(6);
    std::cout << "========================================\n";
}

void TruckzSimApp::Run()
{
    SetupCompany();

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
            ShowCompanyOverview();
            break;

        case 6:
            BankingManager::Run(company);
            break;

        case 7:
            MaintenanceManager::Run(company);
            break;

        case 0:
            std::cout << "Exiting TruckzSim...\n";
            break;

        default:
            std::cout
                << "Invalid selection. Please enter a number from 0 to 7.\n";
            break;
        }

        std::cout << "\n";
    }
}
