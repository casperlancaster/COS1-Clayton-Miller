#include "ValidationManager.h"

#include <cmath>
#include <iostream>
#include <set>
#include <string>

#include "Company.h"
#include "Driver.h"
#include "FinancialTransaction.h"
#include "Load.h"
#include "Truck.h"

namespace
{
    constexpr double EPSILON = 0.01;

    bool IsFinite(double value)
    {
        return std::isfinite(value);
    }

    bool IsValidTruckStatus(
        const std::string& status)
    {
        return status == "Available" ||
            status == "Dispatched" ||
            status == "Maintenance" ||
            status == "Breakdown";
    }

    bool IsValidDriverStatus(
        const std::string& status)
    {
        return status == "Available" ||
            status == "Dispatched";
    }

    bool IsValidLoadStatus(
        const std::string& status)
    {
        return status == "Available" ||
            status == "Assigned" ||
            status == "Completed";
    }

    std::string NumberText(int value)
    {
        return std::to_string(value);
    }
}

bool ValidationManager::ValidateCompany(
    const Company& company,
    std::vector<std::string>& issues)
{
    issues.clear();

    if (company.GetCompanyName().empty())
    {
        issues.push_back(
            "Company name is empty."
        );
    }

    if (!IsFinite(company.GetRevenue()) ||
        company.GetRevenue() < 0.0)
    {
        issues.push_back(
            "Company revenue is invalid."
        );
    }

    if (!IsFinite(company.GetExpenses()) ||
        company.GetExpenses() < 0.0)
    {
        issues.push_back(
            "Company expenses are invalid."
        );
    }

    if (company.GetDateFormat() != "MM/DD/YYYY" &&
        company.GetDateFormat() != "YYYY-MM-DD")
    {
        issues.push_back(
            "Date format setting is invalid."
        );
    }

    if (company.GetTimeFormat() != "12-hour" &&
        company.GetTimeFormat() != "24-hour")
    {
        issues.push_back(
            "Time format setting is invalid."
        );
    }

    std::set<int> truckNumbers;
    std::set<int> driverIDs;
    std::set<int> loadNumbers;

    for (const Truck& truck :
        company.GetTrucks())
    {
        int unitNumber =
            truck.GetUnitNumber();

        if (unitNumber <= 0)
        {
            issues.push_back(
                "A truck has an invalid unit number."
            );
        }
        else if (!truckNumbers.insert(
            unitNumber).second)
        {
            issues.push_back(
                "Duplicate truck unit number #" +
                NumberText(unitNumber) +
                "."
            );
        }

        if (!IsValidTruckStatus(
            truck.GetStatus()))
        {
            issues.push_back(
                "Truck #" +
                NumberText(unitNumber) +
                " has invalid status '" +
                truck.GetStatus() +
                "'."
            );
        }

        if (truck.GetCurrentLocation().empty())
        {
            issues.push_back(
                "Truck #" +
                NumberText(unitNumber) +
                " has an empty current location."
            );
        }

        if (!IsFinite(
            truck.GetMileage()) ||
            truck.GetMileage() < 0.0)
        {
            issues.push_back(
                "Truck #" +
                NumberText(unitNumber) +
                " has invalid mileage."
            );
        }

        if (!IsFinite(
            truck.GetLastServiceMileage()) ||
            truck.GetLastServiceMileage() < 0.0 ||
            truck.GetLastServiceMileage() >
            truck.GetMileage())
        {
            issues.push_back(
                "Truck #" +
                NumberText(unitNumber) +
                " has an invalid last-service mileage."
            );
        }

        if (!IsFinite(
            truck.GetServiceIntervalMiles()) ||
            truck.GetServiceIntervalMiles() <= 0.0)
        {
            issues.push_back(
                "Truck #" +
                NumberText(unitNumber) +
                " has an invalid service interval."
            );
        }

        if (!IsFinite(
            truck.GetFuelTankCapacity()) ||
            truck.GetFuelTankCapacity() <= 0.0 ||
            !IsFinite(
                truck.GetFuelGallons()) ||
            truck.GetFuelGallons() < 0.0 ||
            truck.GetFuelGallons() >
            truck.GetFuelTankCapacity() ||
            !IsFinite(
                truck.GetMilesPerGallon()) ||
            truck.GetMilesPerGallon() <= 0.0)
        {
            issues.push_back(
                "Truck #" +
                NumberText(unitNumber) +
                " has invalid fuel data."
            );
        }

        if (truck.GetAvailableAtEpoch() < 0)
        {
            issues.push_back(
                "Truck #" +
                NumberText(unitNumber) +
                " has an invalid availability time."
            );
        }

        if (truck.HasBreakdown())
        {
            if (truck.GetStatus() !=
                "Breakdown")
            {
                issues.push_back(
                    "Truck #" +
                    NumberText(unitNumber) +
                    " has an active breakdown but "
                    "is not marked Breakdown."
                );
            }

            if (truck.GetBreakdownType().empty() ||
                !IsFinite(
                    truck.GetBreakdownPartsCost()) ||
                truck.GetBreakdownPartsCost() < 0.0 ||
                !IsFinite(
                    truck.GetBreakdownLaborCost()) ||
                truck.GetBreakdownLaborCost() < 0.0 ||
                !IsFinite(
                    truck.
                    GetBreakdownServiceCallCharge()) ||
                truck.
                GetBreakdownServiceCallCharge() < 0.0 ||
                !IsFinite(
                    truck.GetBreakdownRepairHours()) ||
                truck.GetBreakdownRepairHours() <= 0.0)
            {
                issues.push_back(
                    "Truck #" +
                    NumberText(unitNumber) +
                    " has invalid breakdown data."
                );
            }
        }
        else if (truck.GetStatus() ==
            "Breakdown")
        {
            issues.push_back(
                "Truck #" +
                NumberText(unitNumber) +
                " is marked Breakdown without "
                "active breakdown data."
            );
        }
    }

    for (const Driver& driver :
        company.GetDrivers())
    {
        int driverID =
            driver.GetDriverID();

        if (driverID <= 0)
        {
            issues.push_back(
                "A driver has an invalid Driver ID."
            );
        }
        else if (!driverIDs.insert(
            driverID).second)
        {
            issues.push_back(
                "Duplicate Driver ID #" +
                NumberText(driverID) +
                "."
            );
        }

        if (driver.GetName().empty())
        {
            issues.push_back(
                "Driver #" +
                NumberText(driverID) +
                " has an empty name."
            );
        }

        if (!IsValidDriverStatus(
            driver.GetStatus()))
        {
            issues.push_back(
                "Driver #" +
                NumberText(driverID) +
                " has invalid status '" +
                driver.GetStatus() +
                "'."
            );
        }

        if (!IsFinite(
            driver.GetDrivingHoursRemaining()) ||
            driver.GetDrivingHoursRemaining() < 0.0 ||
            driver.GetDrivingHoursRemaining() >
            11.000001 ||
            !IsFinite(
                driver.GetRestHoursAccumulated()) ||
            driver.GetRestHoursAccumulated() < 0.0 ||
            driver.GetRestHoursAccumulated() >
            10.000001 ||
            driver.GetAvailableAtEpoch() < 0)
        {
            issues.push_back(
                "Driver #" +
                NumberText(driverID) +
                " has invalid HOS or availability data."
            );
        }
    }

    std::set<int> assignedTruckNumbers;
    std::set<int> assignedDriverIDs;

    for (const Load& load :
        company.GetLoads())
    {
        int loadNumber =
            load.GetLoadNumber();

        if (loadNumber <= 0)
        {
            issues.push_back(
                "A load has an invalid load number."
            );
        }
        else if (!loadNumbers.insert(
            loadNumber).second)
        {
            issues.push_back(
                "Duplicate load number #" +
                NumberText(loadNumber) +
                "."
            );
        }

        if (load.GetOrigin().empty() ||
            load.GetDestination().empty())
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " has an empty origin or destination."
            );
        }

        if (!IsFinite(
            load.GetDistanceMiles()) ||
            load.GetDistanceMiles() < 0.0 ||
            !IsFinite(
                load.GetPay()) ||
            load.GetPay() < 0.0)
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " has invalid distance or pay data."
            );
        }

        if (!IsValidLoadStatus(
            load.GetStatus()))
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " has invalid status '" +
                load.GetStatus() +
                "'."
            );
        }

        int primaryDriverID =
            load.GetAssignedDriverID();

        int secondDriverID =
            load.GetAssignedSecondDriverID();

        int truckUnitNumber =
            load.GetAssignedTruckUnitNumber();

        if (primaryDriverID < 0 ||
            secondDriverID < 0 ||
            truckUnitNumber < 0 ||
            load.GetDeliveryDeadlineEpoch() < 0 ||
            load.GetDispatchStartEpoch() < 0 ||
            load.GetEstimatedDeliveryEpoch() < 0)
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " has invalid assignment or time data."
            );
        }

        if (primaryDriverID > 0 &&
            driverIDs.count(
                primaryDriverID) == 0)
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " references missing Driver #" +
                NumberText(primaryDriverID) +
                "."
            );
        }

        if (secondDriverID > 0 &&
            driverIDs.count(
                secondDriverID) == 0)
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " references missing team Driver #" +
                NumberText(secondDriverID) +
                "."
            );
        }

        if (truckUnitNumber > 0 &&
            truckNumbers.count(
                truckUnitNumber) == 0)
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " references missing Truck #" +
                NumberText(truckUnitNumber) +
                "."
            );
        }

        if (primaryDriverID > 0 &&
            primaryDriverID ==
            secondDriverID)
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " assigns the same driver twice."
            );
        }

        if (load.GetDispatchStartEpoch() > 0 &&
            load.GetEstimatedDeliveryEpoch() > 0 &&
            load.GetEstimatedDeliveryEpoch() <
            load.GetDispatchStartEpoch())
        {
            issues.push_back(
                "Load #" +
                NumberText(loadNumber) +
                " has delivery scheduled before dispatch."
            );
        }

        if (load.GetStatus() ==
            "Available")
        {
            if (primaryDriverID > 0 ||
                secondDriverID > 0 ||
                truckUnitNumber > 0)
            {
                issues.push_back(
                    "Available Load #" +
                    NumberText(loadNumber) +
                    " still contains dispatch assignments."
                );
            }
        }
        else
        {
            if (primaryDriverID <= 0 ||
                truckUnitNumber <= 0)
            {
                issues.push_back(
                    "Load #" +
                    NumberText(loadNumber) +
                    " is " +
                    load.GetStatus() +
                    " but is missing a primary driver or truck."
                );
            }
        }

        if (load.GetStatus() ==
            "Assigned")
        {
            if (primaryDriverID > 0 &&
                !assignedDriverIDs.insert(
                    primaryDriverID).second)
            {
                issues.push_back(
                    "Driver #" +
                    NumberText(primaryDriverID) +
                    " is assigned to multiple active loads."
                );
            }

            if (secondDriverID > 0 &&
                !assignedDriverIDs.insert(
                    secondDriverID).second)
            {
                issues.push_back(
                    "Driver #" +
                    NumberText(secondDriverID) +
                    " is assigned to multiple active loads."
                );
            }

            if (truckUnitNumber > 0 &&
                !assignedTruckNumbers.insert(
                    truckUnitNumber).second)
            {
                issues.push_back(
                    "Truck #" +
                    NumberText(truckUnitNumber) +
                    " is assigned to multiple active loads."
                );
            }
        }
    }

    double transactionRevenue =
        0.0;

    double transactionExpenses =
        0.0;

    for (const FinancialTransaction& transaction :
        company.GetFinancialTransactions())
    {
        if (transaction.type != "Income" &&
            transaction.type != "Expense")
        {
            issues.push_back(
                "A financial transaction has "
                "an invalid type."
            );
        }

        if (transaction.category.empty())
        {
            issues.push_back(
                "A financial transaction has "
                "an empty category."
            );
        }

        if (transaction.transactionEpoch < 0 ||
            !IsFinite(transaction.amount) ||
            transaction.amount <= 0.0 ||
            transaction.truckUnitNumber < 0 ||
            transaction.loadNumber < 0 ||
            transaction.primaryDriverID < 0 ||
            transaction.secondDriverID < 0 ||
            !IsFinite(transaction.gallons) ||
            transaction.gallons < 0.0 ||
            !IsFinite(
                transaction.pricePerGallon) ||
            transaction.pricePerGallon < 0.0 ||
            !IsFinite(transaction.partsCost) ||
            transaction.partsCost < 0.0 ||
            !IsFinite(transaction.laborCost) ||
            transaction.laborCost < 0.0 ||
            !IsFinite(
                transaction.serviceCallCharge) ||
            transaction.serviceCallCharge < 0.0)
        {
            issues.push_back(
                "A financial transaction contains "
                "invalid numeric data."
            );
        }

        if (transaction.truckUnitNumber > 0 &&
            truckNumbers.count(
                transaction.truckUnitNumber) == 0)
        {
            issues.push_back(
                "A financial transaction references "
                "missing Truck #" +
                NumberText(
                    transaction.truckUnitNumber) +
                "."
            );
        }

        if (transaction.loadNumber > 0 &&
            loadNumbers.count(
                transaction.loadNumber) == 0)
        {
            issues.push_back(
                "A financial transaction references "
                "missing Load #" +
                NumberText(
                    transaction.loadNumber) +
                "."
            );
        }

        if (transaction.primaryDriverID > 0 &&
            driverIDs.count(
                transaction.primaryDriverID) == 0)
        {
            issues.push_back(
                "A financial transaction references "
                "missing Driver #" +
                NumberText(
                    transaction.primaryDriverID) +
                "."
            );
        }

        if (transaction.secondDriverID > 0 &&
            driverIDs.count(
                transaction.secondDriverID) == 0)
        {
            issues.push_back(
                "A financial transaction references "
                "missing team Driver #" +
                NumberText(
                    transaction.secondDriverID) +
                "."
            );
        }

        if (transaction.type == "Income" &&
            IsFinite(transaction.amount))
        {
            transactionRevenue +=
                transaction.amount;
        }
        else if (transaction.type == "Expense" &&
            IsFinite(transaction.amount))
        {
            transactionExpenses +=
                transaction.amount;
        }
    }

    if (std::fabs(
        transactionRevenue -
        company.GetRevenue()) >
        EPSILON)
    {
        issues.push_back(
            "Income transaction total does not "
            "match company revenue."
        );
    }

    if (std::fabs(
        transactionExpenses -
        company.GetExpenses()) >
        EPSILON)
    {
        issues.push_back(
            "Expense transaction total does not "
            "match company expenses."
        );
    }

    return issues.empty();
}

