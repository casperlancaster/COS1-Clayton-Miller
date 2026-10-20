#include "ConsoleTheme.h"
#include "DispatchCenter.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "Driver.h"
#include "HOSManager.h"
#include "InputManager.h"
#include "Load.h"
#include "PayrollManager.h"
#include "Truck.h"

namespace
{
    constexpr double FUEL_PRICE_PER_GALLON =
        4.00;

    long long GetResourceStartTime(
        const Driver& driver,
        const Truck& truck)
    {
        long long startTime =
            HOSManager::GetCurrentEpoch();

        startTime =
            std::max(
                startTime,
                driver.
                GetAvailableAtEpoch()
            );

        startTime =
            std::max(
                startTime,
                truck.
                GetAvailableAtEpoch()
            );

        return startTime;
    }

    long long GetTeamStartTime(
        const Driver& primaryDriver,
        const Driver& secondDriver,
        const Truck& truck)
    {
        long long startTime =
            GetResourceStartTime(
                primaryDriver,
                truck
            );

        startTime =
            std::max(
                startTime,
                secondDriver.
                GetAvailableAtEpoch()
            );

        return startTime;
    }

    bool HasSecondAvailableDriver(
        const Company& company,
        int primaryDriverID)
    {
        for (const Driver& driver :
            company.GetDrivers())
        {
            if (driver.GetDriverID() !=
                primaryDriverID &&
                driver.GetStatus() ==
                "Available")
            {
                return true;
            }
        }

        return false;
    }

    void DisplayHOSPlan(
        const HOSPlan& plan,
        const Driver& primaryDriver,
        const Driver* secondDriver,
        const Load& load,
        long long dispatchStart)
    {
        double primaryHours =
            primaryDriver.
            GetProjectedDrivingHoursAt(
                dispatchStart
            );

        double primaryRest =
            primaryDriver.
            GetProjectedRestHoursAt(
                dispatchStart
            );

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << ConsoleTheme::FormatLiteral("\n========== HOS TRIP PLAN ==========\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Operation: ")
            << (plan.teamOperation
                ? "TEAM"
                : "SOLO")
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Scheduled Start: ")
            << HOSManager::
            FormatDateTime(
                dispatchStart
            )
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Primary Driver: #")
            << primaryDriver.
            GetDriverID()
            << ConsoleTheme::FormatLiteral(" - ")
            << primaryDriver.GetName()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Primary HOS at Start: ")
            << primaryHours
            << ConsoleTheme::FormatLiteral(" hours\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Primary Rest Progress at Start: ")
            << primaryRest
            << ConsoleTheme::FormatLiteral(" / ")
            << HOSManager::
            GetRequiredRestHours()
            << ConsoleTheme::FormatLiteral(" hours\n");

        if (secondDriver != nullptr)
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Team Driver: #")
                << secondDriver->
                GetDriverID()
                << ConsoleTheme::FormatLiteral(" - ")
                << secondDriver->
                GetName()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Team HOS at Start: ")
                << secondDriver->
                GetProjectedDrivingHoursAt(
                    dispatchStart
                )
                << ConsoleTheme::FormatLiteral(" hours\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Team Rest Progress at Start: ")
                << secondDriver->
                GetProjectedRestHoursAt(
                    dispatchStart
                )
                << ConsoleTheme::FormatLiteral(" / ")
                << HOSManager::
                GetRequiredRestHours()
                << ConsoleTheme::FormatLiteral(" hours\n");
        }

        std::cout
            << ConsoleTheme::FormatLiteral("Average Planning Speed: ")
            << HOSManager::
            GetAveragePlanningSpeedMph()
            << ConsoleTheme::FormatLiteral(" MPH\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Driving Time Required: ")
            << plan.
            drivingHoursRequired
            << ConsoleTheme::FormatLiteral(" hours\n");

