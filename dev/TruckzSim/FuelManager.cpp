#include "ConsoleTheme.h"
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
    std::cout << ConsoleTheme::FormatLiteral("\n");
    std::cout << ConsoleTheme::FormatLiteral("========== FUEL MANAGEMENT ==========\n");
    std::cout << ConsoleTheme::FormatLiteral("1. View Fuel Status\n");
    std::cout << ConsoleTheme::FormatLiteral("2. Refuel Truck\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Return to Main Menu\n");
    std::cout << ConsoleTheme::FormatLiteral("=====================================\n");
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
        << ConsoleTheme::FormatLiteral("\n========== FLEET FUEL STATUS ==========\n");

    if (trucks.empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No trucks have been added yet.\n");

        std::cout
            << ConsoleTheme::FormatLiteral("=======================================\n");

        return;
    }

    for (const Truck& truck : trucks)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nTruck #")
            << truck.GetUnitNumber()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Status: ")
            << ConsoleTheme::Status(truck.GetStatus())
            << ConsoleTheme::FormatLiteral("\n");

        if (truck.GetAvailableAtEpoch() >
            HOSManager::GetCurrentEpoch())
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Scheduled Available: ")
                << HOSManager::FormatDateTime(
                    truck.GetAvailableAtEpoch()
                )
                << ConsoleTheme::FormatLiteral("\n");
        }
        else
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Scheduled Available: Now\n");
        }

        std::cout
            << std::fixed
            << std::setprecision(1);

        std::cout
            << ConsoleTheme::FormatLiteral("Fuel: ")
            << truck.GetFuelGallons()
            << ConsoleTheme::FormatLiteral(" / ")
            << truck.GetFuelTankCapacity()
            << ConsoleTheme::FormatLiteral(" gallons\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Fuel Level: ")
            << truck.GetFuelPercentage()
            << ConsoleTheme::FormatLiteral("%\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Fuel Economy: ")
            << truck.GetMilesPerGallon()
            << ConsoleTheme::FormatLiteral(" MPG\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Estimated Range: ")
            << truck.GetFuelRangeMiles()
            << ConsoleTheme::FormatLiteral(" miles\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Minimum Delivery Reserve: ")
            << truck.GetMinimumArrivalFuelGallons()
            << ConsoleTheme::FormatLiteral(" gallons (50% tank)\n");

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\nCurrent Diesel Price: $")
        << std::fixed
        << std::setprecision(2)
        << FUEL_PRICE_PER_GALLON
        << ConsoleTheme::FormatLiteral(" per gallon\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Fuel purchases are allowed even if the company balance becomes negative.\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("=======================================\n");
}

void FuelManager::RefuelTruck(
    Company& company)
{
    if (company.GetTrucks().empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nNo trucks are available to refuel.\n");

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
            << ConsoleTheme::FormatLiteral("Truck #")
            << unitNumber
            << ConsoleTheme::FormatLiteral(" does not exist.\n");

        return;
    }

    if (selectedTruck->GetStatus() !=
        "Available")
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Truck #")
            << unitNumber
            << ConsoleTheme::FormatLiteral(" is currently ")
            << ConsoleTheme::Status(selectedTruck->GetStatus())
            << ConsoleTheme::FormatLiteral(" and cannot be manually refueled.\n");

        return;
    }

    double availableSpace =
        selectedTruck->GetFuelTankCapacity()
        - selectedTruck->GetFuelGallons();

    if (availableSpace <= 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Truck #")
            << unitNumber
            << ConsoleTheme::FormatLiteral(" already has a full tank.\n");

        return;
    }

    std::cout
        << std::fixed
        << std::setprecision(1);

    std::cout
        << ConsoleTheme::FormatLiteral("Maximum fuel that can be added: ")
        << availableSpace
        << ConsoleTheme::FormatLiteral(" gallons\n");

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
        << ConsoleTheme::FormatLiteral("\n========== REFUEL CONFIRMATION ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Truck: #")
        << unitNumber
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Gallons to Purchase: ")
        << gallonsToPurchase
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Fuel Price: $")
        << FUEL_PRICE_PER_GALLON
        << ConsoleTheme::FormatLiteral(" per gallon\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Total Fuel Cost: $")
        << totalCost
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company Balance Before Purchase: $")
        << company.GetBalance()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Projected Balance After Purchase: $")
        << (company.GetBalance() - totalCost)
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("=========================================\n");

    if (!InputManager::ReadYesNo(
        "Confirm fuel purchase? (Y/N): "))
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Refuel cancelled. ")
            << ConsoleTheme::FormatLiteral("No fuel was added and no expense was recorded.\n");

        return;
    }

    double gallonsAdded =
        selectedTruck->AddFuel(
            gallonsToPurchase
        );

    if (gallonsAdded <= 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No fuel was added.\n");

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
        << ConsoleTheme::FormatLiteral("\nTruck #")
        << unitNumber
        << ConsoleTheme::FormatLiteral(" refueled successfully.\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Refuel Time: ")
        << HOSManager::FormatDateTime(
            refuelTime
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Gallons Purchased: ")
        << gallonsAdded
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Fuel Price: $")
        << FUEL_PRICE_PER_GALLON
        << ConsoleTheme::FormatLiteral(" per gallon\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Fuel Cost: $")
        << totalCost
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company Balance: $")
        << company.GetBalance()
        << ConsoleTheme::FormatLiteral("\n");

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("The company is operating with a negative balance. ")
            << ConsoleTheme::FormatLiteral("Future revenue will reduce the deficit.\n");
    }

    if (gallonsAdded <
        requestedGallons)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("The truck reached full capacity, ")
            << ConsoleTheme::FormatLiteral("so only the available tank space was purchased.\n");
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
                << ConsoleTheme::FormatLiteral("\nInvalid selection. ")
                << ConsoleTheme::FormatLiteral("Please choose 0, 1, or 2.\n");

            break;
        }
    }
}