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
            << "\n========== HOS TRIP PLAN ==========\n";

        std::cout
            << "Operation: "
            << (plan.teamOperation
                ? "TEAM"
                : "SOLO")
            << "\n";

        std::cout
            << "Scheduled Start: "
            << HOSManager::
            FormatDateTime(
                dispatchStart
            )
            << "\n";

        std::cout
            << "Primary Driver: #"
            << primaryDriver.
            GetDriverID()
            << " - "
            << primaryDriver.GetName()
            << "\n";

        std::cout
            << "Primary HOS at Start: "
            << primaryHours
            << " hours\n";

        std::cout
            << "Primary Rest Progress at Start: "
            << primaryRest
            << " / "
            << HOSManager::
            GetRequiredRestHours()
            << " hours\n";

        if (secondDriver != nullptr)
        {
            std::cout
                << "Team Driver: #"
                << secondDriver->
                GetDriverID()
                << " - "
                << secondDriver->
                GetName()
                << "\n";

            std::cout
                << "Team HOS at Start: "
                << secondDriver->
                GetProjectedDrivingHoursAt(
                    dispatchStart
                )
                << " hours\n";

            std::cout
                << "Team Rest Progress at Start: "
                << secondDriver->
                GetProjectedRestHoursAt(
                    dispatchStart
                )
                << " / "
                << HOSManager::
                GetRequiredRestHours()
                << " hours\n";
        }

        std::cout
            << "Average Planning Speed: "
            << HOSManager::
            GetAveragePlanningSpeedMph()
            << " MPH\n";

        std::cout
            << "Driving Time Required: "
            << plan.
            drivingHoursRequired
            << " hours\n";

        if (!plan.teamOperation)
        {
            std::cout
                << "Mandatory Rest Periods: "
                << plan.soloRestPeriods
                << "\n";

            std::cout
                << "Stopped Rest Time Required: "
                << plan.stoppedRestHours
                << " hours\n";
        }
        else
        {
            std::cout
                << "Stopped Rest Time Required: "
                << plan.stoppedRestHours
                << " hours\n";

            std::cout
                << "Drivers alternate while "
                << "the off-duty driver rests.\n";
        }

        std::cout
            << "Total Estimated Trip Time: "
            << plan.totalTripHours
            << " hours\n";

        std::cout
            << std::defaultfloat
            << std::setprecision(6);

        std::cout
            << "Estimated Delivery: "
            << HOSManager::
            FormatDateTime(
                plan.
                estimatedDeliveryEpoch
            )
            << "\n";

        std::cout
            << "Delivery Deadline: "
            << HOSManager::
            FormatDateTime(
                load.
                GetDeliveryDeadlineEpoch()
            )
            << "\n";

        if (load.
            GetDeliveryDeadlineEpoch() <= 0)
        {
            std::cout
                << "STATUS: LEGACY LOAD - "
                << "NO DEADLINE\n";
        }
        else if (plan.meetsDeadline)
        {
            std::cout
                << "STATUS: ON TIME\n";
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
                << "STATUS: DEADLINE "
                << "CANNOT BE MET\n";

            std::cout
                << "Estimated Late By: "
                << lateHours
                << " hours\n";

            std::cout
                << std::defaultfloat
                << std::setprecision(6);
        }

        std::cout
            << "===================================\n";
    }
}

