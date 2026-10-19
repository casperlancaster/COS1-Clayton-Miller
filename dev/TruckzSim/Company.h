#pragma once

#include <string>
#include <vector>

#include "Driver.h"
#include "FinancialTransaction.h"
#include "Load.h"
#include "Truck.h"

class Company
{
private:
    std::string companyName;
    double revenue;
    double expenses;

    std::vector<Truck> trucks;
    std::vector<Driver> drivers;
    std::vector<Load> loads;
    std::vector<FinancialTransaction> financialTransactions;

public:
    Company();

    std::string GetCompanyName() const;
    void SetCompanyName(const std::string& newCompanyName);

    double GetRevenue() const;
    void AddRevenue(double amount);

    double GetExpenses() const;
    void AddExpense(double amount);
    double GetBalance() const;

    void RecordIncomeTransaction(
        const FinancialTransaction& transaction
    );

    void RecordExpenseTransaction(
        const FinancialTransaction& transaction
    );

    void RestoreFinancialTransaction(
        const FinancialTransaction& transaction
    );

    void AddTruck(const Truck& newTruck);
    void AddDriver(const Driver& newDriver);
    void AddLoad(const Load& newLoad);

    bool HasTruckUnitNumber(int unitNumber) const;
    bool HasDriverID(int driverID) const;
    bool HasLoadNumber(int loadNumber) const;

    std::vector<Truck>& GetTrucks();
    std::vector<Driver>& GetDrivers();
    std::vector<Load>& GetLoads();

    const std::vector<Truck>& GetTrucks() const;
    const std::vector<Driver>& GetDrivers() const;
    const std::vector<Load>& GetLoads() const;

    const std::vector<FinancialTransaction>&
        GetFinancialTransactions() const;
};