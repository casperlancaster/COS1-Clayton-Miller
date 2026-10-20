#include "ConsoleTheme.h"
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
        << ConsoleTheme::FormatLiteral("\n========== DRIVER MANAGEMENT ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("1. Add Driver\n");

    std::cout
        << ConsoleTheme::FormatLiteral("2. View Drivers\n");

    std::cout
        << ConsoleTheme::FormatLiteral("3. Rest Driver\n");

    std::cout
        << ConsoleTheme::FormatLiteral("4. Rest All Available Drivers\n");

    std::cout
        << ConsoleTheme::FormatLiteral("0. Return to Main Menu\n");

    std::cout
        << ConsoleTheme::FormatLiteral("=======================================\n");
}

void DriverManager::AddDriver(
    Company& company)
{
    Driver newDriver;

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== ADD DRIVER ==========\n");

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
            << ConsoleTheme::FormatLiteral("Driver ID ")
            << driverID
            << ConsoleTheme::FormatLiteral(" already exists. ")
            << ConsoleTheme::FormatLiteral("Please enter a unique Driver ID.\n");
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
        << ConsoleTheme::FormatLiteral("\nDriver ")
        << driverName
        << ConsoleTheme::FormatLiteral(" added successfully.\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Starting HOS: ")
        << std::fixed
        << std::setprecision(1)
        << HOSManager::
        GetMaxDrivingHours()
        << ConsoleTheme::FormatLiteral(" driving hours available.\n")
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
        << ConsoleTheme::FormatLiteral("\n========== COMPANY DRIVERS ==========\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Solo Pay Rate: $")
        << PayrollManager::GetSoloRatePerMile()
        << ConsoleTheme::FormatLiteral(" per mile\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Team Pay Rate: $")
        << PayrollManager::GetTeamRatePerMile()
        << ConsoleTheme::FormatLiteral(" per mile per driver\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    if (drivers.empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No drivers have been added yet.\n");
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
                << ConsoleTheme::FormatLiteral("\nDriver ID: ")
                << driver.GetDriverID()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Name: ")
                << driver.GetName()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Status: ")
                << ConsoleTheme::Status(driver.GetStatus())
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << ConsoleTheme::FormatLiteral("Driving Hours Remaining: ")
                << projectedHours
                << ConsoleTheme::FormatLiteral(" / ")
                << HOSManager::
                GetMaxDrivingHours()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Rest Progress: ")
                << projectedRest
                << ConsoleTheme::FormatLiteral(" / ")
                << HOSManager::
                GetRequiredRestHours()
                << ConsoleTheme::FormatLiteral(" hours\n");

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            if (driver.GetAvailableAtEpoch() >
                now)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Scheduled Available: ")
                    << HOSManager::
                    FormatDateTime(
                        driver.
                        GetAvailableAtEpoch()
                    )
                    << ConsoleTheme::FormatLiteral("\n");
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Scheduled Available: Now\n");
            }
        }
    }

    std::cout
        << ConsoleTheme::FormatLiteral("=====================================\n");
}

