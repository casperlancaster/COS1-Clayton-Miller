#include "FleetManager.h"

#include <iostream>

#include "InputManager.h"
#include "Truck.h"

void FleetManager::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== FLEET MANAGEMENT ==========\n";
    std::cout << "1. Add Truck\n";
    std::cout << "2. View Trucks\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "======================================\n";
}

void FleetManager::AddTruck(Company& company)
{
    Truck newTruck;

    std::cout << "\n========== ADD TRUCK ==========\n";

    int unitNumber = 0;

    while (true)
    {
        unitNumber = InputManager::ReadPositiveInt(
            "Enter unit number: ",
            "unit number"
        );

        if (!company.HasTruckUnitNumber(unitNumber))
        {
            break;
        }

        std::cout << "Truck #" << unitNumber
            << " already exists. Please enter a unique unit number.\n";
    }

    double mileage = InputManager::ReadNonNegativeDouble(
        "Enter current mileage: ",
        "mileage"
    );

    newTruck.SetUnitNumber(unitNumber);
    newTruck.SetMileage(mileage);
    newTruck.SetStatus("Available");

    company.AddTruck(newTruck);

    std::cout << "\nTruck #" << unitNumber << " added successfully.\n";
}

void FleetManager::DisplayTrucks(const Company& company)
{
    const std::vector<Truck>& trucks = company.GetTrucks();

    std::cout << "\n========== COMPANY FLEET ==========\n";

    if (trucks.empty())
    {
        std::cout << "No trucks have been added yet.\n";
    }
    else
    {
        for (int i = 0; i < trucks.size(); i++)
        {
            std::cout << "\nTruck #" << trucks[i].GetUnitNumber() << "\n";
            std::cout << "Mileage: " << trucks[i].GetMileage() << "\n";
            std::cout << "Status: " << trucks[i].GetStatus() << "\n";
        }
    }

    std::cout << "==================================\n";
}

void FleetManager::Run(Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();
        choice = InputManager::ReadInt("Select an option: ");

        switch (choice)
        {
        case 1:
            AddTruck(company);
            break;

        case 2:
            DisplayTrucks(company);
            break;

        case 0:
            break;

        default:
            std::cout << "\nInvalid selection. Please choose 0, 1, or 2.\n";
            break;
        }
    }
}
