#include "ConsoleTheme.h"
#include "TruckzSimApp.h"

#include <iomanip>
#include <iostream>
#include <limits>
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
#include "ValidationManager.h"

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
            << ConsoleTheme::FormatLiteral("=============================================\n");

        std::cout
            << ConsoleTheme::FormatLiteral("               T R U C K Z S I M             \n");

        std::cout
            << ConsoleTheme::FormatLiteral("=============================================\n");

        std::cout
            << ConsoleTheme::FormatLiteral("               COMPANY STARTUP                \n");

        std::cout
            << ConsoleTheme::FormatLiteral("---------------------------------------------\n");

        std::cout
            << ConsoleTheme::FormatLiteral("1. Create New Company\n");

        std::cout
            << ConsoleTheme::FormatLiteral("2. Load Saved Company\n");

        std::cout
            << ConsoleTheme::FormatLiteral("0. Exit\n");

        std::cout
            << ConsoleTheme::FormatLiteral("---------------------------------------------\n");

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                2
            );

        std::cout << ConsoleTheme::FormatLiteral("\n");

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
                    << ConsoleTheme::FormatLiteral("No saved companies were found.\n")
                    << ConsoleTheme::FormatLiteral("Please create a new company first.\n\n");

                break;
            }

            std::cout
                << ConsoleTheme::FormatLiteral("========== SAVED COMPANIES ==========\n");

            for (std::size_t i = 0;
                i < savedCompanies.size();
                i++)
            {
                std::cout
                    << (i + 1)
                    << ConsoleTheme::FormatLiteral(". ")
                    << savedCompanies[i]
                    << ConsoleTheme::FormatLiteral("\n");
            }

            std::cout
                << ConsoleTheme::FormatLiteral("0. Cancel\n")
                << ConsoleTheme::FormatLiteral("=====================================\n");

            // ReadMenuChoice expects an int upper bound. Cap the
            // size_t count before casting, avoiding MSVC warning C4267.
            const int maxCompanyChoice =
                savedCompanies.size() >
                static_cast<std::size_t>(std::numeric_limits<int>::max())
                ? std::numeric_limits<int>::max()
                : static_cast<int>(savedCompanies.size());

            int saveChoice =
                InputManager::ReadMenuChoice(
                    "Select a company: ",
                    0,
                    maxCompanyChoice
                );

            if (saveChoice == 0)
            {
                std::cout << ConsoleTheme::FormatLiteral("\n");
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
                std::vector<std::string>
                    validationIssues;

                if (!ValidationManager::ValidateCompany(
                    company,
                    validationIssues))
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("\nThe saved company failed system ")
                        << ConsoleTheme::FormatLiteral("validation and was not opened.\n");

                    for (std::size_t i = 0;
                        i < validationIssues.size();
                        i++)
                    {
                        std::cout
                            << (i + 1)
                            << ConsoleTheme::FormatLiteral(". ")
                            << validationIssues[i]
                            << ConsoleTheme::FormatLiteral("\n");
                    }

                    company =
                        Company();

                    dispatchManager.SetCompany(
                        &company
                    );

                    SettingsManager::
                        ApplyDateTimeSettings(
                            company
                        );

                    std::cout << ConsoleTheme::FormatLiteral("\n");

                    break;
                }

                dispatchManager.SetCompany(
                    &company
                );

                SettingsManager::ApplyDateTimeSettings(
                    company
                );

                std::cout
                    << ConsoleTheme::FormatLiteral("\nCompany loaded successfully.\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Welcome back to ")
                    << company.GetCompanyName()
                    << ConsoleTheme::FormatLiteral("!\n\n");

                return true;
            }

            std::cout
                << ConsoleTheme::FormatLiteral("\nThe saved company could not be loaded.\n")
                << ConsoleTheme::FormatLiteral("The save file may be damaged or invalid.\n\n");

            break;
        }

        case 0:
            std::cout
                << ConsoleTheme::FormatLiteral("Exiting TruckzSim...\n");

            return false;
        }
    }
}

