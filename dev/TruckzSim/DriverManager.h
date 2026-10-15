#pragma once

#include "Company.h"

class DriverManager
{
public:
    static void Run(Company& company);

private:
    static void ShowMenu();
    static void AddDriver(Company& company);
    static void DisplayDrivers(const Company& company);
};
