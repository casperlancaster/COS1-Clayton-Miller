#include "ConsoleTheme.h"
#include "RepairManager.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include "HOSManager.h"
#include "InputManager.h"

namespace
{
    struct BreakdownTemplate
    {
        std::string type;
        double partsCost;
        double laborCost;
        double serviceCallCharge;
        double repairHours;
    };

    const std::array<BreakdownTemplate, 6>
        BREAKDOWN_TYPES =
    { {
        {
            "Tire Blowout",
            550.00,
            200.00,
            150.00,
            2.0
        },
        {
            "Alternator Failure",
            650.00,
            300.00,
            175.00,
            4.0
        },
        {
            "Air Brake System Failure",
            1200.00,
            550.00,
            250.00,
            6.0
        },
        {
            "Cooling System Failure",
            900.00,
            400.00,
            200.00,
            5.0
        },
        {
            "Electrical System Failure",
            500.00,
            350.00,
            175.00,
            4.0
        },
        {
            "Transmission Failure",
            4500.00,
            1500.00,
            300.00,
            12.0
        }
    } };

    int CalculateBreakdownChance(
        const Truck& truck,
        double tripDistanceMiles)
    {
        int chance = 20;

        if (tripDistanceMiles >= 1000.0)
        {
            chance += 5;
        }

        if (tripDistanceMiles >= 2000.0)
        {
            chance += 5;
        }

        if (truck.IsMaintenanceDue())
        {
            chance += 15;
        }

        if (truck.GetMileage() >= 250000.0)
        {
            chance += 5;
        }

        if (truck.GetMileage() >= 500000.0)
        {
            chance += 5;
        }

        if (chance > 60)
        {
            chance = 60;
        }

        return chance;
    }
}

bool RepairManager::RollForBreakdown(
    Truck& truck,
    double tripDistanceMiles)
{
    if (truck.HasBreakdown())
    {
        return false;
    }

    static std::mt19937 randomEngine(
        std::random_device{}()
    );

    int breakdownChance =
        CalculateBreakdownChance(
            truck,
            tripDistanceMiles
        );

    std::uniform_int_distribution<int>
        chanceDistribution(
            1,
            100
        );

    int roll =
        chanceDistribution(
            randomEngine
        );

    if (roll > breakdownChance)
    {
        return false;
    }

    std::uniform_int_distribution<std::size_t>
        breakdownDistribution(
            0,
            BREAKDOWN_TYPES.size() - 1
        );

    const BreakdownTemplate& breakdown =
        BREAKDOWN_TYPES[
            breakdownDistribution(
                randomEngine
            )
        ];

    truck.SetBreakdown(
        breakdown.type,
        breakdown.partsCost,
        breakdown.laborCost,
        breakdown.serviceCallCharge,
        breakdown.repairHours
    );

    truck.SetStatus(
        "Breakdown"
    );

    return true;
}

void RepairManager::ShowMenu()
{
    std::cout << ConsoleTheme::FormatLiteral("\n");
    std::cout << ConsoleTheme::FormatLiteral("========== BREAKDOWN / REPAIR CENTER ==========\n");
    std::cout << ConsoleTheme::FormatLiteral("1. View Active Breakdowns\n");
    std::cout << ConsoleTheme::FormatLiteral("2. Repair Truck\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Return to Fleet Management\n");
    std::cout << ConsoleTheme::FormatLiteral("===============================================\n");
}

