#include "BankingManager.h"

#include <iomanip>
#include <iostream>

#include "InputManager.h"

void BankingManager::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== COMPANY BANKING ==========\n";
    std::cout << "1. View Account Summary\n";
    std::cout << "2. View Revenue\n";
    std::cout << "3. View Expenses\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=====================================\n";
}

void BankingManager::ShowAccountSummary(
    const Company& company)
{
    std::cout
        << "\n========== ACCOUNT SUMMARY ==========\n";

    std::cout
        << "Company: "
        << company.GetCompanyName()
        << "\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Revenue:  $"
        << company.GetRevenue()
        << "\n";

    std::cout
        << "Expenses: $"
        << company.GetExpenses()
        << "\n";

    std::cout
        << "Balance:  $"
        << company.GetBalance()
        << "\n";

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << "Status: NEGATIVE BALANCE\n";

        std::cout
            << "Fuel, maintenance, and repair expenses are still allowed. "
            << "Future revenue will reduce the deficit.\n";
    }
    else if (company.GetBalance() == 0.0)
    {
        std::cout
            << "Status: No available company funds yet.\n";
    }
    else
    {
        std::cout
            << "Status: Company account is positive.\n";
    }

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "=====================================\n";
}

void BankingManager::ShowRevenue(
    const Company& company)
{
    std::cout
        << "\n========== COMPANY REVENUE ==========\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Total revenue from completed loads: $"
        << company.GetRevenue()
        << "\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "=====================================\n";
}

void BankingManager::ShowExpenses(
    const Company& company)
{
    std::cout
        << "\n========== COMPANY EXPENSES ==========\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Total recorded expenses: $"
        << company.GetExpenses()
        << "\n";

    std::cout
        << "Current expenses include maintenance services "
        << "and fuel purchases.\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "======================================\n";
}

void BankingManager::Run(
    const Company& company)
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
            ShowAccountSummary(
                company
            );
            break;

        case 2:
            ShowRevenue(
                company
            );
            break;

        case 3:
            ShowExpenses(
                company
            );
            break;

        case 0:
            break;

        default:
            std::cout
                << "\nInvalid selection. "
                << "Please choose 0, 1, 2, or 3.\n";

            break;
        }
    }
}