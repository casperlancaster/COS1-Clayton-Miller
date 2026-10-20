#include "ConsoleTheme.h"
#include "MaintenanceManager.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "HOSManager.h"
#include "InputManager.h"
#include "Truck.h"

namespace
{
    constexpr double SERVICE_COST = 500.0;
}

void MaintenanceManager::ShowMenu()
{
    std::cout << ConsoleTheme::FormatLiteral("\n");
    std::cout << ConsoleTheme::FormatLiteral("========== MAINTENANCE CENTER ==========\n");
    std::cout << ConsoleTheme::FormatLiteral("1. View Maintenance Status\n");
    std::cout << ConsoleTheme::FormatLiteral("2. Send Truck to Maintenance\n");
    std::cout << ConsoleTheme::FormatLiteral("3. Complete Maintenance\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Return to Main Menu\n");
    std::cout << ConsoleTheme::FormatLiteral("========================================\n");
}

Truck* MaintenanceManager::FindTruck(
    Company& company,
    int unitNumber)
{
    for (Truck& truck : company.GetTrucks())
    {
        if (truck.GetUnitNumber() == unitNumber)
        {
            return &truck;
        }
    }

    return nullptr;
}

void MaintenanceManager::DisplayMaintenanceStatus(
    const Company& company)
{
    const std::vector<Truck>& trucks = company.GetTrucks();

    std::cout << ConsoleTheme::FormatLiteral("\n========== FLEET MAINTENANCE STATUS ==========\n");

    if (trucks.empty())
    {
        std::cout << ConsoleTheme::FormatLiteral("No trucks have been added yet.\n");
        std::cout << ConsoleTheme::FormatLiteral("==============================================\n");
        return;
    }

    for (const Truck& truck : trucks)
    {
        std::cout << ConsoleTheme::FormatLiteral("\nTruck #") << truck.GetUnitNumber() << ConsoleTheme::FormatLiteral("\n");
        std::cout << ConsoleTheme::FormatLiteral("Mileage: ") << truck.GetMileage() << ConsoleTheme::FormatLiteral("\n");
        std::cout << ConsoleTheme::FormatLiteral("Truck Status: ") << ConsoleTheme::Status(truck.GetStatus()) << ConsoleTheme::FormatLiteral("\n");
        std::cout << ConsoleTheme::FormatLiteral("Last Service Mileage: ")
            << truck.GetLastServiceMileage() << ConsoleTheme::FormatLiteral("\n");
        std::cout << ConsoleTheme::FormatLiteral("Next Service Due: ")
            << truck.GetNextServiceMileage() << ConsoleTheme::FormatLiteral(" miles\n");

        if (truck.GetAvailableAtEpoch() >
            HOSManager::GetCurrentEpoch())
        {
            std::cout << ConsoleTheme::FormatLiteral("Scheduled Available: ")
                << HOSManager::FormatDateTime(
                    truck.GetAvailableAtEpoch()
                )
                << ConsoleTheme::FormatLiteral("\n");
        }
        else
        {
            std::cout << ConsoleTheme::FormatLiteral("Scheduled Available: Now\n");
        }

        if (truck.GetStatus() == "Maintenance")
        {
            std::cout << ConsoleTheme::FormatLiteral("Maintenance Status: IN MAINTENANCE\n");
        }
        else if (truck.IsMaintenanceDue())
        {
            std::cout << ConsoleTheme::FormatLiteral("Maintenance Status: SERVICE DUE\n");
        }
        else
        {
            std::cout << ConsoleTheme::FormatLiteral("Maintenance Status: OK\n");
            std::cout << ConsoleTheme::FormatLiteral("Miles Until Service: ")
                << truck.GetMilesUntilService() << ConsoleTheme::FormatLiteral("\n");
        }
    }

    std::cout << ConsoleTheme::FormatLiteral("\nStandard Service Cost: $")
        << std::fixed
        << std::setprecision(2)
        << SERVICE_COST
        << std::defaultfloat
        << std::setprecision(6)
        << ConsoleTheme::FormatLiteral("\n");

    std::cout << ConsoleTheme::FormatLiteral("==============================================\n");
}

