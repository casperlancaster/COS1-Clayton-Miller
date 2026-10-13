#pragma once

#include "Company.h"

class FleetManager
{
public:
    static void Run(Company& company);

private:
    static void ShowMenu();
    static void AddTruck(Company& company);
    static void DisplayTrucks(const Company& company);
};
