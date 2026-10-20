#pragma once

#include "Company.h"
#include "Truck.h"

class RepairManager
{
public:
    static void Run(Company& company);

    static bool RollForBreakdown(
        Truck& truck,
        double tripDistanceMiles
    );

private:
    static void ShowMenu();

    static Truck* FindTruck(
        Company& company,
        int unitNumber
    );

    static void DisplayActiveBreakdowns(
        const Company& company
    );

    static void RepairTruck(
        Company& company
    );
};