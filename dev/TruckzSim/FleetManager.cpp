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
    std::cout << "\n";
    std::cout << "========== FLEET MANAGEMENT ==========\n";
    std::cout << "1. Add Truck\n";
    std::cout << "2. View Trucks\n";
    std::cout << "3. Breakdown / Repair Center\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "======================================\n";
}

void FleetManager::AddTruck(
    Company& company)
{
    Truck newTruck;

    std::cout
        << "\n========== ADD TRUCK ==========\n";

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
            << "Truck #"
            << unitNumber
            << " already exists. "
            << "Please enter a unique unit number.\n";
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
        << "\nTruck #"
        << unitNumber
        << " added successfully with a full "
        << std::fixed
        << std::setprecision(1)
        << fuelTankCapacity
        << "-gallon tank.\n"
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
        << "\n========== COMPANY FLEET ==========\n";

    if (trucks.empty())
    {
        std::cout
            << "No trucks have been added yet.\n";
    }
    else
    {
        for (const Truck& truck :
            trucks)
        {
            std::cout
                << "\nTruck #"
                << truck.GetUnitNumber()
                << "\n";

            std::cout
                << "Mileage: "
                << truck.GetMileage()
                << "\n";

            std::cout
                << "Status: "
                << truck.GetStatus()
                << "\n";

            std::cout
                << "Current Location: "
                << truck.GetCurrentLocation()
                << "\n";

            if (truck.GetAvailableAtEpoch() >
                now)
            {
                std::cout
                    << "Scheduled Available: "
                    << HOSManager::
                    FormatDateTime(
                        truck.
                        GetAvailableAtEpoch()
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
                << "Estimated Fuel Range: "
                << truck.GetFuelRangeMiles()
                << " miles\n";

            std::cout
                << "Required Delivery Reserve: "
                << truck.
                GetMinimumArrivalFuelGallons()
                << " gallons\n";

            if (truck.HasBreakdown())
            {
                std::cout
                    << "\n*** ACTIVE BREAKDOWN ***\n";

                std::cout
                    << "Repair Required: "
                    << truck.GetBreakdownType()
                    << "\n";

                std::cout
                    << std::fixed
                    << std::setprecision(2);

                std::cout
                    << "Parts: $"
                    << truck.GetBreakdownPartsCost()
                    << "\n";

                std::cout
                    << "Labor: $"
                    << truck.GetBreakdownLaborCost()
                    << "\n";

                std::cout
                    << "Service Call: $"
                    << truck.
                    GetBreakdownServiceCallCharge()
                    << "\n";

                std::cout
                    << "Total Repair: $"
                    << truck.GetBreakdownTotalCost()
                    << "\n";

                std::cout
                    << "Repair Time: "
                    << truck.GetBreakdownRepairHours()
                    << " hours\n";
            }

            std::cout
                << std::defaultfloat
                << std::setprecision(6);
        }
    }

    std::cout
        << "==================================\n";
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
                << "\nInvalid selection.\n";
            break;
        }
    }
}