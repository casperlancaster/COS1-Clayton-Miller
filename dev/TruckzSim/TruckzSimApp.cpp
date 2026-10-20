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
                std::vector<std::string>
                    validationIssues;

                if (!ValidationManager::ValidateCompany(
                    company,
                    validationIssues))
                {
                    std::cout
                        << "\nThe saved company failed system "
                        << "validation and was not opened.\n";

                    for (std::size_t i = 0;
                        i < validationIssues.size();
                        i++)
                    {
                        std::cout
                            << (i + 1)
                            << ". "
                            << validationIssues[i]
                            << "\n";
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

                    std::cout << "\n";

                    break;
                }

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
    std::cout << "11. System Validation\n";
    std::cout << "0. Exit\n";

    std::cout
        << "---------------------------------------------\n";
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
        << "=============== COMPANY OVERVIEW ===============\n";

    std::cout
        << "Company: "
        << company.GetCompanyName()
        << "\n";

    std::cout
        << "\nFLEET\n";

    std::cout
        << "Total Trucks: "
        << company.GetTrucks().size()
        << "\n";

    std::cout
        << "Available: "
        << availableTrucks
        << "\n";

    std::cout
        << "Dispatched: "
        << dispatchedTrucks
        << "\n";

    std::cout
        << "In Maintenance: "
        << maintenanceTrucks
        << "\n";

    std::cout
        << "Breakdown: "
        << breakdownTrucks
        << "\n";

    std::cout
        << "Maintenance Due: "
        << maintenanceDueTrucks
        << "\n";

    std::cout
        << "\nDRIVERS\n";

    std::cout
        << "Total Drivers: "
        << company.GetDrivers().size()
        << "\n";

    std::cout
        << "Available: "
        << availableDrivers
        << "\n";

    std::cout
        << "Dispatched: "
        << dispatchedDrivers
        << "\n";

    std::cout
        << "\nFREIGHT\n";

    std::cout
        << "Total Loads: "
        << company.GetLoads().size()
        << "\n";

    std::cout
        << "Available: "
        << availableLoads
        << "\n";

    std::cout
        << "Active / Assigned: "
        << assignedLoads
        << "\n";

    std::cout
        << "Completed: "
        << completedLoads
        << "\n";

    std::cout
        << "\nFINANCIALS\n"
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Revenue:  $"
        << company.GetRevenue()
        << "\n";

    std::cout
        << "Expenses: $"
        << company.GetExpenses()
        << "\n";

    std::cout
        << "Balance:  $"
        << company.GetBalance()
        << "\n";

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << "Financial Status: NEGATIVE BALANCE\n";
    }
    else
    {
        std::cout
            << "Financial Status: POSITIVE / BALANCED\n";
    }

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "\nTRUCK LOCATIONS\n";

    if (company.GetTrucks().empty())
    {
        std::cout
            << "No trucks have been added yet.\n";
    }
    else
    {
        for (const Truck& truck :
            company.GetTrucks())
        {
            std::cout
                << "Truck #"
                << truck.GetUnitNumber()
                << " - "
                << truck.GetCurrentLocation()
                << " ["
                << truck.GetStatus()
                << "]\n";
        }
    }

    std::cout
        << "================================================\n";
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
        {
            std::vector<std::string>
                validationIssues;

            if (!ValidationManager::ValidateCompany(
                company,
                validationIssues))
            {
                std::cout
                    << "Company save blocked because "
                    << "system validation failed.\n";

                for (std::size_t i = 0;
                    i < validationIssues.size();
                    i++)
                {
                    std::cout
                        << (i + 1)
                        << ". "
                        << validationIssues[i]
                        << "\n";
                }

                break;
            }

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
            std::cout
                << "Exiting TruckzSim...\n";
            break;
        }

        std::cout << "\n";
    }
}