void TruckzSimApp::SetupCompany()
{
    std::cout
        << ConsoleTheme::FormatLiteral("========== COMPANY SETUP ==========\n");

    std::string companyName =
        InputManager::ReadRequiredText(
            "Enter your trucking company name: ",
            "Company name"
        );

    company.SetCompanyName(
        companyName
    );

    std::cout
        << ConsoleTheme::FormatLiteral("\nWelcome to ")
        << company.GetCompanyName()
        << ConsoleTheme::FormatLiteral("!\n");

    std::cout
        << ConsoleTheme::FormatLiteral("===================================\n\n");
}

void TruckzSimApp::ShowMainMenu() const
{
    std::cout
        << ConsoleTheme::FormatLiteral("=============================================\n");

    std::cout
        << ConsoleTheme::FormatLiteral("               T R U C K Z S I M             \n");

    std::cout
        << ConsoleTheme::FormatLiteral("=============================================\n");

    std::cout
        << ConsoleTheme::FormatLiteral("         Trucking Company Simulator           \n");

    std::cout
        << ConsoleTheme::FormatLiteral("---------------------------------------------\n");

    std::cout << ConsoleTheme::FormatLiteral("1. Dispatch Center\n");
    std::cout << ConsoleTheme::FormatLiteral("2. Fleet Management\n");
    std::cout << ConsoleTheme::FormatLiteral("3. Driver Management\n");
    std::cout << ConsoleTheme::FormatLiteral("4. Freight Market\n");
    std::cout << ConsoleTheme::FormatLiteral("5. Company Overview\n");
    std::cout << ConsoleTheme::FormatLiteral("6. Banking\n");
    std::cout << ConsoleTheme::FormatLiteral("7. Maintenance\n");
    std::cout << ConsoleTheme::FormatLiteral("8. Fuel Management\n");
    std::cout << ConsoleTheme::FormatLiteral("9. Save Company\n");
    std::cout << ConsoleTheme::FormatLiteral("10. Settings\n");
    std::cout << ConsoleTheme::FormatLiteral("11. System Validation\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Exit\n");

    std::cout
        << ConsoleTheme::FormatLiteral("---------------------------------------------\n");
}

