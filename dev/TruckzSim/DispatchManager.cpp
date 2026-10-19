#include "DispatchManager.h"

#include <iomanip>
#include <iostream>

#include "HOSManager.h"
#include "RepairManager.h"

namespace
{
    constexpr double FUEL_PRICE_PER_GALLON =
        4.00;
}

DispatchManager::DispatchManager()
{
    company = nullptr;
}

DispatchManager::DispatchManager(
    Company* newCompany)
{
    company = newCompany;
}

Company* DispatchManager::GetCompany() const
{
    return company;
}

void DispatchManager::SetCompany(
    Company* newCompany)
{
    company = newCompany;
}

bool DispatchManager::HasCompany() const
{
    return company != nullptr;
}

bool DispatchManager::AssignLoad(
    int loadNumber,
    int driverID,
    int truckUnitNumber,
    int secondDriverID,
    long long dispatchStartEpoch)
{
    if (company == nullptr)
    {
        return false;
    }

    std::vector<Driver>& drivers =
        company->GetDrivers();

    std::vector<Truck>& trucks =
        company->GetTrucks();

    std::vector<Load>& loads =
        company->GetLoads();

    Driver* primaryDriver = nullptr;
    Driver* secondDriver = nullptr;
    Truck* selectedTruck = nullptr;
    Load* selectedLoad = nullptr;

    for (Driver& driver :
        drivers)
    {
        if (driver.GetDriverID() ==
            driverID)
        {
            primaryDriver =
                &driver;

            break;
        }
    }

    if (secondDriverID > 0)
    {
        for (Driver& driver :
            drivers)
        {
            if (driver.GetDriverID() ==
                secondDriverID)
            {
                secondDriver =
                    &driver;

                break;
            }
        }
    }

    for (Truck& truck :
        trucks)
    {
        if (truck.GetUnitNumber() ==
            truckUnitNumber)
        {
            selectedTruck =
                &truck;

            break;
        }
    }

    for (Load& load :
        loads)
    {
        if (load.GetLoadNumber() ==
            loadNumber)
        {
            selectedLoad =
                &load;

            break;
        }
    }

    if (primaryDriver == nullptr ||
        selectedTruck == nullptr ||
        selectedLoad == nullptr)
    {
        return false;
    }

    if (secondDriverID > 0 &&
        secondDriver == nullptr)
    {
        return false;
    }

    if (secondDriverID ==
        driverID)
    {
        return false;
    }

    if (primaryDriver->GetStatus() !=
        "Available" ||
        selectedTruck->GetStatus() !=
        "Available" ||
        selectedLoad->GetStatus() !=
        "Available")
    {
        return false;
    }

    if (selectedTruck->HasBreakdown())
    {
        return false;
    }

    if (secondDriver != nullptr &&
        secondDriver->GetStatus() !=
        "Available")
    {
        return false;
    }

    if (dispatchStartEpoch <
        primaryDriver->
        GetAvailableAtEpoch())
    {
        return false;
    }

    if (dispatchStartEpoch <
        selectedTruck->
        GetAvailableAtEpoch())
    {
        return false;
    }

    if (secondDriver != nullptr &&
        dispatchStartEpoch <
        secondDriver->
        GetAvailableAtEpoch())
    {
        return false;
    }

    double primaryHours =
        primaryDriver->
        GetProjectedDrivingHoursAt(
            dispatchStartEpoch
        );

    double primaryRest =
        primaryDriver->
        GetProjectedRestHoursAt(
            dispatchStartEpoch
        );

    HOSPlan hosPlan;

    if (secondDriver != nullptr)
    {
        double secondHours =
            secondDriver->
            GetProjectedDrivingHoursAt(
                dispatchStartEpoch
            );

        double secondRest =
            secondDriver->
            GetProjectedRestHoursAt(
                dispatchStartEpoch
            );

        hosPlan =
            HOSManager::CreateTeamPlan(
                primaryHours,
                primaryRest,
                secondHours,
                secondRest,
                selectedLoad->
                GetDistanceMiles(),
                dispatchStartEpoch,
                selectedLoad->
                GetDeliveryDeadlineEpoch()
            );
    }
    else
    {
        hosPlan =
            HOSManager::CreateSoloPlan(
                primaryHours,
                primaryRest,
                selectedLoad->
                GetDistanceMiles(),
                dispatchStartEpoch,
                selectedLoad->
                GetDeliveryDeadlineEpoch()
            );
    }

    if (!hosPlan.valid ||
        !hosPlan.meetsDeadline)
    {
        return false;
    }

    primaryDriver->
        SetStatus(
            "Dispatched"
        );

    if (secondDriver != nullptr)
    {
        secondDriver->
            SetStatus(
                "Dispatched"
            );
    }

    selectedTruck->
        SetStatus(
            "Dispatched"
        );

    selectedLoad->
        SetStatus(
            "Assigned"
        );

    selectedLoad->
        SetAssignedDriverID(
            driverID
        );

    selectedLoad->
        SetAssignedSecondDriverID(
            secondDriverID
        );

    selectedLoad->
        SetAssignedTruckUnitNumber(
            truckUnitNumber
        );

    selectedLoad->
        SetDispatchStartEpoch(
            dispatchStartEpoch
        );

    selectedLoad->
        SetEstimatedDeliveryEpoch(
            hosPlan.
            estimatedDeliveryEpoch
        );

    return true;
}

