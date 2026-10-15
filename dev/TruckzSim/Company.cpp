#include "Company.h"

Company::Company()
{
    companyName = "";
    revenue = 0.0;
    expenses = 0.0;
}

std::string Company::GetCompanyName() const
{
    return companyName;
}

void Company::SetCompanyName(const std::string& newCompanyName)
{
    companyName = newCompanyName;
}

double Company::GetRevenue() const
{
    return revenue;
}

void Company::AddRevenue(double amount)
{
    if (amount > 0.0)
    {
        revenue += amount;
    }
}

double Company::GetExpenses() const
{
    return expenses;
}

void Company::AddExpense(double amount)
{
    if (amount > 0.0)
    {
        expenses += amount;
    }
}

double Company::GetBalance() const
{
    return revenue - expenses;
}

void Company::AddTruck(const Truck& newTruck)
{
    trucks.push_back(newTruck);
}

void Company::AddDriver(const Driver& newDriver)
{
    drivers.push_back(newDriver);
}

void Company::AddLoad(const Load& newLoad)
{
    loads.push_back(newLoad);
}

bool Company::HasTruckUnitNumber(int unitNumber) const
{
    for (const Truck& truck : trucks)
    {
        if (truck.GetUnitNumber() == unitNumber)
        {
            return true;
        }
    }

    return false;
}

bool Company::HasDriverID(int driverID) const
{
    for (const Driver& driver : drivers)
    {
        if (driver.GetDriverID() == driverID)
        {
            return true;
        }
    }

    return false;
}

bool Company::HasLoadNumber(int loadNumber) const
{
    for (const Load& load : loads)
    {
        if (load.GetLoadNumber() == loadNumber)
        {
            return true;
        }
    }

    return false;
}

const std::vector<Truck>& Company::GetTrucks() const
{
    return trucks;
}

const std::vector<Driver>& Company::GetDrivers() const
{
    return drivers;
}

const std::vector<Load>& Company::GetLoads() const
{
    return loads;
}

std::vector<Truck>& Company::GetTrucks()
{
    return trucks;
}

std::vector<Driver>& Company::GetDrivers()
{
    return drivers;
}

std::vector<Load>& Company::GetLoads()
{
    return loads;
}
