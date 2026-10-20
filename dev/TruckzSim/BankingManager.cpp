#include "BankingManager.h"

#include <iomanip>
#include <iostream>

#include "HOSManager.h"
#include "InputManager.h"

void BankingManager::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== COMPANY BANKING ==========\n";
    std::cout << "1. View Account Summary\n";
    std::cout << "2. View Income History\n";
    std::cout << "3. View Expense History\n";
    std::cout << "4. View All Transactions\n";
    std::cout << "5. View Driver Payroll History\n";
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

    std::cout
        << "Recorded Transactions: "
        << company.GetFinancialTransactions().size()
        << "\n";

    if (company.GetBalance() < 0.0)
    {
        std::cout
            << "Status: NEGATIVE BALANCE\n";

        std::cout
            << "Operating expenses are still allowed. "
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

void BankingManager::DisplayTransaction(
    const FinancialTransaction& transaction)
{
    std::cout
        << "\n-------------------------------------\n";

    std::cout
        << "Type: "
        << transaction.type
        << "\n";

    std::cout
        << "Category: "
        << transaction.category
        << "\n";

    if (transaction.transactionEpoch > 0)
    {
        std::cout
            << "Date: "
            << HOSManager::FormatDateTime(
                transaction.transactionEpoch
            )
            << "\n";
    }
    else
    {
        std::cout
            << "Date: Imported from legacy save data\n";
    }

    if (!transaction.description.empty())
    {
        std::cout
            << "Description: "
            << transaction.description
            << "\n";
    }

    if (transaction.loadNumber > 0)
    {
        std::cout
            << "Load: #"
            << transaction.loadNumber
            << "\n";
    }

    if (transaction.truckUnitNumber > 0)
    {
        std::cout
            << "Truck: #"
            << transaction.truckUnitNumber
            << "\n";
    }

    if (transaction.primaryDriverID > 0)
    {
        if (transaction.category ==
            "Driver Payroll")
        {
            std::cout
                << "Driver: #"
                << transaction.primaryDriverID
                << "\n";
        }
        else
        {
            std::cout
                << "Primary Driver: #"
                << transaction.primaryDriverID
                << "\n";
        }
    }

    if (transaction.secondDriverID > 0)
    {
        std::cout
            << "Team Driver: #"
            << transaction.secondDriverID
            << "\n";
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    if (transaction.gallons > 0.0)
    {
        std::cout
            << "Gallons: "
            << transaction.gallons
            << "\n";
    }

    if (transaction.pricePerGallon > 0.0)
    {
        std::cout
            << "Price Per Gallon: $"
            << transaction.pricePerGallon
            << "\n";
    }

    if (transaction.partsCost > 0.0)
    {
        std::cout
            << "Parts: $"
            << transaction.partsCost
            << "\n";
    }

    if (transaction.laborCost > 0.0)
    {
        std::cout
            << "Labor: $"
            << transaction.laborCost
            << "\n";
    }

    if (transaction.serviceCallCharge > 0.0)
    {
        std::cout
            << "Service Call: $"
            << transaction.serviceCallCharge
            << "\n";
    }

    std::cout
        << "Amount: $"
        << transaction.amount
        << "\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);
}

void BankingManager::ShowIncomeHistory(
    const Company& company)
{
    std::cout
        << "\n========== INCOME HISTORY ==========\n";

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
            << "No income transactions have been recorded yet.\n";
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "\nDisplayed Income Total: $"
        << displayedTotal
        << "\n";

    std::cout
        << "Company Revenue Total:  $"
        << company.GetRevenue()
        << "\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "====================================\n";
}

void BankingManager::ShowExpenseHistory(
    const Company& company)
{
    std::cout
        << "\n========== EXPENSE HISTORY ==========\n";

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
            << "No expense transactions have been recorded yet.\n";
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "\nDisplayed Expense Total: $"
        << displayedTotal
        << "\n";

    std::cout
        << "Company Expense Total:   $"
        << company.GetExpenses()
        << "\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "=====================================\n";
}

void BankingManager::ShowAllTransactions(
    const Company& company)
{
    std::cout
        << "\n========== ALL TRANSACTIONS ==========\n";

    const std::vector<FinancialTransaction>& transactions =
        company.GetFinancialTransactions();

    if (transactions.empty())
    {
        std::cout
            << "No financial transactions have been recorded yet.\n";
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
        << "\nRevenue:  $"
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

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "======================================\n";
}

void BankingManager::ShowPayrollHistory(
    const Company& company)
{
    std::cout
        << "\n========== DRIVER PAYROLL HISTORY ==========\n";

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
            << "No driver payroll transactions "
            << "have been recorded yet.\n";
    }

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "\nDriver Payroll Total: $"
        << payrollTotal
        << "\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "============================================\n";
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