void MaintenanceManager::SendTruckToMaintenance(
    Company& company)
{
    if (company.GetTrucks().empty())
    {
        std::cout << ConsoleTheme::FormatLiteral("\nNo trucks are available for maintenance.\n");
        return;
    }

    DisplayMaintenanceStatus(company);

    int unitNumber = InputManager::ReadPositiveInt(
        "\nEnter the truck unit number to send to maintenance: ",
        "truck unit number"
    );

    Truck* selectedTruck = FindTruck(company, unitNumber);

    if (selectedTruck == nullptr)
    {
        std::cout << ConsoleTheme::FormatLiteral("Truck #") << unitNumber << ConsoleTheme::FormatLiteral(" does not exist.\n");
        return;
    }

    if (selectedTruck->GetStatus() == "Maintenance")
    {
        std::cout << ConsoleTheme::FormatLiteral("Truck #") << unitNumber
            << ConsoleTheme::FormatLiteral(" is already in maintenance.\n");
        return;
    }

    if (selectedTruck->GetStatus() != "Available")
    {
        std::cout << ConsoleTheme::FormatLiteral("Truck #") << unitNumber
            << ConsoleTheme::FormatLiteral(" is currently ") << ConsoleTheme::Status(selectedTruck->GetStatus())
            << ConsoleTheme::FormatLiteral(" and cannot be sent to maintenance.\n");
        return;
    }

    long long maintenanceStart =
        std::max(
            HOSManager::GetCurrentEpoch(),
            selectedTruck->GetAvailableAtEpoch()
        );

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== MAINTENANCE CONFIRMATION ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Truck: #")
        << unitNumber
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Current Mileage: ")
        << selectedTruck->GetMileage()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Service Status: ")
        << (selectedTruck->IsMaintenanceDue()
            ? "SERVICE DUE"
            : "PREVENTIVE / EARLY")
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Maintenance Scheduled For: ")
        << HOSManager::FormatDateTime(
            maintenanceStart
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("The truck will become unavailable for dispatch ")
        << ConsoleTheme::FormatLiteral("until maintenance is completed.\n");

    std::cout
        << ConsoleTheme::FormatLiteral("==============================================\n");

    if (!InputManager::ReadYesNo(
        "Send this truck to maintenance? (Y/N): "))
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Maintenance request cancelled. ")
            << ConsoleTheme::FormatLiteral("Truck status was not changed.\n");

        return;
    }

    selectedTruck->SetStatus("Maintenance");

    std::cout << ConsoleTheme::FormatLiteral("\nTruck #") << unitNumber
        << ConsoleTheme::FormatLiteral(" has been sent to maintenance.\n");

    std::cout << ConsoleTheme::FormatLiteral("Maintenance Scheduled For: ")
        << HOSManager::FormatDateTime(
            maintenanceStart
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("The truck is unavailable for dispatch ")
        << ConsoleTheme::FormatLiteral("until service is completed.\n");

    if (selectedTruck->IsMaintenanceDue())
    {
        std::cout << ConsoleTheme::FormatLiteral("This truck was due for scheduled service.\n");
    }
    else
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Preventive maintenance started before ")
            << ConsoleTheme::FormatLiteral("the service deadline.\n");
    }
}

