#pragma once

#include "Company.h"
#include "DispatchManager.h"

class TruckzSimApp
{
private:
    Company company;
    DispatchManager dispatchManager;

    bool RunStartupMenu();
    void SetupCompany();

    void ShowMainMenu() const;
    void ShowCompanyOverview() const;

public:
    TruckzSimApp();

    void Run();
};