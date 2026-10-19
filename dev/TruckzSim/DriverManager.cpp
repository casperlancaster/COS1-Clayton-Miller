#include "DriverManager.h"

#include <iomanip>
#include <iostream>

#include "Driver.h"
#include "HOSManager.h"
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

void DriverManager::AddDriver(
    Company& company)
{
    Driver newDriver;

    std::cout
        << "\n========== ADD DRIVER ==========\n";

    int driverID = 0;

    while (true)
    {
        driverID =
            InputManager::ReadPositiveInt(
                "Enter Driver ID: ",
                "driver ID"
            );

        if (!company.HasDriverID(
            driverID))
        {
            break;
        }

        std::cout
            << "Driver ID "
            << driverID
            << " already exists. "
            << "Please enter a unique Driver ID.\n";
    }

    std::string driverName =
        InputManager::ReadRequiredText(
            "Enter driver name: ",
            "Driver name"
        );

    newDriver.SetDriverID(
        driverID
    );

    newDriver.SetName(
        driverName
    );

    newDriver.SetStatus(
        "Available"
    );

    newDriver.ResetDrivingHours();

    company.AddDriver(
        newDriver
    );

    std::cout
        << "\nDriver "
        << driverName
        << " added successfully.\n";

    std::cout
        << "Starting HOS: "
        << std::fixed
        << std::setprecision(1)
        << HOSManager::
        GetMaxDrivingHours()
        << " driving hours available.\n"
        << std::defaultfloat
        << std::setprecision(6);
}

void DriverManager::DisplayDrivers(
    const Company& company)
{
    const std::vector<Driver>& drivers =
        company.GetDrivers();

    long long now =
        HOSManager::GetCurrentEpoch();

    std::cout
        << "\n========== COMPANY DRIVERS ==========\n";

    if (drivers.empty())
    {
        std::cout
            << "No drivers have been added yet.\n";
    }
    else
    {
        for (const Driver& driver :
            drivers)
        {
            long long planningTime =
                now;

            if (driver.GetAvailableAtEpoch() >
                planningTime)
            {
                planningTime =
                    driver.GetAvailableAtEpoch();
            }

            double projectedHours =
                driver.
                GetProjectedDrivingHoursAt(
                    planningTime
                );

            double projectedRest =
                driver.
                GetProjectedRestHoursAt(
                    planningTime
                );

            std::cout
                << "\nDriver ID: "
                << driver.GetDriverID()
                << "\n";

            std::cout
                << "Name: "
                << driver.GetName()
                << "\n";

            std::cout
                << "Status: "
                << driver.GetStatus()
                << "\n";

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << "Driving Hours Remaining: "
                << projectedHours
                << " / "
                << HOSManager::
                GetMaxDrivingHours()
                << "\n";

            std::cout
                << "Rest Progress: "
                << projectedRest
                << " / "
                << HOSManager::
                GetRequiredRestHours()
                << " hours\n";

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            if (driver.GetAvailableAtEpoch() >
                now)
            {
                std::cout
                    << "Scheduled Available: "
                    << HOSManager::
                    FormatDateTime(
                        driver.
                        GetAvailableAtEpoch()
                    )
                    << "\n";
            }
            else
            {
                std::cout
                    << "Scheduled Available: Now\n";
            }
        }
    }

    std::cout
        << "=====================================\n";
}

void DriverManager::Run(
    Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();

        choice =
            InputManager::ReadInt(
                "Select an option: "
            );

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
            std::cout
                << "\nInvalid selection. "
                << "Please choose 0, 1, or 2.\n";

            break;
        }
    }
}