void MaintenanceManager::CompleteTruckMaintenance(
    Company& company)
{
    bool foundTruckInMaintenance = false;

    std::cout << ConsoleTheme::FormatLiteral("\n========== TRUCKS IN MAINTENANCE ==========\n");

    for (const Truck& truck : company.GetTrucks())
    {
        if (truck.GetStatus() == "Maintenance")
        {
            foundTruckInMaintenance = true;

            std::cout << ConsoleTheme::FormatLiteral("Truck #") << truck.GetUnitNumber()
                << ConsoleTheme::FormatLiteral(" | Mileage: ") << truck.GetMileage()
                << ConsoleTheme::FormatLiteral("\n");
        }
    }

    if (!foundTruckInMaintenance)
    {
        std::cout << ConsoleTheme::FormatLiteral("No trucks are currently in maintenance.\n");
        std::cout << ConsoleTheme::FormatLiteral("===========================================\n");
        return;
    }

    std::cout << ConsoleTheme::FormatLiteral("===========================================\n");

    int unitNumber = InputManager::ReadPositiveInt(
        "\nEnter the truck unit number to complete maintenance: ",
        "truck unit number"
    );

    Truck* selectedTruck = FindTruck(company, unitNumber);

    if (selectedTruck == nullptr)
    {
        std::cout << ConsoleTheme::FormatLiteral("Truck #") << unitNumber << ConsoleTheme::FormatLiteral(" does not exist.\n");
        return;
    }

    if (selectedTruck->GetStatus() != "Maintenance")
    {
        std::cout << ConsoleTheme::FormatLiteral("Truck #") << unitNumber
            << ConsoleTheme::FormatLiteral(" is not currently in maintenance.\n");
        return;
    }

    bool wasScheduledService =
        selectedTruck->IsMaintenanceDue();

    long long maintenanceTime =
        std::max(
            HOSManager::GetCurrentEpoch(),
            selectedTruck->GetAvailableAtEpoch()
        );

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== SERVICE COMPLETION CONFIRMATION ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Truck: #")
        << unitNumber
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Service Time: ")
        << HOSManager::FormatDateTime(
            maintenanceTime
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Service Cost: $")
        << SERVICE_COST
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company Balance Before Service: $")
        << company.GetBalance()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Projected Balance After Service: $")
        << (company.GetBalance() - SERVICE_COST)
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("====================================================\n");

    if (!InputManager::ReadYesNo(
        "Complete maintenance and record the service expense? (Y/N): "))
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Maintenance completion cancelled. ")
            << ConsoleTheme::FormatLiteral("The truck remains in maintenance and no expense was recorded.\n");

        return;
    }

    selectedTruck->CompleteMaintenance();

    FinancialTransaction maintenanceTransaction;

    maintenanceTransaction.transactionEpoch =
        maintenanceTime;

    maintenanceTransaction.category =
        "Maintenance";

    maintenanceTransaction.description =
        wasScheduledService
        ? "Scheduled Maintenance Service"
        : "Preventive Maintenance Service";

    maintenanceTransaction.amount =
        SERVICE_COST;

    maintenanceTransaction.truckUnitNumber =
        unitNumber;

    company.RecordExpenseTransaction(
        maintenanceTransaction
    );

    std::cout << ConsoleTheme::FormatLiteral("\nMaintenance completed for Truck #")
        << unitNumber << ConsoleTheme::FormatLiteral(".\n");

    std::cout << ConsoleTheme::FormatLiteral("Service Time: ")
        << HOSManager::FormatDateTime(
            maintenanceTime
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout << ConsoleTheme::FormatLiteral("Service cost: $")
        << std::fixed
        << std::setprecision(2)
        << SERVICE_COST
        << std::defaultfloat
        << std::setprecision(6)
        << ConsoleTheme::FormatLiteral("\n");

    std::cout << ConsoleTheme::FormatLiteral("Truck #") << unitNumber
        << ConsoleTheme::FormatLiteral(" is now Available for dispatch.\n");

    std::cout << ConsoleTheme::FormatLiteral("Next service is due at ")
        << selectedTruck->GetNextServiceMileage()
        << ConsoleTheme::FormatLiteral(" miles.\n");
}

void MaintenanceManager::Run(Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();
        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                3
            );

        switch (choice)
        {
        case 1:
            DisplayMaintenanceStatus(company);
            break;

        case 2:
            SendTruckToMaintenance(company);
            break;

        case 3:
            CompleteTruckMaintenance(company);
            break;

        case 0:
            break;

        default:
            std::cout
                << ConsoleTheme::FormatLiteral("\nInvalid selection. Please choose 0, 1, 2, or 3.\n");
            break;
        }
    }
}