bool DispatchManager::CompleteLoad(
    int loadNumber)
{
    if (company == nullptr)
    {
        return false;
    }

    std::vector<Driver>& drivers =
        company->GetDrivers();

    std::vector<Truck>& trucks =
        company->GetTrucks();

    std::vector<Load>& loads =
        company->GetLoads();

    Load* selectedLoad = nullptr;

    for (Load& load :
        loads)
    {
        if (load.GetLoadNumber() ==
            loadNumber)
        {
            selectedLoad =
                &load;

            break;
        }
    }

    if (selectedLoad == nullptr ||
        selectedLoad->GetStatus() !=
        "Assigned")
    {
        return false;
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

    Driver* primaryDriver = nullptr;
    Driver* secondDriver = nullptr;
    Truck* selectedTruck = nullptr;

    for (Driver& driver :
        drivers)
    {
        if (driver.GetDriverID() ==
            primaryDriverID)
        {
            primaryDriver =
                &driver;
        }

        if (secondDriverID > 0 &&
            driver.GetDriverID() ==
            secondDriverID)
        {
            secondDriver =
                &driver;
        }
    }

    for (Truck& truck :
        trucks)
    {
        if (truck.GetUnitNumber() ==
            truckUnitNumber)
        {
            selectedTruck =
                &truck;

            break;
        }
    }

    if (primaryDriver == nullptr ||
        selectedTruck == nullptr)
    {
        return false;
    }

    if (secondDriverID > 0 &&
        secondDriver == nullptr)
    {
        return false;
    }

    if (primaryDriver->GetStatus() !=
        "Dispatched" ||
        selectedTruck->GetStatus() !=
        "Dispatched")
    {
        return false;
    }

    if (secondDriver != nullptr &&
        secondDriver->GetStatus() !=
        "Dispatched")
    {
        return false;
    }

    long long dispatchStart =
        selectedLoad->
        GetDispatchStartEpoch();

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

    HOSPlan hosPlan;

    if (secondDriver != nullptr)
    {
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

        hosPlan =
            HOSManager::CreateTeamPlan(
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
    }
    else
    {
        hosPlan =
            HOSManager::CreateSoloPlan(
                primaryHours,
                primaryRest,
                selectedLoad->
                GetDistanceMiles(),
                dispatchStart,
                selectedLoad->
                GetDeliveryDeadlineEpoch()
            );
    }

    if (!hosPlan.valid)
    {
        return false;
    }

    double gallonsPurchased =
        0.0;

    if (!selectedTruck->
        CompleteTripFuel(
            selectedLoad->
            GetDistanceMiles(),
            gallonsPurchased))
    {
        return false;
    }

    double fuelExpense =
        gallonsPurchased *
        FUEL_PRICE_PER_GALLON;

    if (fuelExpense > 0.0)
    {
        FinancialTransaction fuelTransaction;

        fuelTransaction.transactionEpoch =
            hosPlan.estimatedDeliveryEpoch;

        fuelTransaction.category =
            "Fuel";

        fuelTransaction.description =
            "In-Trip Fuel Purchase";

        fuelTransaction.amount =
            fuelExpense;

        fuelTransaction.truckUnitNumber =
            truckUnitNumber;

        fuelTransaction.loadNumber =
            loadNumber;

        fuelTransaction.primaryDriverID =
            primaryDriverID;

        fuelTransaction.secondDriverID =
            secondDriverID;

        fuelTransaction.gallons =
            gallonsPurchased;

        fuelTransaction.pricePerGallon =
            FUEL_PRICE_PER_GALLON;

        company->RecordExpenseTransaction(
            fuelTransaction
        );
    }

    FinancialTransaction revenueTransaction;

    revenueTransaction.transactionEpoch =
        hosPlan.estimatedDeliveryEpoch;

    revenueTransaction.category =
        "Load Revenue";

    revenueTransaction.description =
        selectedLoad->GetOrigin() +
        " -> " +
        selectedLoad->GetDestination();

    revenueTransaction.amount =
        selectedLoad->GetPay();

    revenueTransaction.truckUnitNumber =
        truckUnitNumber;

    revenueTransaction.loadNumber =
        loadNumber;

    revenueTransaction.primaryDriverID =
        primaryDriverID;

    revenueTransaction.secondDriverID =
        secondDriverID;

    company->RecordIncomeTransaction(
        revenueTransaction
    );

    selectedTruck->SetMileage(
        selectedTruck->GetMileage() +
        selectedLoad->
        GetDistanceMiles()
    );

    primaryDriver->
        SetHOSState(
            hosPlan.
            primaryFinalHours,
            hosPlan.
            primaryFinalRestHours,
            hosPlan.
            estimatedDeliveryEpoch
        );

    if (secondDriver != nullptr)
    {
        secondDriver->
            SetHOSState(
                hosPlan.
                secondFinalHours,
                hosPlan.
                secondFinalRestHours,
                hosPlan.
                estimatedDeliveryEpoch
            );
    }

    selectedTruck->
        SetAvailableAtEpoch(
            hosPlan.
            estimatedDeliveryEpoch
        );

    selectedLoad->
        SetEstimatedDeliveryEpoch(
            hosPlan.
            estimatedDeliveryEpoch
        );

    selectedLoad->
        SetStatus(
            "Completed"
        );

    primaryDriver->
        SetStatus(
            "Available"
        );

    if (secondDriver != nullptr)
    {
        secondDriver->
            SetStatus(
                "Available"
            );
    }

    selectedTruck->
        SetStatus(
            "Available"
        );

    bool breakdownOccurred =
        RepairManager::RollForBreakdown(
            *selectedTruck,
            selectedLoad->
            GetDistanceMiles()
        );

    if (breakdownOccurred)
    {
        std::cout
            << "\n";
        std::cout
            << "!!!!!!!!!! POST-TRIP BREAKDOWN !!!!!!!!!!\n";

        std::cout
            << "Truck #"
            << selectedTruck->GetUnitNumber()
            << " suffered a mechanical failure.\n";

        std::cout
            << "Breakdown: "
            << selectedTruck->GetBreakdownType()
            << "\n";

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << "Parts Estimate: $"
            << selectedTruck->
            GetBreakdownPartsCost()
            << "\n";

        std::cout
            << "Labor Estimate: $"
            << selectedTruck->
            GetBreakdownLaborCost()
            << "\n";

        std::cout
            << "Service Call: $"
            << selectedTruck->
            GetBreakdownServiceCallCharge()
            << "\n";

        std::cout
            << "Estimated Total Repair: $"
            << selectedTruck->
            GetBreakdownTotalCost()
            << "\n";

        std::cout
            << "Estimated Repair Time: "
            << selectedTruck->
            GetBreakdownRepairHours()
            << " hours\n";

        std::cout
            << std::defaultfloat
            << std::setprecision(6);

        std::cout
            << "Truck status is now BREAKDOWN.\n";

        std::cout
            << "Use Fleet Management -> "
            << "Breakdown / Repair Center "
            << "before dispatching this truck again.\n";

        std::cout
            << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    }

    return true;
}