#pragma once

#include "Company.h"

class BankingManager
{
public:
    static void Run(const Company& company);

private:
    static void ShowMenu();
    static void ShowAccountSummary(const Company& company);
    static void ShowIncomeHistory(const Company& company);
    static void ShowExpenseHistory(const Company& company);
    static void ShowAllTransactions(const Company& company);

    static void DisplayTransaction(
        const FinancialTransaction& transaction
    );
};