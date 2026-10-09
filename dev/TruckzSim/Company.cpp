#include "Company.h"

Company::Company()
{
    companyName = "";
    revenue = 0.0;
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
