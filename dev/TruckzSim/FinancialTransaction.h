#pragma once

#include <string>

struct FinancialTransaction
{
    long long transactionEpoch = 0;

    std::string type;
    std::string category;
    std::string description;

    double amount = 0.0;

    int truckUnitNumber = 0;
    int loadNumber = 0;
    int primaryDriverID = 0;
    int secondDriverID = 0;

    double gallons = 0.0;
    double pricePerGallon = 0.0;

    double partsCost = 0.0;
    double laborCost = 0.0;
    double serviceCallCharge = 0.0;
};