Truck* RepairManager::FindTruck(
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

void RepairManager::DisplayActiveBreakdowns(
    const Company& company)
{
    bool foundBreakdown = false;

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== ACTIVE BREAKDOWNS ==========\n");

    for (const Truck& truck :
        company.GetTrucks())
    {
        if (!truck.HasBreakdown())
        {
            continue;
        }

        foundBreakdown = true;

        std::cout
            << ConsoleTheme::FormatLiteral("\nTruck #")
            << truck.GetUnitNumber()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Breakdown: ")
            << truck.GetBreakdownType()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << ConsoleTheme::FormatLiteral("Parts Cost: $")
            << truck.GetBreakdownPartsCost()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Labor Cost: $")
            << truck.GetBreakdownLaborCost()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Service Call Charge: $")
            << truck.
            GetBreakdownServiceCallCharge()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Total Repair Cost: $")
            << truck.GetBreakdownTotalCost()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Estimated Repair Time: ")
            << truck.GetBreakdownRepairHours()
            << ConsoleTheme::FormatLiteral(" hours\n");

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    if (!foundBreakdown)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No trucks currently have ")
            << ConsoleTheme::FormatLiteral("an active breakdown.\n");
    }

    std::cout
        << ConsoleTheme::FormatLiteral("=======================================\n");
}

void RepairManager::RepairTruck(
    Company& company)
{
    bool foundBreakdown = false;

    for (const Truck& truck :
        company.GetTrucks())
    {
        if (truck.HasBreakdown())
        {
            foundBreakdown = true;
            break;
        }
    }

    if (!foundBreakdown)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nThere are no active breakdowns ")
            << ConsoleTheme::FormatLiteral("to repair.\n");

        return;
    }

    DisplayActiveBreakdowns(
        company
    );

    int unitNumber =
        InputManager::ReadPositiveInt(
            "\nEnter truck unit number to repair: ",
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

    if (!selectedTruck->HasBreakdown())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Truck #")
            << unitNumber
            << ConsoleTheme::FormatLiteral(" does not have an active breakdown.\n");

        return;
    }

    std::string repairType =
        selectedTruck->
        GetBreakdownType();

    double partsCost =
        selectedTruck->
        GetBreakdownPartsCost();

    double laborCost =
        selectedTruck->
        GetBreakdownLaborCost();

    double serviceCallCharge =
        selectedTruck->
        GetBreakdownServiceCallCharge();

    double totalCost =
        selectedTruck->
        GetBreakdownTotalCost();

    double repairHours =
        selectedTruck->
        GetBreakdownRepairHours();

    long long repairStart =
        std::max(
            HOSManager::GetCurrentEpoch(),
            selectedTruck->
            GetAvailableAtEpoch()
        );

    long long repairSeconds =
        std::llround(
            repairHours *
            3600.0
        );

    long long repairComplete =
        repairStart +
        repairSeconds;

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== REPAIR CONFIRMATION ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Truck: #")
        << unitNumber
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Repair: ")
        << repairType
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Parts: $")
        << partsCost
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Labor: $")
        << laborCost
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Service Call: $")
        << serviceCallCharge
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Total Repair Expense: $")
        << totalCost
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company Balance Before Repair: $")
        << company.GetBalance()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Projected Balance After Repair: $")
        << (company.GetBalance() - totalCost)
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("Repair Start: ")
        << HOSManager::FormatDateTime(
            repairStart
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Estimated Completion: ")
        << HOSManager::FormatDateTime(
            repairComplete
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("=========================================\n");

    if (!InputManager::ReadYesNo(
        "Authorize this repair? (Y/N): "))
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Repair cancelled. ")
            << ConsoleTheme::FormatLiteral("The truck remains in Breakdown status and no expense was recorded.\n");

        return;
    }

    FinancialTransaction repairTransaction;

    repairTransaction.transactionEpoch =
        repairComplete;

    repairTransaction.category =
        "Repair";

    repairTransaction.description =
        repairType;

    repairTransaction.amount =
        totalCost;

    repairTransaction.truckUnitNumber =
        unitNumber;

    repairTransaction.partsCost =
        partsCost;

    repairTransaction.laborCost =
        laborCost;

    repairTransaction.serviceCallCharge =
        serviceCallCharge;

    company.RecordExpenseTransaction(
        repairTransaction
    );

    selectedTruck->
        ClearBreakdown();

    selectedTruck->
        SetStatus(
            "Available"
        );

    selectedTruck->
        SetAvailableAtEpoch(
            repairComplete
        );

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== REPAIR COMPLETED ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Truck: #")
        << unitNumber
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Repair: ")
        << repairType
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Parts: $")
        << partsCost
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Labor: $")
        << laborCost
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Service Call: $")
        << serviceCallCharge
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Total Repair Expense: $")
        << totalCost
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Repair Time: ")
        << repairHours
        << ConsoleTheme::FormatLiteral(" hours\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("Repair Start: ")
        << HOSManager::FormatDateTime(
            repairStart
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Truck Scheduled Available: ")
        << HOSManager::FormatDateTime(
            repairComplete
        )
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Company Expenses: $")
        << company.GetExpenses()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company Balance: $")
        << company.GetBalance()
        << ConsoleTheme::FormatLiteral("\n");

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Financial Status: NEGATIVE BALANCE\n");

        std::cout
            << ConsoleTheme::FormatLiteral("The repair was still completed. ")
            << ConsoleTheme::FormatLiteral("Future revenue will reduce the deficit.\n");
    }

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("======================================\n");
}

void RepairManager::Run(
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
            DisplayActiveBreakdowns(
                company
            );
            break;

        case 2:
            RepairTruck(
                company
            );
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