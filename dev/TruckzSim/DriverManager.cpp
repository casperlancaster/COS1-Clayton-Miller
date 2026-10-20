#include "DriverManager.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "Driver.h"
#include "HOSManager.h"
#include "InputManager.h"
#include "PayrollManager.h"

namespace
{
    constexpr double EPSILON =
        0.000001;

    long long GetRestStartTime(
        const Driver& driver)
    {
        long long currentTime =
            HOSManager::GetCurrentEpoch();

        if (driver.GetAvailableAtEpoch() >
            currentTime)
        {
            return driver.
                GetAvailableAtEpoch();
        }

        return currentTime;
    }

    double GetHoursNeededForReset(
        const Driver& driver,
        long long restStartTime)
    {
        double projectedHours =
            driver.GetProjectedDrivingHoursAt(
                restStartTime
            );

        if (projectedHours >=
            HOSManager::GetMaxDrivingHours() -
            EPSILON)
        {
            return 0.0;
        }

        double projectedRest =
            driver.GetProjectedRestHoursAt(
                restStartTime
            );

        double remainingRest =
            HOSManager::GetRequiredRestHours() -
            projectedRest;

        if (remainingRest < 0.0)
        {
            remainingRest = 0.0;
        }

        return remainingRest;
    }

    void ApplyDriverRest(
        Driver& driver,
        double restHours)
    {
        long long restStartTime =
            GetRestStartTime(
                driver
            );

        double currentDrivingHours =
            driver.GetProjectedDrivingHoursAt(
                restStartTime
            );

        double currentRestHours =
            driver.GetProjectedRestHoursAt(
                restStartTime
            );

        double remainingRest =
            HOSManager::GetRequiredRestHours() -
            currentRestHours;

        if (remainingRest < 0.0)
        {
            remainingRest = 0.0;
        }

        double actualRestHours =
            std::min(
                restHours,
                remainingRest
            );

        long long restSeconds =
            std::llround(
                actualRestHours *
                3600.0
            );

        long long restCompleteTime =
            restStartTime +
            restSeconds;

        double newRestHours =
            currentRestHours +
            actualRestHours;

        if (newRestHours >=
            HOSManager::GetRequiredRestHours() -
            EPSILON)
        {
            driver.SetHOSState(
                HOSManager::GetMaxDrivingHours(),
                0.0,
                restCompleteTime
            );
        }
        else
        {
            driver.SetHOSState(
                currentDrivingHours,
                newRestHours,
                restCompleteTime
            );
        }
    }
}

void DriverManager::ShowMenu()
{
    std::cout
        << "\n========== DRIVER MANAGEMENT ==========\n";

    std::cout
        << "1. Add Driver\n";

    std::cout
        << "2. View Drivers\n";

    std::cout
        << "3. Rest Driver\n";

    std::cout
        << "4. Rest All Available Drivers\n";

    std::cout
        << "0. Return to Main Menu\n";

    std::cout
        << "=======================================\n";
}

void DriverManager::AddDriver(
    Company& company)
{
    Driver newDriver;

    std::cout
        << "\n========== ADD DRIVER ==========\n";

    int driverID = 0;

    while (true)
    {
        driverID =
            InputManager::ReadPositiveInt(
                "Enter Driver ID: ",
                "driver ID"
            );

        if (!company.HasDriverID(
            driverID))
        {
            break;
        }

        std::cout
            << "Driver ID "
            << driverID
            << " already exists. "
            << "Please enter a unique Driver ID.\n";
    }

    std::string driverName =
        InputManager::ReadRequiredText(
            "Enter driver name: ",
            "Driver name"
        );

    newDriver.SetDriverID(
        driverID
    );

    newDriver.SetName(
        driverName
    );

    newDriver.SetStatus(
        "Available"
    );

    newDriver.ResetDrivingHours();

    company.AddDriver(
        newDriver
    );

    std::cout
        << "\nDriver "
        << driverName
        << " added successfully.\n";

    std::cout
        << "Starting HOS: "
        << std::fixed
        << std::setprecision(1)
        << HOSManager::
        GetMaxDrivingHours()
        << " driving hours available.\n"
        << std::defaultfloat
        << std::setprecision(6);
}

