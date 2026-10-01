#pragma once

#include<string>
#include<vector>

#include "Truck.h"
#include "Driver.h"
#include "Load.h"

class Company
{
private:
	std::string companyName;

	std::vector<Truck> trucks;
	std::vector<Driver> drivers;
	std::vector<Load> loads;

public:
	Company();

	std::string GetCompanyName() const;
	void SetCompanyName(const std::string& newCompanyName);

	void AddTruck(const Truck& newTruck);
	void AddDriver(const Driver& newDriver);
	void AddLoad(const Load& newLoad);

	const std::vector<Truck>& GetTrucks() const;
	const std::vector<Driver>& GetDrivers() const;
	const std::vector<Load>& GetLoads() const;

};