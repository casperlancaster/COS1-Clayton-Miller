#pragma once

#include "Company.h"

class FuelManager
{
public:
    static void Run(Company& company);

private:
    static void ShowMenu();

    static Truck* FindTruck(
        Company& company,
        int unitNumber
    );

    static void DisplayFuelStatus(
        const Company& company
    );

    static void RefuelTruck(
        Company& company
    );
};