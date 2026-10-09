#pragma once

#include <string>
#include <vector>

#include "Truck.h"
#include "Driver.h"
#include "Load.h"

class Company
{
private:
    std::string companyName;
    double revenue;

    std::vector<Truck> trucks;
    std::vector<Driver> drivers;
    std::vector<Load> loads;

public:
    Company();

    std::string GetCompanyName() const;
    void SetCompanyName(const std::string& newCompanyName);

    double GetRevenue() const;
    void AddRevenue(double amount);

    void AddTruck(const Truck& newTruck);
    void AddDriver(const Driver& newDriver);
    void AddLoad(const Load& newLoad);

    std::vector<Truck>& GetTrucks();
    std::vector<Driver>& GetDrivers();
    std::vector<Load>& GetLoads();

    const std::vector<Truck>& GetTrucks() const;
    const std::vector<Driver>& GetDrivers() const;
    const std::vector<Load>& GetLoads() const;
};
