#include "FreightManager.h"

#include <iomanip>
#include <iostream>

#include "HOSManager.h"
#include "InputManager.h"
#include "Load.h"

void FreightManager::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== FREIGHT MARKET ==========\n";
    std::cout << "1. Add Load\n";
    std::cout << "2. View Loads\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "====================================\n";
}

void FreightManager::AddLoad(
    Company& company)
{
    Load newLoad;

    std::cout
        << "\n========== ADD LOAD ==========\n";

    int loadNumber = 0;

    while (true)
    {
        loadNumber =
            InputManager::ReadPositiveInt(
                "Enter load number: ",
                "load number"
            );

        if (!company.HasLoadNumber(
            loadNumber))
        {
            break;
        }

        std::cout
            << "Load #"
            << loadNumber
            << " already exists. "
            << "Please enter a unique load number.\n";
    }

    std::string origin =
        InputManager::ReadRequiredText(
            "Enter origin: ",
            "Origin"
        );

    std::string destination =
        InputManager::ReadRequiredText(
            "Enter destination: ",
            "Destination"
        );

    double distanceMiles =
        InputManager::ReadPositiveDouble(
            "Enter load distance in miles: ",
            "load distance"
        );

    double pay =
        InputManager::ReadCurrency(
            "Enter load pay: $"
        );

    long long deadlineEpoch = 0;

    while (true)
    {
        std::cout
            << "\nEnter the delivery deadline "
            << "using local time.\n";

        std::string dueDate =
            InputManager::ReadRequiredText(
                "Due date (YYYY-MM-DD): ",
                "Due date"
            );

        std::string dueTime =
            InputManager::ReadRequiredText(
                "Due time (HH:MM, 24-hour): ",
                "Due time"
            );

        if (!HOSManager::ParseDateTime(
            dueDate,
            dueTime,
            deadlineEpoch))
        {
            std::cout
                << "Invalid date or time. "
                << "Example: 2026-10-22 and 18:30.\n";

            continue;
        }

        if (deadlineEpoch <=
            HOSManager::GetCurrentEpoch())
        {
            std::cout
                << "The delivery deadline must "
                << "be in the future.\n";

            continue;
        }

        break;
    }

    newLoad.SetLoadNumber(
        loadNumber
    );

    newLoad.SetOrigin(
        origin
    );

    newLoad.SetDestination(
        destination
    );

    newLoad.SetDistanceMiles(
        distanceMiles
    );

    newLoad.SetPay(
        pay
    );

    newLoad.SetDeliveryDeadlineEpoch(
        deadlineEpoch
    );

    newLoad.SetStatus(
        "Available"
    );

    company.AddLoad(
        newLoad
    );

    std::cout
        << "\nLoad #"
        << loadNumber
        << " added successfully.\n";

    std::cout
        << "Delivery Deadline: "
        << HOSManager::FormatDateTime(
            deadlineEpoch
        )
        << "\n";
}

void FreightManager::DisplayLoads(
    const Company& company)
{
    const std::vector<Load>& loads =
        company.GetLoads();

    std::cout
        << "\n========== COMPANY LOADS ==========\n";

    if (loads.empty())
    {
        std::cout
            << "No loads have been added yet.\n";
    }
    else
    {
        for (const Load& load :
            loads)
        {
            std::cout
                << "\nLoad #"
                << load.GetLoadNumber()
                << "\n";

            std::cout
                << "Origin: "
                << load.GetOrigin()
                << "\n";

            std::cout
                << "Destination: "
                << load.GetDestination()
                << "\n";

            std::cout
                << "Distance: "
                << load.GetDistanceMiles()
                << " miles\n";

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << "Pay: $"
                << load.GetPay()
                << "\n";

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            std::cout
                << "Deadline: "
                << HOSManager::FormatDateTime(
                    load.
                    GetDeliveryDeadlineEpoch()
                )
                << "\n";

            std::cout
                << "Status: "
                << load.GetStatus()
                << "\n";

            if (load.GetAssignedDriverID() >
                0)
            {
                std::cout
                    << "Primary Driver ID: "
                    << load.
                    GetAssignedDriverID()
                    << "\n";
            }

            if (load.
                GetAssignedSecondDriverID() >
                0)
            {
                std::cout
                    << "Team Driver ID: "
                    << load.
                    GetAssignedSecondDriverID()
                    << "\n";
            }

            if (load.
                GetAssignedTruckUnitNumber() >
                0)
            {
                std::cout
                    << "Assigned Truck: "
                    << load.
                    GetAssignedTruckUnitNumber()
                    << "\n";
            }

            if (load.
                GetEstimatedDeliveryEpoch() >
                0)
            {
                std::cout
                    << "Planned Delivery: "
                    << HOSManager::FormatDateTime(
                        load.
                        GetEstimatedDeliveryEpoch()
                    )
                    << "\n";
            }

            if (load.IsTeamLoad())
            {
                std::cout
                    << "Operation: TEAM\n";
            }
            else if (load.
                GetAssignedDriverID() > 0)
            {
                std::cout
                    << "Operation: SOLO\n";
            }
        }
    }

    std::cout
        << "=====================================\n";
}

void FreightManager::Run(
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
            AddLoad(company);
            break;

        case 2:
            DisplayLoads(company);
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