        if (!plan.teamOperation)
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Mandatory Rest Periods: ")
                << plan.soloRestPeriods
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Stopped Rest Time Required: ")
                << plan.stoppedRestHours
                << ConsoleTheme::FormatLiteral(" hours\n");
        }
        else
        {
            std::cout
                << ConsoleTheme::FormatLiteral("Stopped Rest Time Required: ")
                << plan.stoppedRestHours
                << ConsoleTheme::FormatLiteral(" hours\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Drivers alternate while ")
                << ConsoleTheme::FormatLiteral("the off-duty driver rests.\n");
        }

        std::cout
            << ConsoleTheme::FormatLiteral("Total Estimated Trip Time: ")
            << plan.totalTripHours
            << ConsoleTheme::FormatLiteral(" hours\n");

        std::cout
            << std::defaultfloat
            << std::setprecision(6);

        std::cout
            << ConsoleTheme::FormatLiteral("Estimated Delivery: ")
            << HOSManager::
            FormatDateTime(
                plan.
                estimatedDeliveryEpoch
            )
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Delivery Deadline: ")
            << HOSManager::
            FormatDateTime(
                load.
                GetDeliveryDeadlineEpoch()
            )
            << ConsoleTheme::FormatLiteral("\n");

        if (load.
            GetDeliveryDeadlineEpoch() <= 0)
        {
            std::cout
                << ConsoleTheme::FormatLiteral("STATUS: LEGACY LOAD - ")
                << ConsoleTheme::FormatLiteral("NO DEADLINE\n");
        }
        else if (plan.meetsDeadline)
        {
            std::cout
                << ConsoleTheme::FormatLiteral("STATUS: ON TIME\n");
        }
        else
        {
            long long lateSeconds =
                plan.
                estimatedDeliveryEpoch -
                load.
                GetDeliveryDeadlineEpoch();

            double lateHours =
                lateSeconds /
                3600.0;

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << ConsoleTheme::FormatLiteral("STATUS: DEADLINE ")
                << ConsoleTheme::FormatLiteral("CANNOT BE MET\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Estimated Late By: ")
                << lateHours
                << ConsoleTheme::FormatLiteral(" hours\n");

            std::cout
                << std::defaultfloat
                << std::setprecision(6);
        }

        std::cout
            << ConsoleTheme::FormatLiteral("===================================\n");
    }
}

void DispatchCenter::ShowMenu()
{
    std::cout << ConsoleTheme::FormatLiteral("\n");
    std::cout << ConsoleTheme::FormatLiteral("========== DISPATCH CENTER ==========\n");
    std::cout << ConsoleTheme::FormatLiteral("1. Assign Load\n");
    std::cout << ConsoleTheme::FormatLiteral("2. View Available Resources\n");
    std::cout << ConsoleTheme::FormatLiteral("3. Complete Load\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Return to Main Menu\n");
    std::cout << ConsoleTheme::FormatLiteral("=====================================\n");
}

const Load* DispatchCenter::FindLoad(
    const Company& company,
    int loadNumber)
{
    for (const Load& load :
        company.GetLoads())
    {
        if (load.GetLoadNumber() ==
            loadNumber)
        {
            return &load;
        }
    }

    return nullptr;
}

const Driver* DispatchCenter::FindDriver(
    const Company& company,
    int driverID)
{
    for (const Driver& driver :
        company.GetDrivers())
    {
        if (driver.GetDriverID() ==
            driverID)
        {
            return &driver;
        }
    }

    return nullptr;
}

const Truck* DispatchCenter::FindTruck(
    const Company& company,
    int truckUnitNumber)
{
    for (const Truck& truck :
        company.GetTrucks())
    {
        if (truck.GetUnitNumber() ==
            truckUnitNumber)
        {
            return &truck;
        }
    }

    return nullptr;
}

bool DispatchCenter::HasAvailableLoad(
    const Company& company)
{
    for (const Load& load :
        company.GetLoads())
    {
        if (load.GetStatus() ==
            "Available")
        {
            return true;
        }
    }

    return false;
}

bool DispatchCenter::HasAvailableDriver(
    const Company& company)
{
    for (const Driver& driver :
        company.GetDrivers())
    {
        if (driver.GetStatus() ==
            "Available")
        {
            return true;
        }
    }

    return false;
}

bool DispatchCenter::HasAvailableTruck(
    const Company& company)
{
    for (const Truck& truck :
        company.GetTrucks())
    {
        if (truck.GetStatus() ==
            "Available")
        {
            return true;
        }
    }

    return false;
}

