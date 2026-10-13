#include "FreightManager.h"

#include <iomanip>
#include <iostream>

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

void FreightManager::AddLoad(Company& company)
{
    Load newLoad;

    std::cout << "\n========== ADD LOAD ==========\n";

    int loadNumber = 0;

    while (true)
    {
        loadNumber = InputManager::ReadPositiveInt(
            "Enter load number: ",
            "load number"
        );

        if (!company.HasLoadNumber(loadNumber))
        {
            break;
        }

        std::cout << "Load #" << loadNumber
            << " already exists. Please enter a unique load number.\n";
    }

    std::string origin = InputManager::ReadRequiredText(
        "Enter origin: ",
        "Origin"
    );

    std::string destination = InputManager::ReadRequiredText(
        "Enter destination: ",
        "Destination"
    );

    double pay = InputManager::ReadCurrency("Enter load pay: $");

    newLoad.SetLoadNumber(loadNumber);
    newLoad.SetOrigin(origin);
    newLoad.SetDestination(destination);
    newLoad.SetPay(pay);
    newLoad.SetStatus("Available");

    company.AddLoad(newLoad);

    std::cout << "\nLoad #" << loadNumber
        << " added successfully.\n";
}

void FreightManager::DisplayLoads(const Company& company)
{
    const std::vector<Load>& loads = company.GetLoads();

    std::cout << "\n========== COMPANY LOADS ==========\n";

    if (loads.empty())
    {
        std::cout << "No loads have been added yet.\n";
    }
    else
    {
        for (const Load& load : loads)
        {
            std::cout << "\nLoad #" << load.GetLoadNumber() << "\n";
            std::cout << "Origin: " << load.GetOrigin() << "\n";
            std::cout << "Destination: " << load.GetDestination() << "\n";
            std::cout << "Pay: $"
                << std::fixed
                << std::setprecision(2)
                << load.GetPay()
                << std::defaultfloat
                << std::setprecision(6)
                << "\n";
            std::cout << "Status: " << load.GetStatus() << "\n";

            if (load.GetAssignedDriverID() > 0 &&
                load.GetAssignedTruckUnitNumber() > 0)
            {
                std::cout << "Assigned Driver ID: "
                    << load.GetAssignedDriverID() << "\n";
                std::cout << "Assigned Truck: "
                    << load.GetAssignedTruckUnitNumber() << "\n";
            }
        }
    }

    std::cout << "=====================================\n";
}

void FreightManager::Run(Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();
        choice = InputManager::ReadInt("Select an option: ");

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
            std::cout << "\nInvalid selection. Please choose 0, 1, or 2.\n";
            break;
        }
    }
}
