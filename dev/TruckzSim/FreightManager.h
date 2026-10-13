#pragma once

#include "Company.h"

class FreightManager
{
public:
    static void Run(Company& company);

private:
    static void ShowMenu();
    static void AddLoad(Company& company);
    static void DisplayLoads(const Company& company);
};
