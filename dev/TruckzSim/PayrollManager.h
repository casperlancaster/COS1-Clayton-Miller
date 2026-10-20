#pragma once

class Company;
class Load;

class PayrollManager
{
public:
    static double GetSoloRatePerMile();
    static double GetTeamRatePerMile();

    static double CalculateDriverPay(
        double distanceMiles,
        bool teamOperation
    );

    static double CalculateTotalPayroll(
        double distanceMiles,
        bool teamOperation
    );

    static void RecordLoadPayroll(
        Company& company,
        const Load& load,
        long long transactionEpoch
    );
};