void DriverManager::DisplayDrivers(
    const Company& company)
{
    const std::vector<Driver>& drivers =
        company.GetDrivers();

    long long now =
        HOSManager::GetCurrentEpoch();

    std::cout
        << "\n========== COMPANY DRIVERS ==========\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Solo Pay Rate: $"
        << PayrollManager::GetSoloRatePerMile()
        << " per mile\n";

    std::cout
        << "Team Pay Rate: $"
        << PayrollManager::GetTeamRatePerMile()
        << " per mile per driver\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    if (drivers.empty())
    {
        std::cout
            << "No drivers have been added yet.\n";
    }
    else
    {
        for (const Driver& driver :
            drivers)
        {
            long long planningTime =
                now;

            if (driver.GetAvailableAtEpoch() >
                planningTime)
            {
                planningTime =
                    driver.GetAvailableAtEpoch();
            }

            double projectedHours =
                driver.
                GetProjectedDrivingHoursAt(
                    planningTime
                );

            double projectedRest =
                driver.
                GetProjectedRestHoursAt(
                    planningTime
                );

            std::cout
                << "\nDriver ID: "
                << driver.GetDriverID()
                << "\n";

            std::cout
                << "Name: "
                << driver.GetName()
                << "\n";

            std::cout
                << "Status: "
                << driver.GetStatus()
                << "\n";

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << "Driving Hours Remaining: "
                << projectedHours
                << " / "
                << HOSManager::
                GetMaxDrivingHours()
                << "\n";

            std::cout
                << "Rest Progress: "
                << projectedRest
                << " / "
                << HOSManager::
                GetRequiredRestHours()
                << " hours\n";

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            if (driver.GetAvailableAtEpoch() >
                now)
            {
                std::cout
                    << "Scheduled Available: "
                    << HOSManager::
                    FormatDateTime(
                        driver.
                        GetAvailableAtEpoch()
                    )
                    << "\n";
            }
            else
            {
                std::cout
                    << "Scheduled Available: Now\n";
            }
        }
    }

    std::cout
        << "=====================================\n";
}

