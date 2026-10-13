#pragma once

#include "Company.h"

class MaintenanceManager
{
public:
    static void Run(Company& company);

private:
    static void ShowMenu();
    static void DisplayMaintenanceStatus(const Company& company);
    static void SendTruckToMaintenance(Company& company);
    static void CompleteTruckMaintenance(Company& company);

    static Truck* FindTruck(Company& company, int unitNumber);
};