void DispatchCenter::DisplayAvailableResources(
    const Company& company)
{
    long long now =
        HOSManager::GetCurrentEpoch();

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== AVAILABLE LOADS ==========\n");

    bool foundLoad = false;

    for (const Load& load :
        company.GetLoads())
    {
        if (load.GetStatus() ==
            "Available")
        {
            foundLoad = true;

            std::cout
                << ConsoleTheme::FormatLiteral("Load #")
                << load.GetLoadNumber()
                << ConsoleTheme::FormatLiteral(" | ")
                << load.GetOrigin()
                << ConsoleTheme::FormatLiteral(" -> ")
                << load.GetDestination()
                << ConsoleTheme::FormatLiteral(" | ")
                << load.GetDistanceMiles()
                << ConsoleTheme::FormatLiteral(" miles")
                << ConsoleTheme::FormatLiteral(" | Pay: $")
                << std::fixed
                << std::setprecision(2)
                << load.GetPay()
                << std::defaultfloat
                << std::setprecision(6)
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Deadline: ")
                << HOSManager::
                FormatDateTime(
                    load.
                    GetDeliveryDeadlineEpoch()
                )
                << ConsoleTheme::FormatLiteral("\n");
        }
    }

    if (!foundLoad)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No available loads.\n");
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== SCHEDULABLE DRIVERS ==========\n");

    bool foundDriver = false;

    for (const Driver& driver :
        company.GetDrivers())
    {
        if (driver.GetStatus() ==
            "Available")
        {
            foundDriver = true;

            long long planningTime =
                std::max(
                    now,
                    driver.
                    GetAvailableAtEpoch()
                );

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << ConsoleTheme::FormatLiteral("Driver ID ")
                << driver.GetDriverID()
                << ConsoleTheme::FormatLiteral(" | ")
                << driver.GetName()
                << ConsoleTheme::FormatLiteral(" | HOS at availability: ")
                << driver.
                GetProjectedDrivingHoursAt(
                    planningTime
                )
                << ConsoleTheme::FormatLiteral(" hours")
                << ConsoleTheme::FormatLiteral(" | Rest: ")
                << driver.
                GetProjectedRestHoursAt(
                    planningTime
                )
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

    if (!foundDriver)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No schedulable drivers.\n");
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== SCHEDULABLE TRUCKS ==========\n");

    bool foundTruck = false;

    for (const Truck& truck :
        company.GetTrucks())
    {
        if (truck.GetStatus() ==
            "Available")
        {
            foundTruck = true;

            std::cout
                << std::fixed
                << std::setprecision(1);

            std::cout
                << ConsoleTheme::FormatLiteral("Truck #")
                << truck.GetUnitNumber()
                << ConsoleTheme::FormatLiteral(" | Mileage: ")
                << truck.GetMileage()
                << ConsoleTheme::FormatLiteral(" | Fuel: ")
                << truck.GetFuelGallons()
                << ConsoleTheme::FormatLiteral(" / ")
                << truck.GetFuelTankCapacity()
                << ConsoleTheme::FormatLiteral(" gal")
                << ConsoleTheme::FormatLiteral(" | Range: ")
                << truck.GetFuelRangeMiles()
                << ConsoleTheme::FormatLiteral(" miles\n");

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            if (truck.GetAvailableAtEpoch() >
                now)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Scheduled Available: ")
                    << HOSManager::
                    FormatDateTime(
                        truck.
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

    if (!foundTruck)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No schedulable trucks.\n");
    }

    std::cout
        << ConsoleTheme::FormatLiteral("========================================\n");
}

void DispatchCenter::DisplayAssignedLoads(
    const Company& company)
{
    std::cout
        << ConsoleTheme::FormatLiteral("\n========== ASSIGNED LOADS ==========\n");

    bool foundAssignedLoad = false;

    for (const Load& load :
        company.GetLoads())
    {
        if (load.GetStatus() ==
            "Assigned")
        {
            foundAssignedLoad = true;

            std::cout
                << ConsoleTheme::FormatLiteral("\nLoad #")
                << load.GetLoadNumber()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << load.GetOrigin()
                << ConsoleTheme::FormatLiteral(" -> ")
                << load.GetDestination()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Distance: ")
                << load.GetDistanceMiles()
                << ConsoleTheme::FormatLiteral(" miles\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Primary Driver ID: ")
                << load.GetAssignedDriverID()
                << ConsoleTheme::FormatLiteral("\n");

            if (load.IsTeamLoad())
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Team Driver ID: ")
                    << load.
                    GetAssignedSecondDriverID()
                    << ConsoleTheme::FormatLiteral("\n");
            }

            std::cout
                << ConsoleTheme::FormatLiteral("Truck #")
                << load.
                GetAssignedTruckUnitNumber()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Operation: ")
                << (load.IsTeamLoad()
                    ? "TEAM"
                    : "SOLO")
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Scheduled Start: ")
                << HOSManager::
                FormatDateTime(
                    load.
                    GetDispatchStartEpoch()
                )
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Estimated Delivery: ")
                << HOSManager::
                FormatDateTime(
                    load.
                    GetEstimatedDeliveryEpoch()
                )
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Deadline: ")
                << HOSManager::
                FormatDateTime(
                    load.
                    GetDeliveryDeadlineEpoch()
                )
                << ConsoleTheme::FormatLiteral("\n");
        }
    }

    if (!foundAssignedLoad)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("No loads are currently assigned.\n");
    }

    std::cout
        << ConsoleTheme::FormatLiteral("====================================\n");
}

void DispatchCenter::Run(
    DispatchManager& dispatchManager)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                3
            );

        switch (choice)
        {
        case 1:
        {
            if (!dispatchManager.
                HasCompany())
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nDispatch Center is not ")
                    << ConsoleTheme::FormatLiteral("connected to a company.\n");

                break;
            }

            Company* company =
                dispatchManager.
                GetCompany();

            DisplayAvailableResources(
                *company
            );

            if (!HasAvailableLoad(
                *company) ||
                !HasAvailableDriver(
                    *company) ||
                !HasAvailableTruck(
                    *company))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nA load cannot be scheduled ")
                    << ConsoleTheme::FormatLiteral("until a load, driver, and truck ")
                    << ConsoleTheme::FormatLiteral("are available for scheduling.\n");

                break;
            }

            std::cout
                << ConsoleTheme::FormatLiteral("\n========== ASSIGN LOAD ==========\n");

            int loadNumber =
                InputManager::ReadPositiveInt(
                    "Enter a load number: ",
                    "load number"
                );

            const Load* selectedLoad =
                FindLoad(
                    *company,
                    loadNumber
                );

            if (selectedLoad == nullptr)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Load #")
                    << loadNumber
                    << ConsoleTheme::FormatLiteral(" does not exist.\n");

                break;
            }

            if (selectedLoad->GetStatus() !=
                "Available")
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Load #")
                    << loadNumber
                    << ConsoleTheme::FormatLiteral(" is currently ")
                    << ConsoleTheme::Status(selectedLoad->GetStatus())
                    << ConsoleTheme::FormatLiteral(" and cannot be assigned.\n");

                break;
            }

            int driverID =
                InputManager::ReadPositiveInt(
                    "Enter Primary Driver ID: ",
                    "Driver ID"
                );

            const Driver* primaryDriver =
                FindDriver(
                    *company,
                    driverID
                );

            if (primaryDriver == nullptr)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Driver ID ")
                    << driverID
                    << ConsoleTheme::FormatLiteral(" does not exist.\n");

                break;
            }

            if (primaryDriver->GetStatus() !=
                "Available")
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Driver ID ")
                    << driverID
                    << ConsoleTheme::FormatLiteral(" is currently ")
                    << ConsoleTheme::Status(primaryDriver->GetStatus())
                    << ConsoleTheme::FormatLiteral(" and cannot be scheduled.\n");

                break;
            }

            int truckUnitNumber =
                InputManager::ReadPositiveInt(
                    "Enter Truck Unit Number: ",
                    "Truck Unit Number"
                );

            const Truck* selectedTruck =
                FindTruck(
                    *company,
                    truckUnitNumber
                );

            if (selectedTruck == nullptr)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Truck #")
                    << truckUnitNumber
                    << ConsoleTheme::FormatLiteral(" does not exist.\n");

                break;
            }

            if (selectedTruck->GetStatus() !=
                "Available")
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Truck #")
                    << truckUnitNumber
                    << ConsoleTheme::FormatLiteral(" is currently ")
                    << ConsoleTheme::Status(selectedTruck->GetStatus())
                    << ConsoleTheme::FormatLiteral(" and cannot be scheduled.\n");

                break;
            }

            long long dispatchStart =
                GetResourceStartTime(
                    *primaryDriver,
                    *selectedTruck
                );

            double primaryHours =
                primaryDriver->
                GetProjectedDrivingHoursAt(
                    dispatchStart
                );

            double primaryRest =
                primaryDriver->
                GetProjectedRestHoursAt(
                    dispatchStart
                );

            HOSPlan selectedPlan =
                HOSManager::CreateSoloPlan(
                    primaryHours,
                    primaryRest,
                    selectedLoad->
                    GetDistanceMiles(),
                    dispatchStart,
                    selectedLoad->
                    GetDeliveryDeadlineEpoch()
                );

            DisplayHOSPlan(
                selectedPlan,
                *primaryDriver,
                nullptr,
                *selectedLoad,
                dispatchStart
            );

            int secondDriverID = 0;

            const Driver* secondDriver =
                nullptr;

            if (!selectedPlan.
                meetsDeadline)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nThe primary driver cannot ")
                    << ConsoleTheme::FormatLiteral("legally deliver this load ")
                    << ConsoleTheme::FormatLiteral("before the deadline.\n");

                if (!HasSecondAvailableDriver(
                    *company,
                    driverID))
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("No second available driver ")
                        << ConsoleTheme::FormatLiteral("exists for team operation.\n");

                    std::cout
                        << ConsoleTheme::FormatLiteral("Load cannot be dispatched.\n");

                    break;
                }

                bool tryTeam =
                    InputManager::ReadYesNo(
                        "Try a team operation? (Y/N): "
                    );

                if (!tryTeam)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Dispatch cancelled. ")
                        << ConsoleTheme::FormatLiteral("No changes were made.\n");

                    break;
                }

                std::cout
                    << ConsoleTheme::FormatLiteral("\n========== AVAILABLE TEAM DRIVERS ==========\n");

                for (const Driver& driver :
                    company->GetDrivers())
                {
                    if (driver.GetDriverID() !=
                        driverID &&
                        driver.GetStatus() ==
                        "Available")
                    {
                        long long candidateTime =
                            std::max(
                                dispatchStart,
                                driver.
                                GetAvailableAtEpoch()
                            );

                        std::cout
                            << std::fixed
                            << std::setprecision(2);

                        std::cout
                            << ConsoleTheme::FormatLiteral("Driver ID ")
                            << driver.GetDriverID()
                            << ConsoleTheme::FormatLiteral(" | ")
                            << driver.GetName()
                            << ConsoleTheme::FormatLiteral(" | HOS: ")
                            << driver.
                            GetProjectedDrivingHoursAt(
                                candidateTime
                            )
                            << ConsoleTheme::FormatLiteral(" | Rest: ")
                            << driver.
                            GetProjectedRestHoursAt(
                                candidateTime
                            )
                            << ConsoleTheme::FormatLiteral("\n");

                        std::cout
                            << std::defaultfloat
                            << std::setprecision(6);
                    }
                }

                secondDriverID =
                    InputManager::ReadPositiveInt(
                        "Enter Team Driver ID: ",
                        "Team Driver ID"
                    );

                secondDriver =
                    FindDriver(
                        *company,
                        secondDriverID
                    );

                if (secondDriver == nullptr)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Driver ID ")
                        << secondDriverID
                        << ConsoleTheme::FormatLiteral(" does not exist.\n");

                    break;
                }

                if (secondDriverID ==
                    driverID)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("The team driver must ")
                        << ConsoleTheme::FormatLiteral("be a different driver.\n");

                    break;
                }

                if (secondDriver->GetStatus() !=
                    "Available")
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Driver ID ")
                        << secondDriverID
                        << ConsoleTheme::FormatLiteral(" is not available.\n");

                    break;
                }

                dispatchStart =
                    GetTeamStartTime(
                        *primaryDriver,
                        *secondDriver,
                        *selectedTruck
                    );

                primaryHours =
                    primaryDriver->
                    GetProjectedDrivingHoursAt(
                        dispatchStart
                    );

                primaryRest =
                    primaryDriver->
                    GetProjectedRestHoursAt(
                        dispatchStart
                    );

                double secondHours =
                    secondDriver->
                    GetProjectedDrivingHoursAt(
                        dispatchStart
                    );

                double secondRest =
                    secondDriver->
                    GetProjectedRestHoursAt(
                        dispatchStart
                    );

                selectedPlan =
                    HOSManager::
                    CreateTeamPlan(
                        primaryHours,
                        primaryRest,
                        secondHours,
                        secondRest,
                        selectedLoad->
                        GetDistanceMiles(),
                        dispatchStart,
                        selectedLoad->
                        GetDeliveryDeadlineEpoch()
                    );

                DisplayHOSPlan(
                    selectedPlan,
                    *primaryDriver,
                    secondDriver,
                    *selectedLoad,
                    dispatchStart
                );

                if (!selectedPlan.
                    meetsDeadline)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("\nEven a two-driver team ")
                        << ConsoleTheme::FormatLiteral("cannot legally meet ")
                        << ConsoleTheme::FormatLiteral("this delivery deadline.\n");

                    std::cout
                        << ConsoleTheme::FormatLiteral("Load cannot be dispatched.\n");

                    break;
                }
            }

            double tripFuelNeeded =
                selectedTruck->
                GetFuelNeededForDistance(
                    selectedLoad->
                    GetDistanceMiles()
                );

            double minimumArrivalFuel =
                selectedTruck->
                GetMinimumArrivalFuelGallons();

            double plannedFuelPurchase =
                selectedTruck->
                GetFuelPurchaseNeededForTrip(
                    selectedLoad->
                    GetDistanceMiles()
                );

            double plannedFuelCost =
                plannedFuelPurchase *
                FUEL_PRICE_PER_GALLON;

            bool teamOperation =
                secondDriverID > 0;

            double estimatedPayroll =
                PayrollManager::
                CalculateTotalPayroll(
                    selectedLoad->
                    GetDistanceMiles(),
                    teamOperation
                );

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << ConsoleTheme::FormatLiteral("\n========== TRIP FUEL PLAN ==========\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Current Fuel: ")
                << selectedTruck->
                GetFuelGallons()
                << ConsoleTheme::FormatLiteral(" gallons\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Trip Fuel Required: ")
                << tripFuelNeeded
                << ConsoleTheme::FormatLiteral(" gallons\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Required Delivery Reserve: ")
                << minimumArrivalFuel
                << ConsoleTheme::FormatLiteral(" gallons (50% tank)\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Estimated In-Trip Purchase: ")
                << plannedFuelPurchase
                << ConsoleTheme::FormatLiteral(" gallons\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Estimated Fuel Cost: $")
                << plannedFuelCost
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("====================================\n");

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            std::cout
                << ConsoleTheme::FormatLiteral("\n========== DISPATCH CONFIRMATION ==========\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Load: #")
                << loadNumber
                << ConsoleTheme::FormatLiteral(" | ")
                << selectedLoad->GetOrigin()
                << ConsoleTheme::FormatLiteral(" -> ")
                << selectedLoad->GetDestination()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Primary Driver: #")
                << driverID
                << ConsoleTheme::FormatLiteral(" - ")
                << primaryDriver->GetName()
                << ConsoleTheme::FormatLiteral("\n");

            if (secondDriver != nullptr)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Team Driver: #")
                    << secondDriverID
                    << ConsoleTheme::FormatLiteral(" - ")
                    << secondDriver->GetName()
                    << ConsoleTheme::FormatLiteral("\n");
            }

            std::cout
                << ConsoleTheme::FormatLiteral("Truck: #")
                << truckUnitNumber
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Scheduled Start: ")
                << HOSManager::FormatDateTime(
                    dispatchStart
                )
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Estimated Delivery: ")
                << HOSManager::FormatDateTime(
                    selectedPlan.estimatedDeliveryEpoch
                )
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << ConsoleTheme::FormatLiteral("Load Revenue: $")
                << selectedLoad->GetPay()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Estimated In-Trip Fuel Cost: $")
                << plannedFuelCost
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Driver Payroll on Completion: $")
                << estimatedPayroll
                << ConsoleTheme::FormatLiteral("\n");

            if (teamOperation)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Team Pay Rate: $")
                    << PayrollManager::
                    GetTeamRatePerMile()
                    << ConsoleTheme::FormatLiteral(" per mile per driver\n");
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Solo Pay Rate: $")
                    << PayrollManager::
                    GetSoloRatePerMile()
                    << ConsoleTheme::FormatLiteral(" per mile\n");
            }

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            std::cout
                << ConsoleTheme::FormatLiteral("==========================================\n");

            if (!InputManager::ReadYesNo(
                "Confirm dispatch? (Y/N): "))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Dispatch cancelled. ")
                    << ConsoleTheme::FormatLiteral("No changes were made.\n");

                break;
            }

            bool assigned =
                dispatchManager.AssignLoad(
                    loadNumber,
                    driverID,
                    truckUnitNumber,
                    secondDriverID,
                    dispatchStart
                );

            if (assigned)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nLoad #")
                    << loadNumber
                    << ConsoleTheme::FormatLiteral(" assigned successfully.\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Primary Driver: #")
                    << driverID
                    << ConsoleTheme::FormatLiteral("\n");

                if (secondDriverID > 0)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Team Driver: #")
                        << secondDriverID
                        << ConsoleTheme::FormatLiteral("\n");

                    std::cout
                        << ConsoleTheme::FormatLiteral("Operation: TEAM\n");
                }
                else
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Operation: SOLO\n");
                }

                std::cout
                    << ConsoleTheme::FormatLiteral("Truck: #")
                    << truckUnitNumber
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Scheduled Start: ")
                    << HOSManager::
                    FormatDateTime(
                        dispatchStart
                    )
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Estimated Delivery: ")
                    << HOSManager::
                    FormatDateTime(
                        selectedPlan.
                        estimatedDeliveryEpoch
                    )
                    << ConsoleTheme::FormatLiteral("\n");
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nUnable to assign load. ")
                    << ConsoleTheme::FormatLiteral("The HOS, deadline, or ")
                    << ConsoleTheme::FormatLiteral("resource state is no longer valid.\n");
            }

            break;
        }

        case 2:
        {
            if (!dispatchManager.
                HasCompany())
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nDispatch Center is not ")
                    << ConsoleTheme::FormatLiteral("connected to a company.\n");

                break;
            }

            DisplayAvailableResources(
                *dispatchManager.
                GetCompany()
            );

            break;
        }

        case 3:
        {
            if (!dispatchManager.
                HasCompany())
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nDispatch Center is not ")
                    << ConsoleTheme::FormatLiteral("connected to a company.\n");

                break;
            }

            Company* company =
                dispatchManager.
                GetCompany();

            DisplayAssignedLoads(
                *company
            );

            bool hasAssignedLoad =
                false;

            for (const Load& load :
                company->GetLoads())
            {
                if (load.GetStatus() ==
                    "Assigned")
                {
                    hasAssignedLoad =
                        true;

                    break;
                }
            }

            if (!hasAssignedLoad)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nThere are no assigned ")
                    << ConsoleTheme::FormatLiteral("loads to complete.\n");

                break;
            }

            int loadNumber =
                InputManager::ReadPositiveInt(
                    "\nEnter the load number "
                    "to complete: ",
                    "load number"
                );

            const Load* selectedLoad =
                FindLoad(
                    *company,
                    loadNumber
                );

            if (selectedLoad == nullptr ||
                selectedLoad->GetStatus() !=
                "Assigned")
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("That load is not currently assigned.\n");

                break;
            }

            int primaryDriverID =
                selectedLoad->
                GetAssignedDriverID();

            int secondDriverID =
                selectedLoad->
                GetAssignedSecondDriverID();

            int truckUnitNumber =
                selectedLoad->
                GetAssignedTruckUnitNumber();

            const Truck* tripTruck =
                FindTruck(
                    *company,
                    truckUnitNumber
                );

            double tripFuelUsed = 0.0;
            double tripFuelPurchased = 0.0;
            double tripFuelCost = 0.0;

            if (tripTruck != nullptr)
            {
                tripFuelUsed =
                    tripTruck->
                    GetFuelNeededForDistance(
                        selectedLoad->
                        GetDistanceMiles()
                    );

                tripFuelPurchased =
                    tripTruck->
                    GetFuelPurchaseNeededForTrip(
                        selectedLoad->
                        GetDistanceMiles()
                    );

                tripFuelCost =
                    tripFuelPurchased *
                    FUEL_PRICE_PER_GALLON;
            }

            bool teamOperation =
                secondDriverID > 0;

            double primaryDriverPay =
                PayrollManager::
                CalculateDriverPay(
                    selectedLoad->
                    GetDistanceMiles(),
                    teamOperation
                );

            double secondDriverPay =
                teamOperation
                ? primaryDriverPay
                : 0.0;

            double totalPayroll =
                primaryDriverPay +
                secondDriverPay;

            std::cout
                << ConsoleTheme::FormatLiteral("\n========== LOAD COMPLETION CONFIRMATION ==========\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Load: #")
                << loadNumber
                << ConsoleTheme::FormatLiteral(" | ")
                << selectedLoad->GetOrigin()
                << ConsoleTheme::FormatLiteral(" -> ")
                << selectedLoad->GetDestination()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Truck: #")
                << truckUnitNumber
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Primary Driver: #")
                << primaryDriverID
                << ConsoleTheme::FormatLiteral("\n");

            if (secondDriverID > 0)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Team Driver: #")
                    << secondDriverID
                    << ConsoleTheme::FormatLiteral("\n");
            }

            std::cout
                << ConsoleTheme::FormatLiteral("Simulated Delivery: ")
                << HOSManager::FormatDateTime(
                    selectedLoad->
                    GetEstimatedDeliveryEpoch()
                )
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << ConsoleTheme::FormatLiteral("Load Revenue: $")
                << selectedLoad->GetPay()
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << ConsoleTheme::FormatLiteral("Trip Fuel Used: ")
                << tripFuelUsed
                << ConsoleTheme::FormatLiteral(" gallons\n");

            std::cout
                << ConsoleTheme::FormatLiteral("In-Trip Fuel Purchase: ")
                << tripFuelPurchased
                << ConsoleTheme::FormatLiteral(" gallons\n");

            std::cout
                << ConsoleTheme::FormatLiteral("In-Trip Fuel Cost: $")
                << tripFuelCost
                << ConsoleTheme::FormatLiteral("\n");

            if (teamOperation)
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Team Pay Rate: $")
                    << PayrollManager::
                    GetTeamRatePerMile()
                    << ConsoleTheme::FormatLiteral(" per mile per driver\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Primary Driver Payroll: $")
                    << primaryDriverPay
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Team Driver Payroll: $")
                    << secondDriverPay
                    << ConsoleTheme::FormatLiteral("\n");
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Solo Pay Rate: $")
                    << PayrollManager::
                    GetSoloRatePerMile()
                    << ConsoleTheme::FormatLiteral(" per mile\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Primary Driver Payroll: $")
                    << primaryDriverPay
                    << ConsoleTheme::FormatLiteral("\n");
            }

            std::cout
                << ConsoleTheme::FormatLiteral("Total Driver Payroll Expense: $")
                << totalPayroll
                << ConsoleTheme::FormatLiteral("\n");

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            std::cout
                << ConsoleTheme::FormatLiteral("Completing this load will update HOS, fuel, ")
                << ConsoleTheme::FormatLiteral("mileage, revenue, driver payroll, truck location, ")
                << ConsoleTheme::FormatLiteral("and may trigger a post-trip breakdown.\n");

            std::cout
                << ConsoleTheme::FormatLiteral("==================================================\n");

            if (!InputManager::ReadYesNo(
                "Confirm load completion? (Y/N): "))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Load completion cancelled. ")
                    << ConsoleTheme::FormatLiteral("No changes were made.\n");

                break;
            }

            bool completed =
                dispatchManager.
                CompleteLoad(
                    loadNumber
                );

            if (completed)
            {
                const Driver* primaryDriver =
                    FindDriver(
                        *company,
                        primaryDriverID
                    );

                const Driver* secondDriver =
                    nullptr;

                if (secondDriverID > 0)
                {
                    secondDriver =
                        FindDriver(
                            *company,
                            secondDriverID
                        );
                }

                const Truck* completedTruck =
                    FindTruck(
                        *company,
                        truckUnitNumber
                    );

                const Load* completedLoad =
                    FindLoad(
                        *company,
                        loadNumber
                    );

                std::cout
                    << ConsoleTheme::FormatLiteral("\nLoad #")
                    << loadNumber
                    << ConsoleTheme::FormatLiteral(" completed successfully.\n");

                if (completedLoad != nullptr)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Simulated Delivery: ")
                        << HOSManager::
                        FormatDateTime(
                            completedLoad->
                            GetEstimatedDeliveryEpoch()
                        )
                        << ConsoleTheme::FormatLiteral("\n");
                }

                std::cout
                    << std::fixed
                    << std::setprecision(2);

                if (primaryDriver != nullptr)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Primary Driver HOS Remaining: ")
                        << primaryDriver->
                        GetDrivingHoursRemaining()
                        << ConsoleTheme::FormatLiteral(" hours.\n");

                    std::cout
                        << ConsoleTheme::FormatLiteral("Primary Driver Rest Progress: ")
                        << primaryDriver->
                        GetRestHoursAccumulated()
                        << ConsoleTheme::FormatLiteral(" / 10.00 hours.\n");

                    std::cout
                        << std::defaultfloat
                        << std::setprecision(6);

                    std::cout
                        << ConsoleTheme::FormatLiteral("Primary Driver Scheduled Available: ")
                        << HOSManager::
                        FormatDateTime(
                            primaryDriver->
                            GetAvailableAtEpoch()
                        )
                        << ConsoleTheme::FormatLiteral("\n");

                    std::cout
                        << std::fixed
                        << std::setprecision(2);
                }

                if (secondDriver != nullptr)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Team Driver HOS Remaining: ")
                        << secondDriver->
                        GetDrivingHoursRemaining()
                        << ConsoleTheme::FormatLiteral(" hours.\n");

                    std::cout
                        << ConsoleTheme::FormatLiteral("Team Driver Rest Progress: ")
                        << secondDriver->
                        GetRestHoursAccumulated()
                        << ConsoleTheme::FormatLiteral(" / 10.00 hours.\n");

                    std::cout
                        << std::defaultfloat
                        << std::setprecision(6);

                    std::cout
                        << ConsoleTheme::FormatLiteral("Team Driver Scheduled Available: ")
                        << HOSManager::
                        FormatDateTime(
                            secondDriver->
                            GetAvailableAtEpoch()
                        )
                        << ConsoleTheme::FormatLiteral("\n");

                    std::cout
                        << std::fixed
                        << std::setprecision(2);
                }

                std::cout
                    << ConsoleTheme::FormatLiteral("Trip Fuel Used: ")
                    << tripFuelUsed
                    << ConsoleTheme::FormatLiteral(" gallons.\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("In-Trip Fuel Purchased: ")
                    << tripFuelPurchased
                    << ConsoleTheme::FormatLiteral(" gallons.\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("In-Trip Fuel Expense: $")
                    << tripFuelCost
                    << ConsoleTheme::FormatLiteral(".\n");

                if (completedTruck != nullptr)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Fuel at Delivery: ")
                        << completedTruck->
                        GetFuelGallons()
                        << ConsoleTheme::FormatLiteral(" / ")
                        << completedTruck->
                        GetFuelTankCapacity()
                        << ConsoleTheme::FormatLiteral(" gallons (")
                        << completedTruck->
                        GetFuelPercentage()
                        << ConsoleTheme::FormatLiteral("%).\n");

                    std::cout
                        << ConsoleTheme::FormatLiteral("Truck Mileage: ")
                        << completedTruck->
                        GetMileage()
                        << ConsoleTheme::FormatLiteral(" miles.\n");

                    std::cout
                        << std::defaultfloat
                        << std::setprecision(6);

                    std::cout
                        << ConsoleTheme::FormatLiteral("Truck Scheduled Available: ")
                        << HOSManager::
                        FormatDateTime(
                            completedTruck->
                            GetAvailableAtEpoch()
                        )
                        << ConsoleTheme::FormatLiteral("\n");

                    std::cout
                        << std::fixed
                        << std::setprecision(2);
                }

                if (teamOperation)
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Primary Driver Payroll: $")
                        << primaryDriverPay
                        << ConsoleTheme::FormatLiteral(".\n");

                    std::cout
                        << ConsoleTheme::FormatLiteral("Team Driver Payroll: $")
                        << secondDriverPay
                        << ConsoleTheme::FormatLiteral(".\n");
                }
                else
                {
                    std::cout
                        << ConsoleTheme::FormatLiteral("Driver Payroll: $")
                        << primaryDriverPay
                        << ConsoleTheme::FormatLiteral(".\n");
                }

                std::cout
                    << ConsoleTheme::FormatLiteral("Total Driver Payroll Expense: $")
                    << totalPayroll
                    << ConsoleTheme::FormatLiteral(".\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Company Revenue: $")
                    << company->GetRevenue()
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Company Expenses: $")
                    << company->GetExpenses()
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << ConsoleTheme::FormatLiteral("Company Balance: $")
                    << company->GetBalance()
                    << ConsoleTheme::FormatLiteral("\n");

                std::cout
                    << std::defaultfloat
                    << std::setprecision(6);
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("\nUnable to complete load. ")
                    << ConsoleTheme::FormatLiteral("Check the dispatch state ")
                    << ConsoleTheme::FormatLiteral("and try again.\n");
            }

            break;
        }

        case 0:
            break;

        default:
            std::cout
                << ConsoleTheme::FormatLiteral("\nInvalid selection. ")
                << ConsoleTheme::FormatLiteral("Please choose 0, 1, 2, or 3.\n");

            break;
        }
    }
}