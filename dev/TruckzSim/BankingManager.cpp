#include "ConsoleTheme.h"
#include "BankingManager.h"

#include <iomanip>
#include <iostream>

#include "HOSManager.h"
#include "InputManager.h"

void BankingManager::ShowMenu()
{
    std::cout << ConsoleTheme::FormatLiteral("\n");
    std::cout << ConsoleTheme::FormatLiteral("========== COMPANY BANKING ==========\n");
    std::cout << ConsoleTheme::FormatLiteral("1. View Account Summary\n");
    std::cout << ConsoleTheme::FormatLiteral("2. View Income History\n");
    std::cout << ConsoleTheme::FormatLiteral("3. View Expense History\n");
    std::cout << ConsoleTheme::FormatLiteral("4. View All Transactions\n");
    std::cout << ConsoleTheme::FormatLiteral("5. View Driver Payroll History\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Return to Main Menu\n");
    std::cout << ConsoleTheme::FormatLiteral("=====================================\n");
}

void BankingManager::ShowAccountSummary(
    const Company& company)
{
    std::cout
        << ConsoleTheme::FormatLiteral("\n========== ACCOUNT SUMMARY ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company: ")
        << company.GetCompanyName()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Revenue:  $")
        << ConsoleTheme::Begin(ConsoleTheme::Color::Lime)
        << company.GetRevenue()
        << ConsoleTheme::End()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Expenses: $")
        << ConsoleTheme::Begin(ConsoleTheme::Color::Yellow)
        << company.GetExpenses()
        << ConsoleTheme::End()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Balance:  $")
        << ConsoleTheme::Begin(company.GetBalance() < 0.0
            ? ConsoleTheme::Color::Red : ConsoleTheme::Color::Lime)
        << company.GetBalance()
        << ConsoleTheme::End()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Recorded Transactions: ")
        << company.GetFinancialTransactions().size()
        << ConsoleTheme::FormatLiteral("\n");

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Status: NEGATIVE BALANCE\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Operating expenses are still allowed. ")
            << ConsoleTheme::FormatLiteral("Future revenue will reduce the deficit.\n");
    }
    else if (company.GetBalance() == 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Status: No available company funds yet.\n");
    }
    else
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Status: Company account is positive.\n");
    }

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("=====================================\n");
}

void BankingManager::DisplayTransaction(
    const FinancialTransaction& transaction)
{
    std::cout
        << ConsoleTheme::FormatLiteral("\n-------------------------------------\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Type: ")
        << transaction.type
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Category: ")
        << transaction.category
        << ConsoleTheme::FormatLiteral("\n");

    if (transaction.transactionEpoch > 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Date: ")
            << HOSManager::FormatDateTime(
                transaction.transactionEpoch
            )
            << ConsoleTheme::FormatLiteral("\n");
    }
    else
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Date: Imported from legacy save data\n");
    }

    if (!transaction.description.empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Description: ")
            << transaction.description
            << ConsoleTheme::FormatLiteral("\n");
    }

    if (transaction.loadNumber > 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Load: #")
            << transaction.loadNumber
            << ConsoleTheme::FormatLiteral("\n");
    }

    if (transaction.truckUnitNumber > 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Truck: #")
            << transaction.truckUnitNumber
            << ConsoleTheme::FormatLiteral("\n");
    }

    if (transaction.primaryDriverID > 0)
    {
        if (transaction.category ==
            "Driver Payroll")
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Driver: #")
                << transaction.primaryDriverID
                << ConsoleTheme::FormatLiteral("\n");
        }
        else
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Primary Driver: #")
                << transaction.primaryDriverID
                << ConsoleTheme::FormatLiteral("\n");
        }
    }

    if (transaction.secondDriverID > 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Team Driver: #")
            << transaction.secondDriverID
            << ConsoleTheme::FormatLiteral("\n");
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    if (transaction.gallons > 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Gallons: ")
            << transaction.gallons
            << ConsoleTheme::FormatLiteral("\n");
    }

    if (transaction.pricePerGallon > 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Price Per Gallon: $")
            << transaction.pricePerGallon
            << ConsoleTheme::FormatLiteral("\n");
    }

    if (transaction.partsCost > 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Parts: $")
            << transaction.partsCost
            << ConsoleTheme::FormatLiteral("\n");
    }

    if (transaction.laborCost > 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Labor: $")
            << transaction.laborCost
            << ConsoleTheme::FormatLiteral("\n");
    }

    if (transaction.serviceCallCharge > 0.0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Service Call: $")
            << transaction.serviceCallCharge
            << ConsoleTheme::FormatLiteral("\n");
    }

    std::cout
        << ConsoleTheme::FormatLiteral("Amount: $")
        << transaction.amount
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);
}

