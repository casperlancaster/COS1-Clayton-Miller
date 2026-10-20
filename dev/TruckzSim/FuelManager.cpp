#include "FuelManager.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "HOSManager.h"
#include "InputManager.h"
#include "Truck.h"

namespace
{
    constexpr double FUEL_PRICE_PER_GALLON =
        4.00;
}

void FuelManager::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== FUEL MANAGEMENT ==========\n";
    std::cout << "1. View Fuel Status\n";
    std::cout << "2. Refuel Truck\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=====================================\n";
}

Truck* FuelManager::FindTruck(
    Company& company,
    int unitNumber)
{
    for (Truck& truck :
        company.GetTrucks())
    {
        if (truck.GetUnitNumber() ==
            unitNumber)
        {
            return &truck;
        }
    }

    return nullptr;
}

void FuelManager::DisplayFuelStatus(
    const Company& company)
{
    const std::vector<Truck>& trucks =
        company.GetTrucks();

    std::cout
        << "\n========== FLEET FUEL STATUS ==========\n";

    if (trucks.empty())
    {
        std::cout
            << "No trucks have been added yet.\n";

        std::cout
            << "=======================================\n";

        return;
    }

    for (const Truck& truck : trucks)
    {
        std::cout
            << "\nTruck #"
            << truck.GetUnitNumber()
            << "\n";

        std::cout
            << "Status: "
            << truck.GetStatus()
            << "\n";

        if (truck.GetAvailableAtEpoch() >
            HOSManager::GetCurrentEpoch())
        {
            std::cout
                << "Scheduled Available: "
                << HOSManager::FormatDateTime(
                    truck.GetAvailableAtEpoch()
                )
                << "\n";
        }
        else
        {
            std::cout
                << "Scheduled Available: Now\n";
        }

        std::cout
            << std::fixed
            << std::setprecision(1);

        std::cout
            << "Fuel: "
            << truck.GetFuelGallons()
            << " / "
            << truck.GetFuelTankCapacity()
            << " gallons\n";

        std::cout
            << "Fuel Level: "
            << truck.GetFuelPercentage()
            << "%\n";

        std::cout
            << "Fuel Economy: "
            << truck.GetMilesPerGallon()
            << " MPG\n";

        std::cout
            << "Estimated Range: "
            << truck.GetFuelRangeMiles()
            << " miles\n";

        std::cout
            << "Minimum Delivery Reserve: "
            << truck.GetMinimumArrivalFuelGallons()
            << " gallons (50% tank)\n";

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    std::cout
        << "\nCurrent Diesel Price: $"
        << std::fixed
        << std::setprecision(2)
        << FUEL_PRICE_PER_GALLON
        << " per gallon\n";

    std::cout
        << "Fuel purchases are allowed even if the company balance becomes negative.\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "=======================================\n";
}

void FuelManager::RefuelTruck(
    Company& company)
{
    if (company.GetTrucks().empty())
    {
        std::cout
            << "\nNo trucks are available to refuel.\n";

        return;
    }

    DisplayFuelStatus(company);

    int unitNumber =
        InputManager::ReadPositiveInt(
            "\nEnter the truck unit number to refuel: ",
            "truck unit number"
        );

    Truck* selectedTruck =
        FindTruck(
            company,
            unitNumber
        );

    if (selectedTruck == nullptr)
    {
        std::cout
            << "Truck #"
            << unitNumber
            << " does not exist.\n";

        return;
    }

    if (selectedTruck->GetStatus() !=
        "Available")
    {
        std::cout
            << "Truck #"
            << unitNumber
            << " is currently "
            << selectedTruck->GetStatus()
            << " and cannot be manually refueled.\n";

        return;
    }

    double availableSpace =
        selectedTruck->GetFuelTankCapacity()
        - selectedTruck->GetFuelGallons();

    if (availableSpace <= 0.0)
    {
        std::cout
            << "Truck #"
            << unitNumber
            << " already has a full tank.\n";

        return;
    }

    std::cout
        << std::fixed
        << std::setprecision(1);

    std::cout
        << "Maximum fuel that can be added: "
        << availableSpace
        << " gallons\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    double requestedGallons =
        InputManager::ReadPositiveDouble(
            "Enter gallons to purchase: ",
            "fuel amount"
        );

    double gallonsToPurchase =
        std::min(
            requestedGallons,
            availableSpace
        );

    double totalCost =
        gallonsToPurchase *
        FUEL_PRICE_PER_GALLON;

    std::cout
        << "\n========== REFUEL CONFIRMATION ==========\n";

    std::cout
        << "Truck: #"
        << unitNumber
        << "\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Gallons to Purchase: "
        << gallonsToPurchase
        << "\n";

    std::cout
        << "Fuel Price: $"
        << FUEL_PRICE_PER_GALLON
        << " per gallon\n";

    std::cout
        << "Total Fuel Cost: $"
        << totalCost
        << "\n";

    std::cout
        << "Company Balance Before Purchase: $"
        << company.GetBalance()
        << "\n";

    std::cout
        << "Projected Balance After Purchase: $"
        << (company.GetBalance() - totalCost)
        << "\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "=========================================\n";

    if (!InputManager::ReadYesNo(
        "Confirm fuel purchase? (Y/N): "))
    {
        std::cout
            << "Refuel cancelled. "
            << "No fuel was added and no expense was recorded.\n";

        return;
    }

    double gallonsAdded =
        selectedTruck->AddFuel(
            gallonsToPurchase
        );

    if (gallonsAdded <= 0.0)
    {
        std::cout
            << "No fuel was added.\n";

        return;
    }

    long long refuelTime =
        std::max(
            HOSManager::GetCurrentEpoch(),
            selectedTruck->GetAvailableAtEpoch()
        );

    FinancialTransaction fuelTransaction;

    fuelTransaction.transactionEpoch =
        refuelTime;

    fuelTransaction.category =
        "Fuel";

    fuelTransaction.description =
        "Manual Refuel";

    fuelTransaction.amount =
        totalCost;

    fuelTransaction.truckUnitNumber =
        unitNumber;

    fuelTransaction.gallons =
        gallonsAdded;

    fuelTransaction.pricePerGallon =
        FUEL_PRICE_PER_GALLON;

    company.RecordExpenseTransaction(
        fuelTransaction
    );

    std::cout
        << "\nTruck #"
        << unitNumber
        << " refueled successfully.\n";

    std::cout
        << "Refuel Time: "
        << HOSManager::FormatDateTime(
            refuelTime
        )
        << "\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Gallons Purchased: "
        << gallonsAdded
        << "\n";

    std::cout
        << "Fuel Price: $"
        << FUEL_PRICE_PER_GALLON
        << " per gallon\n";

    std::cout
        << "Fuel Cost: $"
        << totalCost
        << "\n";

    std::cout
        << "Company Balance: $"
        << company.GetBalance()
        << "\n";

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << "The company is operating with a negative balance. "
            << "Future revenue will reduce the deficit.\n";
    }

    if (gallonsAdded <
        requestedGallons)
    {
        std::cout
            << "The truck reached full capacity, "
            << "so only the available tank space was purchased.\n";
    }

    std::cout
        << std::defaultfloat
        << std::setprecision(6);
}

void FuelManager::Run(
    Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                2
            );

        switch (choice)
        {
        case 1:
            DisplayFuelStatus(company);
            break;

        case 2:
            RefuelTruck(company);
            break;

        case 0:
            break;

        default:
            std::cout
                << "\nInvalid selection. "
                << "Please choose 0, 1, or 2.\n";

            break;
        }
    }
}