void DispatchCenter::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== DISPATCH CENTER ==========\n";
    std::cout << "1. Assign Load\n";
    std::cout << "2. View Available Resources\n";
    std::cout << "3. Complete Load\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=====================================\n";
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
        << "\n========== AVAILABLE LOADS ==========\n";

    bool foundLoad = false;

    for (const Load& load :
        company.GetLoads())
    {
        if (load.GetStatus() ==
            "Available")
        {
            foundLoad = true;

            std::cout
                << "Load #"
                << load.GetLoadNumber()
                << " | "
                << load.GetOrigin()
                << " -> "
                << load.GetDestination()
                << " | "
                << load.GetDistanceMiles()
                << " miles"
                << " | Pay: $"
                << std::fixed
                << std::setprecision(2)
                << load.GetPay()
                << std::defaultfloat
                << std::setprecision(6)
                << "\n";

            std::cout
                << "Deadline: "
                << HOSManager::
                FormatDateTime(
                    load.
                    GetDeliveryDeadlineEpoch()
                )
                << "\n";
        }
    }

    if (!foundLoad)
    {
        std::cout
            << "No available loads.\n";
    }

    std::cout
        << "\n========== SCHEDULABLE DRIVERS ==========\n";

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
                << "Driver ID "
                << driver.GetDriverID()
                << " | "
                << driver.GetName()
                << " | HOS at availability: "
                << driver.
                GetProjectedDrivingHoursAt(
                    planningTime
                )
                << " hours"
                << " | Rest: "
                << driver.
                GetProjectedRestHoursAt(
                    planningTime
                )
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

    if (!foundDriver)
    {
        std::cout
            << "No schedulable drivers.\n";
    }

    std::cout
        << "\n========== SCHEDULABLE TRUCKS ==========\n";

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
                << "Truck #"
                << truck.GetUnitNumber()
                << " | Mileage: "
                << truck.GetMileage()
                << " | Fuel: "
                << truck.GetFuelGallons()
                << " / "
                << truck.GetFuelTankCapacity()
                << " gal"
                << " | Range: "
                << truck.GetFuelRangeMiles()
                << " miles\n";

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            if (truck.GetAvailableAtEpoch() >
                now)
            {
                std::cout
                    << "Scheduled Available: "
                    << HOSManager::
                    FormatDateTime(
                        truck.
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

    if (!foundTruck)
    {
        std::cout
            << "No schedulable trucks.\n";
    }

    std::cout
        << "========================================\n";
}

void DispatchCenter::DisplayAssignedLoads(
    const Company& company)
{
    std::cout
        << "\n========== ASSIGNED LOADS ==========\n";

    bool foundAssignedLoad = false;

    for (const Load& load :
        company.GetLoads())
    {
        if (load.GetStatus() ==
            "Assigned")
        {
            foundAssignedLoad = true;

            std::cout
                << "\nLoad #"
                << load.GetLoadNumber()
                << "\n";

            std::cout
                << load.GetOrigin()
                << " -> "
                << load.GetDestination()
                << "\n";

            std::cout
                << "Distance: "
                << load.GetDistanceMiles()
                << " miles\n";

            std::cout
                << "Primary Driver ID: "
                << load.GetAssignedDriverID()
                << "\n";

            if (load.IsTeamLoad())
            {
                std::cout
                    << "Team Driver ID: "
                    << load.
                    GetAssignedSecondDriverID()
                    << "\n";
            }

            std::cout
                << "Truck #"
                << load.
                GetAssignedTruckUnitNumber()
                << "\n";

            std::cout
                << "Operation: "
                << (load.IsTeamLoad()
                    ? "TEAM"
                    : "SOLO")
                << "\n";

            std::cout
                << "Scheduled Start: "
                << HOSManager::
                FormatDateTime(
                    load.
                    GetDispatchStartEpoch()
                )
                << "\n";

            std::cout
                << "Estimated Delivery: "
                << HOSManager::
                FormatDateTime(
                    load.
                    GetEstimatedDeliveryEpoch()
                )
                << "\n";

            std::cout
                << "Deadline: "
                << HOSManager::
                FormatDateTime(
                    load.
                    GetDeliveryDeadlineEpoch()
                )
                << "\n";
        }
    }

    if (!foundAssignedLoad)
    {
        std::cout
            << "No loads are currently assigned.\n";
    }

    std::cout
        << "====================================\n";
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
                    << "\nDispatch Center is not "
                    << "connected to a company.\n";

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
                    << "\nA load cannot be scheduled "
                    << "until a load, driver, and truck "
                    << "are available for scheduling.\n";

                break;
            }

            std::cout
                << "\n========== ASSIGN LOAD ==========\n";

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
                    << "Load #"
                    << loadNumber
                    << " does not exist.\n";

                break;
            }

            if (selectedLoad->GetStatus() !=
                "Available")
            {
                std::cout
                    << "Load #"
                    << loadNumber
                    << " is currently "
                    << selectedLoad->GetStatus()
                    << " and cannot be assigned.\n";

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
                    << "Driver ID "
                    << driverID
                    << " does not exist.\n";

                break;
            }

            if (primaryDriver->GetStatus() !=
                "Available")
            {
                std::cout
                    << "Driver ID "
                    << driverID
                    << " is currently "
                    << primaryDriver->GetStatus()
                    << " and cannot be scheduled.\n";

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
                    << "Truck #"
                    << truckUnitNumber
                    << " does not exist.\n";

                break;
            }

            if (selectedTruck->GetStatus() !=
                "Available")
            {
                std::cout
                    << "Truck #"
                    << truckUnitNumber
                    << " is currently "
                    << selectedTruck->GetStatus()
                    << " and cannot be scheduled.\n";

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
                    << "\nThe primary driver cannot "
                    << "legally deliver this load "
                    << "before the deadline.\n";

                if (!HasSecondAvailableDriver(
                    *company,
                    driverID))
                {
                    std::cout
                        << "No second available driver "
                        << "exists for team operation.\n";

                    std::cout
                        << "Load cannot be dispatched.\n";

                    break;
                }

                bool tryTeam =
                    InputManager::ReadYesNo(
                        "Try a team operation? (Y/N): "
                    );

                if (!tryTeam)
                {
                    std::cout
                        << "Dispatch cancelled. "
                        << "No changes were made.\n";

                    break;
                }

                std::cout
                    << "\n========== AVAILABLE TEAM DRIVERS ==========\n";

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
                            << "Driver ID "
                            << driver.GetDriverID()
                            << " | "
                            << driver.GetName()
                            << " | HOS: "
                            << driver.
                            GetProjectedDrivingHoursAt(
                                candidateTime
                            )
                            << " | Rest: "
                            << driver.
                            GetProjectedRestHoursAt(
                                candidateTime
                            )
                            << "\n";

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
                        << "Driver ID "
                        << secondDriverID
                        << " does not exist.\n";

                    break;
                }

                if (secondDriverID ==
                    driverID)
                {
                    std::cout
                        << "The team driver must "
                        << "be a different driver.\n";

                    break;
                }

                if (secondDriver->GetStatus() !=
                    "Available")
                {
                    std::cout
                        << "Driver ID "
                        << secondDriverID
                        << " is not available.\n";

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
                        << "\nEven a two-driver team "
                        << "cannot legally meet "
                        << "this delivery deadline.\n";

                    std::cout
                        << "Load cannot be dispatched.\n";

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
                << "\n========== TRIP FUEL PLAN ==========\n";

            std::cout
                << "Current Fuel: "
                << selectedTruck->
                GetFuelGallons()
                << " gallons\n";

            std::cout
                << "Trip Fuel Required: "
                << tripFuelNeeded
                << " gallons\n";

            std::cout
                << "Required Delivery Reserve: "
                << minimumArrivalFuel
                << " gallons (50% tank)\n";

            std::cout
                << "Estimated In-Trip Purchase: "
                << plannedFuelPurchase
                << " gallons\n";

            std::cout
                << "Estimated Fuel Cost: $"
                << plannedFuelCost
                << "\n";

            std::cout
                << "====================================\n";

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            std::cout
                << "\n========== DISPATCH CONFIRMATION ==========\n";

            std::cout
                << "Load: #"
                << loadNumber
                << " | "
                << selectedLoad->GetOrigin()
                << " -> "
                << selectedLoad->GetDestination()
                << "\n";

            std::cout
                << "Primary Driver: #"
                << driverID
                << " - "
                << primaryDriver->GetName()
                << "\n";

            if (secondDriver != nullptr)
            {
                std::cout
                    << "Team Driver: #"
                    << secondDriverID
                    << " - "
                    << secondDriver->GetName()
                    << "\n";
            }

            std::cout
                << "Truck: #"
                << truckUnitNumber
                << "\n";

            std::cout
                << "Scheduled Start: "
                << HOSManager::FormatDateTime(
                    dispatchStart
                )
                << "\n";

            std::cout
                << "Estimated Delivery: "
                << HOSManager::FormatDateTime(
                    selectedPlan.estimatedDeliveryEpoch
                )
                << "\n";

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << "Load Revenue: $"
                << selectedLoad->GetPay()
                << "\n";

            std::cout
                << "Estimated In-Trip Fuel Cost: $"
                << plannedFuelCost
                << "\n";

            std::cout
                << "Driver Payroll on Completion: $"
                << estimatedPayroll
                << "\n";

            if (teamOperation)
            {
                std::cout
                    << "Team Pay Rate: $"
                    << PayrollManager::
                    GetTeamRatePerMile()
                    << " per mile per driver\n";
            }
            else
            {
                std::cout
                    << "Solo Pay Rate: $"
                    << PayrollManager::
                    GetSoloRatePerMile()
                    << " per mile\n";
            }

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            std::cout
                << "==========================================\n";

            if (!InputManager::ReadYesNo(
                "Confirm dispatch? (Y/N): "))
            {
                std::cout
                    << "Dispatch cancelled. "
                    << "No changes were made.\n";

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
                    << "\nLoad #"
                    << loadNumber
                    << " assigned successfully.\n";

                std::cout
                    << "Primary Driver: #"
                    << driverID
                    << "\n";

                if (secondDriverID > 0)
                {
                    std::cout
                        << "Team Driver: #"
                        << secondDriverID
                        << "\n";

                    std::cout
                        << "Operation: TEAM\n";
                }
                else
                {
                    std::cout
                        << "Operation: SOLO\n";
                }

                std::cout
                    << "Truck: #"
                    << truckUnitNumber
                    << "\n";

                std::cout
                    << "Scheduled Start: "
                    << HOSManager::
                    FormatDateTime(
                        dispatchStart
                    )
                    << "\n";

                std::cout
                    << "Estimated Delivery: "
                    << HOSManager::
                    FormatDateTime(
                        selectedPlan.
                        estimatedDeliveryEpoch
                    )
                    << "\n";
            }
            else
            {
                std::cout
                    << "\nUnable to assign load. "
                    << "The HOS, deadline, or "
                    << "resource state is no longer valid.\n";
            }

            break;
        }

        case 2:
        {
            if (!dispatchManager.
                HasCompany())
            {
                std::cout
                    << "\nDispatch Center is not "
                    << "connected to a company.\n";

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
                    << "\nDispatch Center is not "
                    << "connected to a company.\n";

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
                    << "\nThere are no assigned "
                    << "loads to complete.\n";

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
                    << "That load is not currently assigned.\n";

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
                << "\n========== LOAD COMPLETION CONFIRMATION ==========\n";

            std::cout
                << "Load: #"
                << loadNumber
                << " | "
                << selectedLoad->GetOrigin()
                << " -> "
                << selectedLoad->GetDestination()
                << "\n";

            std::cout
                << "Truck: #"
                << truckUnitNumber
                << "\n";

            std::cout
                << "Primary Driver: #"
                << primaryDriverID
                << "\n";

            if (secondDriverID > 0)
            {
                std::cout
                    << "Team Driver: #"
                    << secondDriverID
                    << "\n";
            }

            std::cout
                << "Simulated Delivery: "
                << HOSManager::FormatDateTime(
                    selectedLoad->
                    GetEstimatedDeliveryEpoch()
                )
                << "\n";

            std::cout
                << std::fixed
                << std::setprecision(2);

            std::cout
                << "Load Revenue: $"
                << selectedLoad->GetPay()
                << "\n";

            std::cout
                << "Trip Fuel Used: "
                << tripFuelUsed
                << " gallons\n";

            std::cout
                << "In-Trip Fuel Purchase: "
                << tripFuelPurchased
                << " gallons\n";

            std::cout
                << "In-Trip Fuel Cost: $"
                << tripFuelCost
                << "\n";

            if (teamOperation)
            {
                std::cout
                    << "Team Pay Rate: $"
                    << PayrollManager::
                    GetTeamRatePerMile()
                    << " per mile per driver\n";

                std::cout
                    << "Primary Driver Payroll: $"
                    << primaryDriverPay
                    << "\n";

                std::cout
                    << "Team Driver Payroll: $"
                    << secondDriverPay
                    << "\n";
            }
            else
            {
                std::cout
                    << "Solo Pay Rate: $"
                    << PayrollManager::
                    GetSoloRatePerMile()
                    << " per mile\n";

                std::cout
                    << "Primary Driver Payroll: $"
                    << primaryDriverPay
                    << "\n";
            }

            std::cout
                << "Total Driver Payroll Expense: $"
                << totalPayroll
                << "\n";

            std::cout
                << std::defaultfloat
                << std::setprecision(6);

            std::cout
                << "Completing this load will update HOS, fuel, "
                << "mileage, revenue, driver payroll, truck location, "
                << "and may trigger a post-trip breakdown.\n";

            std::cout
                << "==================================================\n";

            if (!InputManager::ReadYesNo(
                "Confirm load completion? (Y/N): "))
            {
                std::cout
                    << "Load completion cancelled. "
                    << "No changes were made.\n";

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
                    << "\nLoad #"
                    << loadNumber
                    << " completed successfully.\n";

                if (completedLoad != nullptr)
                {
                    std::cout
                        << "Simulated Delivery: "
                        << HOSManager::
                        FormatDateTime(
                            completedLoad->
                            GetEstimatedDeliveryEpoch()
                        )
                        << "\n";
                }

                std::cout
                    << std::fixed
                    << std::setprecision(2);

                if (primaryDriver != nullptr)
                {
                    std::cout
                        << "Primary Driver HOS Remaining: "
                        << primaryDriver->
                        GetDrivingHoursRemaining()
                        << " hours.\n";

                    std::cout
                        << "Primary Driver Rest Progress: "
                        << primaryDriver->
                        GetRestHoursAccumulated()
                        << " / 10.00 hours.\n";

                    std::cout
                        << std::defaultfloat
                        << std::setprecision(6);

                    std::cout
                        << "Primary Driver Scheduled Available: "
                        << HOSManager::
                        FormatDateTime(
                            primaryDriver->
                            GetAvailableAtEpoch()
                        )
                        << "\n";

                    std::cout
                        << std::fixed
                        << std::setprecision(2);
                }

                if (secondDriver != nullptr)
                {
                    std::cout
                        << "Team Driver HOS Remaining: "
                        << secondDriver->
                        GetDrivingHoursRemaining()
                        << " hours.\n";

                    std::cout
                        << "Team Driver Rest Progress: "
                        << secondDriver->
                        GetRestHoursAccumulated()
                        << " / 10.00 hours.\n";

                    std::cout
                        << std::defaultfloat
                        << std::setprecision(6);

                    std::cout
                        << "Team Driver Scheduled Available: "
                        << HOSManager::
                        FormatDateTime(
                            secondDriver->
                            GetAvailableAtEpoch()
                        )
                        << "\n";

                    std::cout
                        << std::fixed
                        << std::setprecision(2);
                }

                std::cout
                    << "Trip Fuel Used: "
                    << tripFuelUsed
                    << " gallons.\n";

                std::cout
                    << "In-Trip Fuel Purchased: "
                    << tripFuelPurchased
                    << " gallons.\n";

                std::cout
                    << "In-Trip Fuel Expense: $"
                    << tripFuelCost
                    << ".\n";

                if (completedTruck != nullptr)
                {
                    std::cout
                        << "Fuel at Delivery: "
                        << completedTruck->
                        GetFuelGallons()
                        << " / "
                        << completedTruck->
                        GetFuelTankCapacity()
                        << " gallons ("
                        << completedTruck->
                        GetFuelPercentage()
                        << "%).\n";

                    std::cout
                        << "Truck Mileage: "
                        << completedTruck->
                        GetMileage()
                        << " miles.\n";

                    std::cout
                        << std::defaultfloat
                        << std::setprecision(6);

                    std::cout
                        << "Truck Scheduled Available: "
                        << HOSManager::
                        FormatDateTime(
                            completedTruck->
                            GetAvailableAtEpoch()
                        )
                        << "\n";

                    std::cout
                        << std::fixed
                        << std::setprecision(2);
                }

                if (teamOperation)
                {
                    std::cout
                        << "Primary Driver Payroll: $"
                        << primaryDriverPay
                        << ".\n";

                    std::cout
                        << "Team Driver Payroll: $"
                        << secondDriverPay
                        << ".\n";
                }
                else
                {
                    std::cout
                        << "Driver Payroll: $"
                        << primaryDriverPay
                        << ".\n";
                }

                std::cout
                    << "Total Driver Payroll Expense: $"
                    << totalPayroll
                    << ".\n";

                std::cout
                    << "Company Revenue: $"
                    << company->GetRevenue()
                    << "\n";

                std::cout
                    << "Company Expenses: $"
                    << company->GetExpenses()
                    << "\n";

                std::cout
                    << "Company Balance: $"
                    << company->GetBalance()
                    << "\n";

                std::cout
                    << std::defaultfloat
                    << std::setprecision(6);
            }
            else
            {
                std::cout
                    << "\nUnable to complete load. "
                    << "Check the dispatch state "
                    << "and try again.\n";
            }

            break;
        }

        case 0:
            break;

        default:
            std::cout
                << "\nInvalid selection. "
                << "Please choose 0, 1, 2, or 3.\n";

            break;
        }
    }
}