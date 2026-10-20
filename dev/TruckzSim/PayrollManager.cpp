#include "PayrollManager.h"

#include <iomanip>
#include <sstream>
#include <string>

#include "Company.h"
#include "Driver.h"
#include "FinancialTransaction.h"
#include "Load.h"

namespace
{
    constexpr double SOLO_RATE_PER_MILE = 0.60;
    constexpr double TEAM_RATE_PERCENT = 0.70;

    double GetTeamRate()
    {
        return SOLO_RATE_PER_MILE *
            TEAM_RATE_PERCENT;
    }

    std::string BuildPayrollDescription(
        const Driver& driver,
        double distanceMiles,
        double ratePerMile,
        bool teamOperation)
    {
        std::ostringstream description;

        description
            << (teamOperation
                ? "Team Driver Pay - "
                : "Solo Driver Pay - ")
            << driver.GetName()
            << " - "
            << std::fixed
            << std::setprecision(2)
            << distanceMiles
            << " miles @ $"
            << ratePerMile
            << "/mile";

        return description.str();
    }

    void RecordDriverPayroll(
        Company& company,
        const Load& load,
        const Driver& driver,
        double ratePerMile,
        bool teamOperation,
        long long transactionEpoch)
    {
        FinancialTransaction payrollTransaction;

        payrollTransaction.transactionEpoch =
            transactionEpoch;

        payrollTransaction.category =
            "Driver Payroll";

        payrollTransaction.description =
            BuildPayrollDescription(
                driver,
                load.GetDistanceMiles(),
                ratePerMile,
                teamOperation
            );

        payrollTransaction.amount =
            load.GetDistanceMiles() *
            ratePerMile;

        payrollTransaction.truckUnitNumber =
            load.GetAssignedTruckUnitNumber();

        payrollTransaction.loadNumber =
            load.GetLoadNumber();

        payrollTransaction.primaryDriverID =
            driver.GetDriverID();

        company.RecordExpenseTransaction(
            payrollTransaction
        );
    }
}

double PayrollManager::GetSoloRatePerMile()
{
    return SOLO_RATE_PER_MILE;
}

double PayrollManager::GetTeamRatePerMile()
{
    return GetTeamRate();
}

double PayrollManager::CalculateDriverPay(
    double distanceMiles,
    bool teamOperation)
{
    if (distanceMiles <= 0.0)
    {
        return 0.0;
    }

    double ratePerMile =
        teamOperation
        ? GetTeamRatePerMile()
        : GetSoloRatePerMile();

    return distanceMiles *
        ratePerMile;
}

double PayrollManager::CalculateTotalPayroll(
    double distanceMiles,
    bool teamOperation)
{
    double driverPay =
        CalculateDriverPay(
            distanceMiles,
            teamOperation
        );

    if (teamOperation)
    {
        return driverPay * 2.0;
    }

    return driverPay;
}

void PayrollManager::RecordLoadPayroll(
    Company& company,
    const Load& load,
    long long transactionEpoch)
{
    int primaryDriverID =
        load.GetAssignedDriverID();

    int secondDriverID =
        load.GetAssignedSecondDriverID();

    const Driver* primaryDriver =
        company.FindDriver(
            primaryDriverID
        );

    if (primaryDriver == nullptr)
    {
        return;
    }

    bool teamOperation =
        secondDriverID > 0;

    double ratePerMile =
        teamOperation
        ? GetTeamRatePerMile()
        : GetSoloRatePerMile();

    const Driver* secondDriver =
        nullptr;

    if (teamOperation)
    {
        secondDriver =
            company.FindDriver(
                secondDriverID
            );

        if (secondDriver == nullptr)
        {
            return;
        }
    }

    RecordDriverPayroll(
        company,
        load,
        *primaryDriver,
        ratePerMile,
        teamOperation,
        transactionEpoch
    );

    if (secondDriver != nullptr)
    {
        RecordDriverPayroll(
            company,
            load,
            *secondDriver,
            ratePerMile,
            true,
            transactionEpoch
        );
    }
}
