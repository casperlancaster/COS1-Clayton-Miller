#pragma once

#include "Company.h"

class SettingsManager
{
public:
    static void Run(Company& company);
    static void ApplyDateTimeSettings(
        const Company& company
    );

private:
    static void ShowMenu();
    static void ConfigureDateTime(
        Company& company
    );
    static void ShowCurrentSettings(
        const Company& company
    );
    static void EditTruck(
        Company& company
    );
    static void EditDriver(
        Company& company
    );
    static void EditLoad(
        Company& company
    );
    static void ResetDisplaySettings(
        Company& company
    );
    static long long ReadDeadline();
};
