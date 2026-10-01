#include "DispatchManager.h"

DispatchManager::DispatchManager()
{
	company = nullptr;
}

DispatchManager::DispatchManager(Company* newCompany)
{
	company = newCompany;
}

Company* DispatchManager::GetCompany() const
{
	return company;
}

void DispatchManager::SetCompany(Company* newCompany)
{
	company = newCompany;
}

bool DispatchManager::HasCompany() const
{
	return company != nullptr;
}

bool DispatchManager::AssignLoad(int loadNumber, int driverID, int truckUnitNumber)
{
	if (company == nullptr)
	{
		return false;
	}

	std::vector<Driver>& drivers = company->GetDrivers();
	std::vector<Truck>& trucks = company->GetTrucks();
	std::vector<Load>& loads = company->GetLoads();

	Driver* selectedDriver = nullptr;
	Truck* selectedTruck = nullptr;
	Load* selectedLoad = nullptr;

	for (Driver& driver : drivers)
	{
		if (driver.GetDriverID() == driverID)
		{
			selectedDriver = &driver;
			break;
		}
	}

	for (Truck& truck : trucks)
	{
		if (truck.GetUnitNumber() == truckUnitNumber)
		{
			selectedTruck = &truck;
			break;
		}
	}

	for (Load& load : loads)
	{
		if (load.GetLoadNumber() == loadNumber)
		{
			selectedLoad = &load;
			break;
		}
	}

	if (selectedDriver == nullptr || selectedTruck == nullptr || selectedLoad == nullptr)
	{
		return false;
	}

	if (selectedDriver->GetStatus() != "Available" || selectedTruck->GetStatus() != "Available" || selectedLoad->GetStatus() != "Available")
	{
		return false;
	}

	selectedDriver->SetStatus("Dispatched");
	selectedTruck->SetStatus("Dispatched");

	selectedLoad->SetStatus("Assigned");
	selectedLoad->SetAssignedDriverID(driverID);
	selectedLoad->SetAssignedTruckUnitNumber(truckUnitNumber);

	return true;
}