void DriverManager::RestDriver(
    Company& company)
{
    std::vector<Driver>& drivers =
        company.GetDrivers();

    if (drivers.empty())
    {
        std::cout
            << "\nNo drivers have been added yet.\n";

        return;
    }

    std::cout
        << "\n========== DRIVER REST MANAGEMENT ==========\n";

    for (const Driver& driver :
        drivers)
    {
        long long planningTime =
            GetRestStartTime(
                driver
            );

        double projectedHours =
            driver.GetProjectedDrivingHoursAt(
                planningTime
            );

        double projectedRest =
            driver.GetProjectedRestHoursAt(
                planningTime
            );

        std::cout
            << "Driver #"
            << driver.GetDriverID()
            << " | "
            << driver.GetName()
            << " | "
            << driver.GetStatus();

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << " | HOS: "
            << projectedHours
            << " / "
            << HOSManager::
            GetMaxDrivingHours();

        std::cout
            << " | Rest: "
            << projectedRest
            << " / "
            << HOSManager::
            GetRequiredRestHours()
            << "\n";

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    std::cout
        << "============================================\n";

    int driverID =
        InputManager::ReadInt(
            "Enter Driver ID to rest (0 to cancel): "
        );

    if (driverID == 0)
    {
        std::cout
            << "Driver rest cancelled.\n";

        return;
    }

    Driver* selectedDriver =
        company.FindDriver(
            driverID
        );

    if (selectedDriver == nullptr)
    {
        std::cout
            << "Driver ID "
            << driverID
            << " was not found.\n";

        return;
    }

    if (selectedDriver->GetStatus() !=
        "Available")
    {
        std::cout
            << "Driver #"
            << driverID
            << " cannot be manually rested "
            << "while "
            << selectedDriver->GetStatus()
            << ".\n";

        std::cout
            << "Complete the driver's active load first.\n";

        return;
    }

    long long restStartTime =
        GetRestStartTime(
            *selectedDriver
        );

    double currentHours =
        selectedDriver->
        GetProjectedDrivingHoursAt(
            restStartTime
        );

    double currentRest =
        selectedDriver->
        GetProjectedRestHoursAt(
            restStartTime
        );

    if (currentHours >=
        HOSManager::GetMaxDrivingHours() -
        EPSILON)
    {
        std::cout
            << "\nDriver #"
            << driverID
            << " already has a full "
            << std::fixed
            << std::setprecision(2)
            << HOSManager::
            GetMaxDrivingHours()
            << " driving hours available.\n"
            << std::defaultfloat
            << std::setprecision(6);

        return;
    }

    std::cout
        << "\n========== REST OPTIONS ==========\n";

    std::cout
        << "Driver: #"
        << selectedDriver->GetDriverID()
        << " - "
        << selectedDriver->GetName()
        << "\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Driving Hours Remaining: "
        << currentHours
        << " / "
        << HOSManager::
        GetMaxDrivingHours()
        << "\n";

    std::cout
        << "Current Rest Progress: "
        << currentRest
        << " / "
        << HOSManager::
        GetRequiredRestHours()
        << "\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    if (restStartTime >
        HOSManager::GetCurrentEpoch())
    {
        std::cout
            << "Rest Can Begin: "
            << HOSManager::
            FormatDateTime(
                restStartTime
            )
            << "\n";
    }
    else
    {
        std::cout
            << "Rest Can Begin: Now\n";
    }

    std::cout
        << "\n1. Rest Until Full HOS Reset\n";

    std::cout
        << "2. Advance Off-Duty Time Manually\n";

    std::cout
        << "0. Cancel\n";

    std::cout
        << "==================================\n";

    int restChoice =
        InputManager::ReadMenuChoice(
            "Select an option: ",
            0,
            2
        );

    if (restChoice == 0)
    {
        std::cout
            << "Driver rest cancelled.\n";

        return;
    }

    double hoursNeeded =
        GetHoursNeededForReset(
            *selectedDriver,
            restStartTime
        );

    double restHours =
        hoursNeeded;

    if (restChoice == 2)
    {
        restHours =
            InputManager::ReadPositiveDouble(
                "Enter off-duty hours to advance: ",
                "off-duty hours"
            );

        if (restHours >
            hoursNeeded)
        {
            std::cout
                << "Only "
                << std::fixed
                << std::setprecision(2)
                << hoursNeeded
                << " more rest hours are needed "
                << "for a full HOS reset.\n";

            std::cout
                << "Rest time will be limited to "
                << hoursNeeded
                << " hours.\n"
                << std::defaultfloat
                << std::setprecision(6);

            restHours =
                hoursNeeded;
        }
    }

    long long restSeconds =
        std::llround(
            restHours *
            3600.0
        );

    long long restCompleteTime =
        restStartTime +
        restSeconds;

    double projectedRestAfter =
        currentRest +
        restHours;

    bool fullReset =
        projectedRestAfter >=
        HOSManager::
        GetRequiredRestHours() -
        EPSILON;

    std::cout
        << "\n========== REST CONFIRMATION ==========\n";

    std::cout
        << "Driver: #"
        << selectedDriver->GetDriverID()
        << " - "
        << selectedDriver->GetName()
        << "\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Off-Duty Time: "
        << restHours
        << " hours\n";

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "Rest Complete: "
        << HOSManager::
        FormatDateTime(
            restCompleteTime
        )
        << "\n";

    if (fullReset)
    {
        std::cout
            << "Result: Full HOS reset to "
            << std::fixed
            << std::setprecision(2)
            << HOSManager::
            GetMaxDrivingHours()
            << " driving hours.\n"
            << std::defaultfloat
            << std::setprecision(6);
    }
    else
    {
        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << "Resulting Rest Progress: "
            << projectedRestAfter
            << " / "
            << HOSManager::
            GetRequiredRestHours()
            << " hours\n";

        std::cout
            << "Driving Hours Remaining: "
            << currentHours
            << "\n";

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    std::cout
        << "=======================================\n";

    if (!InputManager::ReadYesNo(
        "Schedule this driver rest? (Y/N): "))
    {
        std::cout
            << "Driver rest cancelled. "
            << "No HOS changes were made.\n";

        return;
    }

    ApplyDriverRest(
        *selectedDriver,
        restHours
    );

    std::cout
        << "\nDriver #"
        << selectedDriver->GetDriverID()
        << " scheduled for "
        << std::fixed
        << std::setprecision(2)
        << restHours
        << " off-duty hours.\n"
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << "Scheduled Available: "
        << HOSManager::
        FormatDateTime(
            selectedDriver->
            GetAvailableAtEpoch()
        )
        << "\n";

    if (fullReset)
    {
        std::cout
            << "Driver will return with a full "
            << std::fixed
            << std::setprecision(2)
            << HOSManager::
            GetMaxDrivingHours()
            << " driving hours available.\n"
            << std::defaultfloat
            << std::setprecision(6);
    }
    else
    {
        std::cout
            << "Driver will retain "
            << std::fixed
            << std::setprecision(2)
            << currentHours
            << " driving hours with "
            << projectedRestAfter
            << " / "
            << HOSManager::
            GetRequiredRestHours()
            << " rest hours accumulated.\n"
            << std::defaultfloat
            << std::setprecision(6);
    }
}

void DriverManager::RestAllDrivers(
    Company& company)
{
    std::vector<Driver>& drivers =
        company.GetDrivers();

    if (drivers.empty())
    {
        std::cout
            << "\nNo drivers have been added yet.\n";

        return;
    }

    int driversNeedingRest = 0;

    std::cout
        << "\n========== REST ALL AVAILABLE DRIVERS ==========\n";

    for (const Driver& driver :
        drivers)
    {
        if (driver.GetStatus() !=
            "Available")
        {
            continue;
        }

        long long restStartTime =
            GetRestStartTime(
                driver
            );

        double hoursNeeded =
            GetHoursNeededForReset(
                driver,
                restStartTime
            );

        if (hoursNeeded <=
            EPSILON)
        {
            continue;
        }

        driversNeedingRest++;

        std::cout
            << "Driver #"
            << driver.GetDriverID()
            << " - "
            << driver.GetName();

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << " | Rest Needed: "
            << hoursNeeded
            << " hours\n";

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    if (driversNeedingRest == 0)
    {
        std::cout
            << "No available drivers currently "
            << "need an HOS reset.\n";

        std::cout
            << "================================================\n";

        return;
    }

    std::cout
        << "================================================\n";

    std::cout
        << driversNeedingRest
        << " available driver";

    if (driversNeedingRest != 1)
    {
        std::cout << "s";
    }

    std::cout
        << " will be rested until full HOS reset.\n";

    if (!InputManager::ReadYesNo(
        "Rest all listed drivers? (Y/N): "))
    {
        std::cout
            << "Rest-all cancelled. "
            << "No HOS changes were made.\n";

        return;
    }

    int restedDrivers = 0;

    for (Driver& driver :
        drivers)
    {
        if (driver.GetStatus() !=
            "Available")
        {
            continue;
        }

        long long restStartTime =
            GetRestStartTime(
                driver
            );

        double hoursNeeded =
            GetHoursNeededForReset(
                driver,
                restStartTime
            );

        if (hoursNeeded <=
            EPSILON)
        {
            continue;
        }

        ApplyDriverRest(
            driver,
            hoursNeeded
        );

        restedDrivers++;

        std::cout
            << "Driver #"
            << driver.GetDriverID()
            << " scheduled available at "
            << HOSManager::
            FormatDateTime(
                driver.GetAvailableAtEpoch()
            )
            << " with full HOS.\n";
    }

    std::cout
        << "\n"
        << restedDrivers
        << " driver";

    if (restedDrivers != 1)
    {
        std::cout << "s";
    }

    std::cout
        << " scheduled for full HOS reset.\n";
}

void DriverManager::Run(
    Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                4
            );

        switch (choice)
        {
        case 1:
            AddDriver(
                company
            );
            break;

        case 2:
            DisplayDrivers(
                company
            );
            break;

        case 3:
            RestDriver(
                company
            );
            break;

        case 4:
            RestAllDrivers(
                company
            );
            break;

        case 0:
            break;
        }
    }
}