bool ValidationManager::DisplayReport(
    const Company& company)
{
    std::vector<std::string> issues;

    bool valid =
        ValidateCompany(
            company,
            issues
        );

    std::cout
        << "\n========== SYSTEM VALIDATION ==========\n";

    std::cout
        << "Company: "
        << company.GetCompanyName()
        << "\n";

    std::cout
        << "Trucks Checked: "
        << company.GetTrucks().size()
        << "\n";

    std::cout
        << "Drivers Checked: "
        << company.GetDrivers().size()
        << "\n";

    std::cout
        << "Loads Checked: "
        << company.GetLoads().size()
        << "\n";

    std::cout
        << "Transactions Checked: "
        << company.GetFinancialTransactions().size()
        << "\n";

    if (valid)
    {
        std::cout
            << "Result: PASS\n";

        std::cout
            << "No consistency or edge-case "
            << "validation issues were found.\n";
    }
    else
    {
        std::cout
            << "Result: FAILED\n";

        std::cout
            << "Issues Found: "
            << issues.size()
            << "\n";

        for (std::size_t i = 0;
            i < issues.size();
            i++)
        {
            std::cout
                << (i + 1)
                << ". "
                << issues[i]
                << "\n";
        }
    }

    std::cout
        << "=======================================\n";

    return valid;
}