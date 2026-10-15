#include "DriverManager.h"

#include <iostream>

#include "Driver.h"
#include "InputManager.h"

void DriverManager::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== DRIVER MANAGEMENT ==========\n";
    std::cout << "1. Add Driver\n";
    std::cout << "2. View Drivers\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=======================================\n";
}

void DriverManager::AddDriver(Company& company)
{
    Driver newDriver;

    std::cout << "\n========== ADD DRIVER ==========\n";

    int driverID = 0;

    while (true)
    {
        driverID = InputManager::ReadPositiveInt(
            "Enter Driver ID: ",
            "driver ID"
        );

        if (!company.HasDriverID(driverID))
        {
            break;
        }

        std::cout << "Driver ID " << driverID
            << " already exists. Please enter a unique Driver ID.\n";
    }

    std::string driverName = InputManager::ReadRequiredText(
        "Enter driver name: ",
        "Driver name"
    );

    newDriver.SetDriverID(driverID);
    newDriver.SetName(driverName);
    newDriver.SetStatus("Available");

    company.AddDriver(newDriver);

    std::cout << "\nDriver " << driverName << " added successfully.\n";
}

void DriverManager::DisplayDrivers(const Company& company)
{
    const std::vector<Driver>& drivers = company.GetDrivers();

    std::cout << "\n========== COMPANY DRIVERS ==========\n";

    if (drivers.empty())
    {
        std::cout << "No drivers have been added yet.\n";
    }
    else
    {
        for (int i = 0; i < drivers.size(); i++)
        {
            std::cout << "\nDriver ID: " << drivers[i].GetDriverID() << "\n";
            std::cout << "Name: " << drivers[i].GetName() << "\n";
            std::cout << "Status: " << drivers[i].GetStatus() << "\n";
        }
    }

    std::cout << "===============================================\n";
}

void DriverManager::Run(Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();
        choice = InputManager::ReadInt("Select an option: ");

        switch (choice)
        {
        case 1:
            AddDriver(company);
            break;

        case 2:
            DisplayDrivers(company);
            break;

        case 0:
            break;

        default:
            std::cout << "\nInvalid selection. Please choose 0, 1, or 2.\n";
            break;
        }
    }
}