void TruckzSimApp::ShowCompanyOverview() const
{
    int availableTrucks = 0;
    int dispatchedTrucks = 0;
    int maintenanceTrucks = 0;
    int breakdownTrucks = 0;
    int maintenanceDueTrucks = 0;

    for (const Truck& truck :
        company.GetTrucks())
    {
        if (truck.GetStatus() ==
            "Available")
        {
            availableTrucks++;
        }
        else if (truck.GetStatus() ==
            "Dispatched")
        {
            dispatchedTrucks++;
        }
        else if (truck.GetStatus() ==
            "Maintenance")
        {
            maintenanceTrucks++;
        }
        else if (truck.GetStatus() ==
            "Breakdown")
        {
            breakdownTrucks++;
        }

        if (truck.IsMaintenanceDue())
        {
            maintenanceDueTrucks++;
        }
    }

    int availableDrivers = 0;
    int dispatchedDrivers = 0;

    for (const Driver& driver :
        company.GetDrivers())
    {
        if (driver.GetStatus() ==
            "Available")
        {
            availableDrivers++;
        }
        else if (driver.GetStatus() ==
            "Dispatched")
        {
            dispatchedDrivers++;
        }
    }

    int availableLoads = 0;
    int assignedLoads = 0;
    int completedLoads = 0;

    for (const Load& load :
        company.GetLoads())
    {
        if (load.GetStatus() ==
            "Available")
        {
            availableLoads++;
        }
        else if (load.GetStatus() ==
            "Assigned")
        {
            assignedLoads++;
        }
        else if (load.GetStatus() ==
            "Completed")
        {
            completedLoads++;
        }
    }

    std::cout
        << ConsoleTheme::FormatLiteral("=============== COMPANY OVERVIEW ===============\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company: ")
        << company.GetCompanyName()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("\nFLEET\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Total Trucks: ")
        << company.GetTrucks().size()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Available: ")
        << availableTrucks
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Dispatched: ")
        << dispatchedTrucks
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("In Maintenance: ")
        << maintenanceTrucks
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Breakdown: ")
        << breakdownTrucks
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Maintenance Due: ")
        << maintenanceDueTrucks
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("\nDRIVERS\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Total Drivers: ")
        << company.GetDrivers().size()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Available: ")
        << availableDrivers
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Dispatched: ")
        << dispatchedDrivers
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("\nFREIGHT\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Total Loads: ")
        << company.GetLoads().size()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Available: ")
        << availableLoads
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Active / Assigned: ")
        << assignedLoads
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Completed: ")
        << completedLoads
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("\nFINANCIALS\n")
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Revenue:  $")
        << ConsoleTheme::Begin(ConsoleTheme::Color::Lime)
        << company.GetRevenue()
        << ConsoleTheme::End()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Expenses: $")
        << ConsoleTheme::Begin(ConsoleTheme::Color::Yellow)
        << company.GetExpenses()
        << ConsoleTheme::End()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Balance:  $")
        << ConsoleTheme::Begin(company.GetBalance() < 0.0
            ? ConsoleTheme::Color::Red : ConsoleTheme::Color::Lime)
        << company.GetBalance()
        << ConsoleTheme::End()
        << ConsoleTheme::FormatLiteral("\n");

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Financial Status: NEGATIVE BALANCE\n");
    }
    else
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Financial Status: POSITIVE / BALANCED\n");
    }

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("\nTRUCK LOCATIONS\n");

    if (company.GetTrucks().empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No trucks have been added yet.\n");
    }
    else
    {
        for (const Truck& truck :
            company.GetTrucks())
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Truck #")
                << truck.GetUnitNumber()
                << ConsoleTheme::FormatLiteral(" - ")
                << truck.GetCurrentLocation()
                << ConsoleTheme::FormatLiteral(" [")
                << ConsoleTheme::Status(truck.GetStatus())
                << ConsoleTheme::FormatLiteral("]\n");
        }
    }

    std::cout
        << ConsoleTheme::FormatLiteral("================================================\n");
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
                11
            );

        std::cout << ConsoleTheme::FormatLiteral("\n");

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
        {
            std::vector<std::string>
                validationIssues;

            if (!ValidationManager::ValidateCompany(
                company,
                validationIssues))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Company save blocked because ")
                    << ConsoleTheme::FormatLiteral("system validation failed.\n");

                for (std::size_t i = 0;
                    i < validationIssues.size();
                    i++)
                {
                    std::cout
                        << (i + 1)
                        << ConsoleTheme::FormatLiteral(". ")
                        << validationIssues[i]
                        << ConsoleTheme::FormatLiteral("\n");
                }

                break;
            }

            if (SaveLoadManager::SaveCompany(
                company))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Company saved successfully.\n");
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Unable to save company data.\n");
            }

            break;
        }

        case 10:
            SettingsManager::Run(
                company
            );
            break;

        case 11:
            ValidationManager::DisplayReport(
                company
            );
            break;

        case 0:
            if (InputManager::ReadYesNo(
                "Exit TruckzSim? Unsaved changes are not "
                "saved automatically. (Y/N): "))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Exiting TruckzSim...\n");
            }
            else
            {
                choice = -1;

                std::cout
                    << ConsoleTheme::FormatLiteral("Exit cancelled. ")
                    << ConsoleTheme::FormatLiteral("Returning to the main menu.\n");
            }
            break;
        }

        std::cout << ConsoleTheme::FormatLiteral("\n");
    }
}