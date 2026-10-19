#include "TruckzSimApp.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "BankingManager.h"
#include "DispatchCenter.h"
#include "DriverManager.h"
#include "FleetManager.h"
#include "FreightManager.h"
#include "FuelManager.h"
#include "InputManager.h"
#include "MaintenanceManager.h"
#include "SaveLoadManager.h"
#include "SettingsManager.h"

TruckzSimApp::TruckzSimApp()
    : dispatchManager(&company)
{
    SettingsManager::ApplyDateTimeSettings(
        company
    );
}

bool TruckzSimApp::RunStartupMenu()
{
    int choice = -1;

    while (true)
    {
        std::cout
            << "=============================================\n";

        std::cout
            << "               T R U C K Z S I M             \n";

        std::cout
            << "=============================================\n";

        std::cout
            << "               COMPANY STARTUP                \n";

        std::cout
            << "---------------------------------------------\n";

        std::cout
            << "1. Create New Company\n";

        std::cout
            << "2. Load Saved Company\n";

        std::cout
            << "0. Exit\n";

        std::cout
            << "---------------------------------------------\n";

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                2
            );

        std::cout << "\n";

        switch (choice)
        {
        case 1:
            company =
                Company();

            dispatchManager.SetCompany(
                &company
            );

            SettingsManager::ApplyDateTimeSettings(
                company
            );

            SetupCompany();

            return true;

        case 2:
        {
            std::vector<std::string>
                savedCompanies =
                SaveLoadManager::
                GetSavedCompanies();

            if (savedCompanies.empty())
            {
                std::cout
                    << "No saved companies were found.\n"
                    << "Please create a new company first.\n\n";

                break;
            }

            std::cout
                << "========== SAVED COMPANIES ==========\n";

            for (std::size_t i = 0;
                i < savedCompanies.size();
                i++)
            {
                std::cout
                    << (i + 1)
                    << ". "
                    << savedCompanies[i]
                    << "\n";
            }

            std::cout
                << "0. Cancel\n"
                << "=====================================\n";

            int saveChoice =
                InputManager::ReadMenuChoice(
                    "Select a company: ",
                    0,
                    static_cast<int>(
                        savedCompanies.size()
                        )
                );

            if (saveChoice == 0)
            {
                std::cout << "\n";
                break;
            }

            std::string selectedCompany =
                savedCompanies[
                    saveChoice - 1
                ];

            if (SaveLoadManager::LoadCompany(
                company,
                selectedCompany))
            {
                dispatchManager.SetCompany(
                    &company
                );

                SettingsManager::ApplyDateTimeSettings(
                    company
                );

                std::cout
                    << "\nCompany loaded successfully.\n";

                std::cout
                    << "Welcome back to "
                    << company.GetCompanyName()
                    << "!\n\n";

                return true;
            }

            std::cout
                << "\nThe saved company could not be loaded.\n"
                << "The save file may be damaged or invalid.\n\n";

            break;
        }

        case 0:
            std::cout
                << "Exiting TruckzSim...\n";

            return false;
        }
    }
}

void TruckzSimApp::SetupCompany()
{
    std::cout
        << "========== COMPANY SETUP ==========\n";

    std::string companyName =
        InputManager::ReadRequiredText(
            "Enter your trucking company name: ",
            "Company name"
        );

    company.SetCompanyName(
        companyName
    );

    std::cout
        << "\nWelcome to "
        << company.GetCompanyName()
        << "!\n";

    std::cout
        << "===================================\n\n";
}

void TruckzSimApp::ShowMainMenu() const
{
    std::cout
        << "=============================================\n";

    std::cout
        << "               T R U C K Z S I M             \n";

    std::cout
        << "=============================================\n";

    std::cout
        << "         Trucking Company Simulator           \n";

    std::cout
        << "---------------------------------------------\n";

    std::cout << "1. Dispatch Center\n";
    std::cout << "2. Fleet Management\n";
    std::cout << "3. Driver Management\n";
    std::cout << "4. Freight Market\n";
    std::cout << "5. Company Overview\n";
    std::cout << "6. Banking\n";
    std::cout << "7. Maintenance\n";
    std::cout << "8. Fuel Management\n";
    std::cout << "9. Save Company\n";
    std::cout << "10. Settings\n";
    std::cout << "0. Exit\n";

    std::cout
        << "---------------------------------------------\n";
}

void TruckzSimApp::ShowCompanyOverview() const
{
    std::cout
        << "========== COMPANY OVERVIEW ==========\n";

    std::cout
        << "Company: "
        << company.GetCompanyName()
        << "\n";

    std::cout
        << "Trucks: "
        << company.GetTrucks().size()
        << "\n";

    std::cout
        << "Drivers: "
        << company.GetDrivers().size()
        << "\n";

    std::cout
        << "Loads: "
        << company.GetLoads().size()
        << "\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Revenue: $"
        << company.GetRevenue()
        << "\n";

    std::cout
        << "Expenses: $"
        << company.GetExpenses()
        << "\n";

    std::cout
        << "Balance: $"
        << company.GetBalance()
        << "\n";

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << "Financial Status: NEGATIVE BALANCE\n";
    }

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "========================================\n";
}

void TruckzSimApp::Run()
{
    if (!RunStartupMenu())
    {
        return;
    }

    int choice = -1;

    while (choice != 0)
    {
        ShowMainMenu();

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                10
            );

        std::cout << "\n";

        switch (choice)
        {
        case 1:
            DispatchCenter::Run(
                dispatchManager
            );
            break;

        case 2:
            FleetManager::Run(
                company
            );
            break;

        case 3:
            DriverManager::Run(
                company
            );
            break;

        case 4:
            FreightManager::Run(
                company
            );
            break;

        case 5:
            ShowCompanyOverview();
            break;

        case 6:
            BankingManager::Run(
                company
            );
            break;

        case 7:
            MaintenanceManager::Run(
                company
            );
            break;

        case 8:
            FuelManager::Run(
                company
            );
            break;

        case 9:
            if (SaveLoadManager::SaveCompany(
                company))
            {
                std::cout
                    << "Company saved successfully.\n";
            }
            else
            {
                std::cout
                    << "Unable to save company data.\n";
            }

            break;

        case 10:
            SettingsManager::Run(
                company
            );
            break;

        case 0:
            std::cout
                << "Exiting TruckzSim...\n";
            break;
        }

        std::cout << "\n";
    }
}