void DriverManager::RestDriver(
    Company& company)
{
    std::vector<Driver>& drivers =
        company.GetDrivers();

    if (drivers.empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nNo drivers have been added yet.\n");

        return;
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== DRIVER REST MANAGEMENT ==========\n");

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
            << ConsoleTheme::FormatLiteral("Driver #")
            << driver.GetDriverID()
            << ConsoleTheme::FormatLiteral(" | ")
            << driver.GetName()
            << ConsoleTheme::FormatLiteral(" | ")
            << ConsoleTheme::Status(driver.GetStatus());

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << ConsoleTheme::FormatLiteral(" | HOS: ")
            << projectedHours
            << ConsoleTheme::FormatLiteral(" / ")
            << HOSManager::
            GetMaxDrivingHours();

        std::cout
            << ConsoleTheme::FormatLiteral(" | Rest: ")
            << projectedRest
            << ConsoleTheme::FormatLiteral(" / ")
            << HOSManager::
            GetRequiredRestHours()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    std::cout
        << ConsoleTheme::FormatLiteral("============================================\n");

    int driverID =
        InputManager::ReadInt(
            "Enter Driver ID to rest (0 to cancel): "
        );

    if (driverID == 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver rest cancelled.\n");

        return;
    }

    Driver* selectedDriver =
        company.FindDriver(
            driverID
        );

    if (selectedDriver == nullptr)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver ID ")
            << driverID
            << ConsoleTheme::FormatLiteral(" was not found.\n");

        return;
    }

    if (selectedDriver->GetStatus() !=
        "Available")
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver #")
            << driverID
            << ConsoleTheme::FormatLiteral(" cannot be manually rested ")
            << ConsoleTheme::FormatLiteral("while ")
            << ConsoleTheme::Status(selectedDriver->GetStatus())
            << ConsoleTheme::FormatLiteral(".\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Complete the driver's active load first.\n");

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
            << ConsoleTheme::FormatLiteral("\nDriver #")
            << driverID
            << ConsoleTheme::FormatLiteral(" already has a full ")
            << std::fixed
            << std::setprecision(2)
            << HOSManager::
            GetMaxDrivingHours()
            << ConsoleTheme::FormatLiteral(" driving hours available.\n")
            << std::defaultfloat
            << std::setprecision(6);

        return;
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== REST OPTIONS ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Driver: #")
        << selectedDriver->GetDriverID()
        << ConsoleTheme::FormatLiteral(" - ")
        << selectedDriver->GetName()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Driving Hours Remaining: ")
        << currentHours
        << ConsoleTheme::FormatLiteral(" / ")
        << HOSManager::
        GetMaxDrivingHours()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Current Rest Progress: ")
        << currentRest
        << ConsoleTheme::FormatLiteral(" / ")
        << HOSManager::
        GetRequiredRestHours()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    if (restStartTime >
        HOSManager::GetCurrentEpoch())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Rest Can Begin: ")
            << HOSManager::
            FormatDateTime(
                restStartTime
            )
            << ConsoleTheme::FormatLiteral("\n");
    }
    else
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Rest Can Begin: Now\n");
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n1. Rest Until Full HOS Reset\n");

    std::cout
        << ConsoleTheme::FormatLiteral("2. Advance Off-Duty Time Manually\n");

    std::cout
        << ConsoleTheme::FormatLiteral("0. Cancel\n");

    std::cout
        << ConsoleTheme::FormatLiteral("==================================\n");

    int restChoice =
        InputManager::ReadMenuChoice(
            "Select an option: ",
            0,
            2
        );

    if (restChoice == 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver rest cancelled.\n");

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
                << ConsoleTheme::FormatLiteral("Only ")
                << std::fixed
                << std::setprecision(2)
                << hoursNeeded
                << ConsoleTheme::FormatLiteral(" more rest hours are needed ")
                << ConsoleTheme::FormatLiteral("for a full HOS reset.\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Rest time will be limited to ")
                << hoursNeeded
                << ConsoleTheme::FormatLiteral(" hours.\n")
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
        << ConsoleTheme::FormatLiteral("\n========== REST CONFIRMATION ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Driver: #")
        << selectedDriver->GetDriverID()
        << ConsoleTheme::FormatLiteral(" - ")
        << selectedDriver->GetName()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << ConsoleTheme::FormatLiteral("Off-Duty Time: ")
        << restHours
        << ConsoleTheme::FormatLiteral(" hours\n");

    std::cout
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("Rest Complete: ")
        << HOSManager::
        FormatDateTime(
            restCompleteTime
        )
        << ConsoleTheme::FormatLiteral("\n");

    if (fullReset)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Result: Full HOS reset to ")
            << std::fixed
            << std::setprecision(2)
            << HOSManager::
            GetMaxDrivingHours()
            << ConsoleTheme::FormatLiteral(" driving hours.\n")
            << std::defaultfloat
            << std::setprecision(6);
    }
    else
    {
        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << ConsoleTheme::FormatLiteral("Resulting Rest Progress: ")
            << projectedRestAfter
            << ConsoleTheme::FormatLiteral(" / ")
            << HOSManager::
            GetRequiredRestHours()
            << ConsoleTheme::FormatLiteral(" hours\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Driving Hours Remaining: ")
            << currentHours
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    std::cout
        << ConsoleTheme::FormatLiteral("=======================================\n");

    if (!InputManager::ReadYesNo(
        "Schedule this driver rest? (Y/N): "))
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver rest cancelled. ")
            << ConsoleTheme::FormatLiteral("No HOS changes were made.\n");

        return;
    }

    ApplyDriverRest(
        *selectedDriver,
        restHours
    );

    std::cout
        << ConsoleTheme::FormatLiteral("\nDriver #")
        << selectedDriver->GetDriverID()
        << ConsoleTheme::FormatLiteral(" scheduled for ")
        << std::fixed
        << std::setprecision(2)
        << restHours
        << ConsoleTheme::FormatLiteral(" off-duty hours.\n")
        << std::defaultfloat
        << std::setprecision(6);

    std::cout
        << ConsoleTheme::FormatLiteral("Scheduled Available: ")
        << HOSManager::
        FormatDateTime(
            selectedDriver->
            GetAvailableAtEpoch()
        )
        << ConsoleTheme::FormatLiteral("\n");

    if (fullReset)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver will return with a full ")
            << std::fixed
            << std::setprecision(2)
            << HOSManager::
            GetMaxDrivingHours()
            << ConsoleTheme::FormatLiteral(" driving hours available.\n")
            << std::defaultfloat
            << std::setprecision(6);
    }
    else
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver will retain ")
            << std::fixed
            << std::setprecision(2)
            << currentHours
            << ConsoleTheme::FormatLiteral(" driving hours with ")
            << projectedRestAfter
            << ConsoleTheme::FormatLiteral(" / ")
            << HOSManager::
            GetRequiredRestHours()
            << ConsoleTheme::FormatLiteral(" rest hours accumulated.\n")
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
            << ConsoleTheme::FormatLiteral("\nNo drivers have been added yet.\n");

        return;
    }

    int driversNeedingRest = 0;

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== REST ALL AVAILABLE DRIVERS ==========\n");

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
            << ConsoleTheme::FormatLiteral("Driver #")
            << driver.GetDriverID()
            << ConsoleTheme::FormatLiteral(" - ")
            << driver.GetName();

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << ConsoleTheme::FormatLiteral(" | Rest Needed: ")
            << hoursNeeded
            << ConsoleTheme::FormatLiteral(" hours\n");

        std::cout
            << std::defaultfloat
            << std::setprecision(6);
    }

    if (driversNeedingRest == 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No available drivers currently ")
            << ConsoleTheme::FormatLiteral("need an HOS reset.\n");

        std::cout
            << ConsoleTheme::FormatLiteral("================================================\n");

        return;
    }

    std::cout
        << ConsoleTheme::FormatLiteral("================================================\n");

    std::cout
        << driversNeedingRest
        << ConsoleTheme::FormatLiteral(" available driver");

    if (driversNeedingRest != 1)
    {
        std::cout << ConsoleTheme::FormatLiteral("s");
    }

    std::cout
        << ConsoleTheme::FormatLiteral(" will be rested until full HOS reset.\n");

    if (!InputManager::ReadYesNo(
        "Rest all listed drivers? (Y/N): "))
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Rest-all cancelled. ")
            << ConsoleTheme::FormatLiteral("No HOS changes were made.\n");

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
            << ConsoleTheme::FormatLiteral("Driver #")
            << driver.GetDriverID()
            << ConsoleTheme::FormatLiteral(" scheduled available at ")
            << HOSManager::
            FormatDateTime(
                driver.GetAvailableAtEpoch()
            )
            << ConsoleTheme::FormatLiteral(" with full HOS.\n");
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n")
        << restedDrivers
        << ConsoleTheme::FormatLiteral(" driver");

    if (restedDrivers != 1)
    {
        std::cout << ConsoleTheme::FormatLiteral("s");
    }

    std::cout
        << ConsoleTheme::FormatLiteral(" scheduled for full HOS reset.\n");
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