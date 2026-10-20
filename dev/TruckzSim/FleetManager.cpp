#include "ConsoleTheme.h"
#include "FleetManager.h"

#include <iomanip>
#include <iostream>
#include <string>

#include "HOSManager.h"
#include "InputManager.h"
#include "RepairManager.h"
#include "Truck.h"

void FleetManager::ShowMenu()
{
    std::cout << ConsoleTheme::FormatLiteral("\n");
    std::cout << ConsoleTheme::FormatLiteral("========== FLEET MANAGEMENT ==========\n");
    std::cout << ConsoleTheme::FormatLiteral("1. Add Truck\n");
    std::cout << ConsoleTheme::FormatLiteral("2. View Trucks\n");
    std::cout << ConsoleTheme::FormatLiteral("3. Breakdown / Repair Center\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Return to Main Menu\n");
    std::cout << ConsoleTheme::FormatLiteral("======================================\n");
}

void FleetManager::AddTruck(
    Company& company)
{
    Truck newTruck;

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== ADD TRUCK ==========\n");

    int unitNumber = 0;

    while (true)
    {
        unitNumber =
            InputManager::ReadPositiveInt(
                "Enter unit number: ",
                "unit number"
            );

        if (!company.HasTruckUnitNumber(
            unitNumber))
        {
            break;
        }

        std::cout
            << ConsoleTheme::FormatLiteral("Truck #")
            << unitNumber
            << ConsoleTheme::FormatLiteral(" already exists. ")
            << ConsoleTheme::FormatLiteral("Please enter a unique unit number.\n");
    }

    double mileage =
        InputManager::ReadNonNegativeDouble(
            "Enter current mileage: ",
            "mileage"
        );

    std::string currentLocation =
        InputManager::ReadRequiredText(
            "Enter current location: ",
            "current location"
        );

    double fuelTankCapacity =
        InputManager::ReadPositiveDouble(
            "Enter fuel tank capacity in gallons: ",
            "fuel tank capacity"
        );

    double milesPerGallon =
        InputManager::ReadPositiveDouble(
            "Enter estimated fuel economy (MPG): ",
            "fuel economy"
        );

    newTruck.SetUnitNumber(
        unitNumber
    );

    newTruck.SetMileage(
        mileage
    );

    newTruck.SetCurrentLocation(
        currentLocation
    );

    newTruck.InitializeMaintenanceSchedule();

    newTruck.InitializeFuelSystem(
        fuelTankCapacity,
        milesPerGallon
    );

    newTruck.SetStatus(
        "Available"
    );

    newTruck.SetAvailableAtEpoch(
        0
    );

    company.AddTruck(
        newTruck
    );

    std::cout
        << ConsoleTheme::FormatLiteral("\nTruck #")
        << unitNumber
        << ConsoleTheme::FormatLiteral(" added successfully with a full ")
        << std::fixed
        << std::setprecision(1)
        << fuelTankCapacity
        << ConsoleTheme::FormatLiteral("-gallon tank.\n")
        << std::defaultfloat
        << std::setprecision(6);
}

void FleetManager::DisplayTrucks(
    const Company& company)
{
    const std::vector<Truck>& trucks =
        company.GetTrucks();

    long long now =
        HOSManager::GetCurrentEpoch();

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== COMPANY FLEET ==========\n");

    if (trucks.empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No trucks have been added yet.\n");
    }
    else
    {
        for (const Truck& truck :
            trucks)
        {
            std::cout
                << ConsoleTheme::FormatLiteral("\nTruck #")
                << truck.GetUnitNumber()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Mileage: ")
                << truck.GetMileage()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Status: ")
                << ConsoleTheme::Status(truck.GetStatus())
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Current Location: ")
                << truck.GetCurrentLocation()
                << ConsoleTheme::FormatLiteral("\n");

            if (truck.GetAvailableAtEpoch() >
                now)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Scheduled Available: ")
                    << HOSManager::
                    FormatDateTime(
                        truck.
                        GetAvailableAtEpoch()
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
                << ConsoleTheme::FormatLiteral("Estimated Fuel Range: ")
                << truck.GetFuelRangeMiles()
                << ConsoleTheme::FormatLiteral(" miles\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Required Delivery Reserve: ")
                << truck.
                GetMinimumArrivalFuelGallons()
                << ConsoleTheme::FormatLiteral(" gallons\n");

            if (truck.HasBreakdown())
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\n*** ACTIVE BREAKDOWN ***\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Repair Required: ")
                    << truck.GetBreakdownType()
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << std::fixed
                    << std::setprecision(2);

                std::cout
                    << ConsoleTheme::FormatLiteral("Parts: $")
                    << truck.GetBreakdownPartsCost()
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Labor: $")
                    << truck.GetBreakdownLaborCost()
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Service Call: $")
                    << truck.
                    GetBreakdownServiceCallCharge()
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Total Repair: $")
                    << truck.GetBreakdownTotalCost()
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Repair Time: ")
                    << truck.GetBreakdownRepairHours()
                    << ConsoleTheme::FormatLiteral(" hours\n");
            }

            std::cout
                << std::defaultfloat
                << std::setprecision(6);
        }
    }

    std::cout
        << ConsoleTheme::FormatLiteral("==================================\n");
}

void FleetManager::Run(
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
                3
            );

        switch (choice)
        {
        case 1:
            AddTruck(
                company
            );
            break;

        case 2:
            DisplayTrucks(
                company
            );
            break;

        case 3:
            RepairManager::Run(
                company
            );
            break;

        case 0:
            break;

        default:
            std::cout
                << ConsoleTheme::FormatLiteral("\nInvalid selection.\n");
            break;
        }
    }
}