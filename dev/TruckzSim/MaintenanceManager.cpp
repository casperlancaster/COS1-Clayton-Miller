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
    std::cout << "\n";
    std::cout << "========== MAINTENANCE CENTER ==========\n";
    std::cout << "1. View Maintenance Status\n";
    std::cout << "2. Send Truck to Maintenance\n";
    std::cout << "3. Complete Maintenance\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "========================================\n";
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

    std::cout << "\n========== FLEET MAINTENANCE STATUS ==========\n";

    if (trucks.empty())
    {
        std::cout << "No trucks have been added yet.\n";
        std::cout << "==============================================\n";
        return;
    }

    for (const Truck& truck : trucks)
    {
        std::cout << "\nTruck #" << truck.GetUnitNumber() << "\n";
        std::cout << "Mileage: " << truck.GetMileage() << "\n";
        std::cout << "Truck Status: " << truck.GetStatus() << "\n";
        std::cout << "Last Service Mileage: "
            << truck.GetLastServiceMileage() << "\n";
        std::cout << "Next Service Due: "
            << truck.GetNextServiceMileage() << " miles\n";

        if (truck.GetAvailableAtEpoch() >
            HOSManager::GetCurrentEpoch())
        {
            std::cout << "Scheduled Available: "
                << HOSManager::FormatDateTime(
                    truck.GetAvailableAtEpoch()
                )
                << "\n";
        }
        else
        {
            std::cout << "Scheduled Available: Now\n";
        }

        if (truck.GetStatus() == "Maintenance")
        {
            std::cout << "Maintenance Status: IN MAINTENANCE\n";
        }
        else if (truck.IsMaintenanceDue())
        {
            std::cout << "Maintenance Status: SERVICE DUE\n";
        }
        else
        {
            std::cout << "Maintenance Status: OK\n";
            std::cout << "Miles Until Service: "
                << truck.GetMilesUntilService() << "\n";
        }
    }

    std::cout << "\nStandard Service Cost: $"
        << std::fixed
        << std::setprecision(2)
        << SERVICE_COST
        << std::defaultfloat
        << std::setprecision(6)
        << "\n";

    std::cout << "==============================================\n";
}

void MaintenanceManager::SendTruckToMaintenance(
    Company& company)
{
    if (company.GetTrucks().empty())
    {
        std::cout << "\nNo trucks are available for maintenance.\n";
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
        std::cout << "Truck #" << unitNumber << " does not exist.\n";
        return;
    }

    if (selectedTruck->GetStatus() == "Maintenance")
    {
        std::cout << "Truck #" << unitNumber
            << " is already in maintenance.\n";
        return;
    }

    if (selectedTruck->GetStatus() != "Available")
    {
        std::cout << "Truck #" << unitNumber
            << " is currently " << selectedTruck->GetStatus()
            << " and cannot be sent to maintenance.\n";
        return;
    }

    long long maintenanceStart =
        std::max(
            HOSManager::GetCurrentEpoch(),
            selectedTruck->GetAvailableAtEpoch()
        );

    std::cout
        << "\n========== MAINTENANCE CONFIRMATION ==========\n";

    std::cout
        << "Truck: #"
        << unitNumber
        << "\n";

    std::cout
        << "Current Mileage: "
        << selectedTruck->GetMileage()
        << "\n";

    std::cout
        << "Service Status: "
        << (selectedTruck->IsMaintenanceDue()
            ? "SERVICE DUE"
            : "PREVENTIVE / EARLY")
        << "\n";

    std::cout
        << "Maintenance Scheduled For: "
        << HOSManager::FormatDateTime(
            maintenanceStart
        )
        << "\n";

    std::cout
        << "The truck will become unavailable for dispatch "
        << "until maintenance is completed.\n";

    std::cout
        << "==============================================\n";

    if (!InputManager::ReadYesNo(
        "Send this truck to maintenance? (Y/N): "))
    {
        std::cout
            << "Maintenance request cancelled. "
            << "Truck status was not changed.\n";

        return;
    }

    selectedTruck->SetStatus("Maintenance");

    std::cout << "\nTruck #" << unitNumber
        << " has been sent to maintenance.\n";

    std::cout << "Maintenance Scheduled For: "
        << HOSManager::FormatDateTime(
            maintenanceStart
        )
        << "\n";

    std::cout
        << "The truck is unavailable for dispatch "
        << "until service is completed.\n";

    if (selectedTruck->IsMaintenanceDue())
    {
        std::cout << "This truck was due for scheduled service.\n";
    }
    else
    {
        std::cout
            << "Preventive maintenance started before "
            << "the service deadline.\n";
    }
}

void MaintenanceManager::CompleteTruckMaintenance(
    Company& company)
{
    bool foundTruckInMaintenance = false;

    std::cout << "\n========== TRUCKS IN MAINTENANCE ==========\n";

    for (const Truck& truck : company.GetTrucks())
    {
        if (truck.GetStatus() == "Maintenance")
        {
            foundTruckInMaintenance = true;

            std::cout << "Truck #" << truck.GetUnitNumber()
                << " | Mileage: " << truck.GetMileage()
                << "\n";
        }
    }

    if (!foundTruckInMaintenance)
    {
        std::cout << "No trucks are currently in maintenance.\n";
        std::cout << "===========================================\n";
        return;
    }

    std::cout << "===========================================\n";

    int unitNumber = InputManager::ReadPositiveInt(
        "\nEnter the truck unit number to complete maintenance: ",
        "truck unit number"
    );

    Truck* selectedTruck = FindTruck(company, unitNumber);

    if (selectedTruck == nullptr)
    {
        std::cout << "Truck #" << unitNumber << " does not exist.\n";
        return;
    }

    if (selectedTruck->GetStatus() != "Maintenance")
    {
        std::cout << "Truck #" << unitNumber
            << " is not currently in maintenance.\n";
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
        << "\n========== SERVICE COMPLETION CONFIRMATION ==========\n";

    std::cout
        << "Truck: #"
        << unitNumber
        << "\n";

    std::cout
        << "Service Time: "
        << HOSManager::FormatDateTime(
            maintenanceTime
        )
        << "\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Service Cost: $"
        << SERVICE_COST
        << "\n";

    std::cout
        << "Company Balance Before Service: $"
        << company.GetBalance()
        << "\n";

    std::cout
        << "Projected Balance After Service: $"
        << (company.GetBalance() - SERVICE_COST)
        << "\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "====================================================\n";

    if (!InputManager::ReadYesNo(
        "Complete maintenance and record the service expense? (Y/N): "))
    {
        std::cout
            << "Maintenance completion cancelled. "
            << "The truck remains in maintenance and no expense was recorded.\n";

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

    std::cout << "\nMaintenance completed for Truck #"
        << unitNumber << ".\n";

    std::cout << "Service Time: "
        << HOSManager::FormatDateTime(
            maintenanceTime
        )
        << "\n";

    std::cout << "Service cost: $"
        << std::fixed
        << std::setprecision(2)
        << SERVICE_COST
        << std::defaultfloat
        << std::setprecision(6)
        << "\n";

    std::cout << "Truck #" << unitNumber
        << " is now Available for dispatch.\n";

    std::cout << "Next service is due at "
        << selectedTruck->GetNextServiceMileage()
        << " miles.\n";
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
                << "\nInvalid selection. Please choose 0, 1, 2, or 3.\n";
            break;
        }
    }
}