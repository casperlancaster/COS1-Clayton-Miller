#include "ConsoleTheme.h"
#include "FreightManager.h"

#include <iomanip>
#include <iostream>

#include "HOSManager.h"
#include "InputManager.h"
#include "Load.h"

void FreightManager::ShowMenu()
{
    std::cout << ConsoleTheme::FormatLiteral("\n");
    std::cout << ConsoleTheme::FormatLiteral("========== FREIGHT MARKET ==========\n");
    std::cout << ConsoleTheme::FormatLiteral("1. Add Load\n");
    std::cout << ConsoleTheme::FormatLiteral("2. View Loads\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Return to Main Menu\n");
    std::cout << ConsoleTheme::FormatLiteral("====================================\n");
}

void FreightManager::AddLoad(
    Company& company)
{
    Load newLoad;

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== ADD LOAD ==========\n");

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
            << ConsoleTheme::FormatLiteral("Load #")
            << loadNumber
            << ConsoleTheme::FormatLiteral(" already exists. ")
            << ConsoleTheme::FormatLiteral("Please enter a unique load number.\n");
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
            << ConsoleTheme::FormatLiteral("\nEnter the delivery deadline using local time.\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Preferred Date Format: ")
            << HOSManager::GetDateFormat()
            << ConsoleTheme::FormatLiteral(" | Example: ")
            << HOSManager::GetDateInputExample()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Preferred Time Format: ")
            << HOSManager::GetTimeFormat()
            << ConsoleTheme::FormatLiteral(" | Example: ")
            << HOSManager::GetTimeInputExample()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Separators are optional, so entries like ")
            << ConsoleTheme::FormatLiteral("10222026 and 830 PM also work.\n");

        std::string dueDate =
            InputManager::ReadRequiredText(
                "Due date: ",
                "Due date"
            );

        std::string dueTime =
            InputManager::ReadRequiredText(
                "Due time: ",
                "Due time"
            );

        if (!HOSManager::ParseDateTime(
            dueDate,
            dueTime,
            deadlineEpoch))
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Invalid date or time. Please try again.\n");

            continue;
        }

        if (deadlineEpoch <=
            HOSManager::GetCurrentEpoch())
        {
            std::cout
                << ConsoleTheme::FormatLiteral("The delivery deadline must be in the future.\n");

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
        << ConsoleTheme::FormatLiteral("\nLoad #")
        << loadNumber
        << ConsoleTheme::FormatLiteral(" added successfully.\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Delivery Deadline: ")
        << HOSManager::FormatDateTime(
            deadlineEpoch
        )
        << ConsoleTheme::FormatLiteral("\n");
}

void FreightManager::DisplayLoads(
    const Company& company)
{
    const std::vector<Load>& loads =
        company.GetLoads();

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== COMPANY LOADS ==========\n");

    if (loads.empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No loads have been added yet.\n");
    }
    else
    {
        for (const Load& load :
            loads)
        {
            std::cout
                << ConsoleTheme::FormatLiteral("\nLoad #")
                << load.GetLoadNumber()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Origin: ")
                << load.GetOrigin()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Destination: ")
                << load.GetDestination()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Distance: ")
                << load.GetDistanceMiles()
                << ConsoleTheme::FormatLiteral(" miles\n");

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << ConsoleTheme::FormatLiteral("Pay: $")
                << load.GetPay()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            std::cout
                << ConsoleTheme::FormatLiteral("Deadline: ")
                << HOSManager::FormatDateTime(
                    load.GetDeliveryDeadlineEpoch()
                )
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Status: ")
                << ConsoleTheme::Status(load.GetStatus())
                << ConsoleTheme::FormatLiteral("\n");

            if (load.GetAssignedDriverID() > 0)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Primary Driver ID: ")
                    << load.GetAssignedDriverID()
                    << ConsoleTheme::FormatLiteral("\n");
            }

            if (load.GetAssignedSecondDriverID() > 0)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Team Driver ID: ")
                    << load.GetAssignedSecondDriverID()
                    << ConsoleTheme::FormatLiteral("\n");
            }

            if (load.GetAssignedTruckUnitNumber() > 0)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Assigned Truck: ")
                    << load.GetAssignedTruckUnitNumber()
                    << ConsoleTheme::FormatLiteral("\n");
            }

            if (load.GetEstimatedDeliveryEpoch() > 0)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Planned Delivery: ")
                    << HOSManager::FormatDateTime(
                        load.GetEstimatedDeliveryEpoch()
                    )
                    << ConsoleTheme::FormatLiteral("\n");
            }

            if (load.IsTeamLoad())
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Operation: TEAM\n");
            }
            else if (load.GetAssignedDriverID() > 0)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Operation: SOLO\n");
            }
        }
    }

    std::cout
        << ConsoleTheme::FormatLiteral("=====================================\n");
}

void FreightManager::Run(
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
            AddLoad(
                company
            );
            break;

        case 2:
            DisplayLoads(
                company
            );
            break;

        case 0:
            break;
        }
    }
}
