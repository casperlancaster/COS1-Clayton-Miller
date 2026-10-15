#pragma once

#include "Company.h"

class BankingManager
{
public:
    static void Run(const Company& company);

private:
    static void ShowMenu();
    static void ShowAccountSummary(const Company& company);
    static void ShowRevenue(const Company& company);
    static void ShowExpenses(const Company& company);
};
