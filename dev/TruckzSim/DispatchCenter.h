#pragma once

#include "Company.h"
#include "DispatchManager.h"

class DispatchCenter
{
public:
    static void Run(DispatchManager& dispatchManager);

private:
    static void ShowMenu();

    static const Load* FindLoad(const Company& company, int loadNumber);
    static const Driver* FindDriver(const Company& company, int driverID);
    static const Truck* FindTruck(const Company& company, int truckUnitNumber);

    static bool HasAvailableLoad(const Company& company);
    static bool HasAvailableDriver(const Company& company);
    static bool HasAvailableTruck(const Company& company);

    static void DisplayAvailableResources(const Company& company);
    static void DisplayAssignedLoads(const Company& company);
};
#pragma once