void BankingManager::ShowIncomeHistory(
    const Company& company)
{
    std::cout
        << ConsoleTheme::FormatLiteral("\n========== INCOME HISTORY ==========\n");

    bool foundIncome = false;
    double displayedTotal = 0.0;

    for (const FinancialTransaction& transaction :
        company.GetFinancialTransactions())
    {
        if (transaction.type != "Income")
        {
            continue;
        }

        foundIncome = true;
        displayedTotal += transaction.amount;

        DisplayTransaction(
            transaction
        );
    }

    if (!foundIncome)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No income transactions have been recorded yet.\n");
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("\nDisplayed Income Total: $")
        << displayedTotal
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company Revenue Total:  $")
        << company.GetRevenue()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("====================================\n");
}

void BankingManager::ShowExpenseHistory(
    const Company& company)
{
    std::cout
        << ConsoleTheme::FormatLiteral("\n========== EXPENSE HISTORY ==========\n");

    bool foundExpense = false;
    double displayedTotal = 0.0;

    for (const FinancialTransaction& transaction :
        company.GetFinancialTransactions())
    {
        if (transaction.type != "Expense")
        {
            continue;
        }

        foundExpense = true;
        displayedTotal += transaction.amount;

        DisplayTransaction(
            transaction
        );
    }

    if (!foundExpense)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No expense transactions have been recorded yet.\n");
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("\nDisplayed Expense Total: $")
        << displayedTotal
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Company Expense Total:   $")
        << company.GetExpenses()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("=====================================\n");
}

void BankingManager::ShowAllTransactions(
    const Company& company)
{
    std::cout
        << ConsoleTheme::FormatLiteral("\n========== ALL TRANSACTIONS ==========\n");

    const std::vector<FinancialTransaction>& transactions =
        company.GetFinancialTransactions();

    if (transactions.empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No financial transactions have been recorded yet.\n");
    }
    else
    {
        for (const FinancialTransaction& transaction :
            transactions)
        {
            DisplayTransaction(
                transaction
            );
        }
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("\nRevenue:  $")
        << company.GetRevenue()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Expenses: $")
        << company.GetExpenses()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Balance:  $")
        << company.GetBalance()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("======================================\n");
}

void BankingManager::ShowPayrollHistory(
    const Company& company)
{
    std::cout
        << ConsoleTheme::FormatLiteral("\n========== DRIVER PAYROLL HISTORY ==========\n");

    bool foundPayroll = false;
    double payrollTotal = 0.0;

    for (const FinancialTransaction& transaction :
        company.GetFinancialTransactions())
    {
        if (transaction.type != "Expense" ||
            transaction.category !=
            "Driver Payroll")
        {
            continue;
        }

        foundPayroll = true;
        payrollTotal +=
            transaction.amount;

        DisplayTransaction(
            transaction
        );
    }

    if (!foundPayroll)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No driver payroll transactions ")
            << ConsoleTheme::FormatLiteral("have been recorded yet.\n");
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("\nDriver Payroll Total: $")
        << payrollTotal
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("============================================\n");
}

void BankingManager::Run(
    const Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                5
            );

        switch (choice)
        {
        case 1:
            ShowAccountSummary(
                company
            );
            break;

        case 2:
            ShowIncomeHistory(
                company
            );
            break;

        case 3:
            ShowExpenseHistory(
                company
            );
            break;

        case 4:
            ShowAllTransactions(
                company
            );
            break;

        case 5:
            ShowPayrollHistory(
                company
            );
            break;

        case 0:
            break;
        }
    }
}