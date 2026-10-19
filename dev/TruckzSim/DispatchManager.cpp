#include "DispatchManager.h"

namespace
{
    constexpr double FUEL_PRICE_PER_GALLON =
        4.00;
}

DispatchManager::DispatchManager()
{
    company = nullptr;
}

DispatchManager::DispatchManager(
    Company* newCompany)
{
    company = newCompany;
}

Company* DispatchManager::GetCompany() const
{
    return company;
}

void DispatchManager::SetCompany(
    Company* newCompany)
{
    company = newCompany;
}

bool DispatchManager::HasCompany() const
{
    return company != nullptr;
}

bool DispatchManager::AssignLoad(
    int loadNumber,
    int driverID,
    int truckUnitNumber)
{
    if (company == nullptr)
    {
        return false;
    }

    std::vector<Driver>& drivers =
        company->GetDrivers();

    std::vector<Truck>& trucks =
        company->GetTrucks();

    std::vector<Load>& loads =
        company->GetLoads();

    Driver* selectedDriver = nullptr;
    Truck* selectedTruck = nullptr;
    Load* selectedLoad = nullptr;

    for (Driver& driver : drivers)
    {
        if (driver.GetDriverID() ==
            driverID)
        {
            selectedDriver =
                &driver;

            break;
        }
    }

    for (Truck& truck : trucks)
    {
        if (truck.GetUnitNumber() ==
            truckUnitNumber)
        {
            selectedTruck =
                &truck;

            break;
        }
    }

    for (Load& load : loads)
    {
        if (load.GetLoadNumber() ==
            loadNumber)
        {
            selectedLoad =
                &load;

            break;
        }
    }

    if (selectedDriver == nullptr ||
        selectedTruck == nullptr ||
        selectedLoad == nullptr)
    {
        return false;
    }

    if (selectedDriver->GetStatus() !=
        "Available" ||
        selectedTruck->GetStatus() !=
        "Available" ||
        selectedLoad->GetStatus() !=
        "Available")
    {
        return false;
    }

    selectedDriver->
        SetStatus("Dispatched");

    selectedTruck->
        SetStatus("Dispatched");

    selectedLoad->
        SetStatus("Assigned");

    selectedLoad->
        SetAssignedDriverID(
            driverID
        );

    selectedLoad->
        SetAssignedTruckUnitNumber(
            truckUnitNumber
        );

    return true;
}

bool DispatchManager::CompleteLoad(
    int loadNumber)
{
    if (company == nullptr)
    {
        return false;
    }

    std::vector<Driver>& drivers =
        company->GetDrivers();

    std::vector<Truck>& trucks =
        company->GetTrucks();

    std::vector<Load>& loads =
        company->GetLoads();

    Load* selectedLoad = nullptr;

    for (Load& load : loads)
    {
        if (load.GetLoadNumber() ==
            loadNumber)
        {
            selectedLoad =
                &load;

            break;
        }
    }

    if (selectedLoad == nullptr ||
        selectedLoad->GetStatus() !=
        "Assigned")
    {
        return false;
    }

    int assignedDriverID =
        selectedLoad->
        GetAssignedDriverID();

    int assignedTruckUnitNumber =
        selectedLoad->
        GetAssignedTruckUnitNumber();

    Driver* selectedDriver = nullptr;
    Truck* selectedTruck = nullptr;

    for (Driver& driver : drivers)
    {
        if (driver.GetDriverID() ==
            assignedDriverID)
        {
            selectedDriver =
                &driver;

            break;
        }
    }

    for (Truck& truck : trucks)
    {
        if (truck.GetUnitNumber() ==
            assignedTruckUnitNumber)
        {
            selectedTruck =
                &truck;

            break;
        }
    }

    if (selectedDriver == nullptr ||
        selectedTruck == nullptr)
    {
        return false;
    }

    if (selectedDriver->GetStatus() !=
        "Dispatched" ||
        selectedTruck->GetStatus() !=
        "Dispatched")
    {
        return false;
    }

    double gallonsPurchased = 0.0;

    if (!selectedTruck->
        CompleteTripFuel(
            selectedLoad->
            GetDistanceMiles(),
            gallonsPurchased))
    {
        return false;
    }

    double fuelExpense =
        gallonsPurchased *
        FUEL_PRICE_PER_GALLON;

    if (fuelExpense > 0.0)
    {
        company->AddExpense(
            fuelExpense
        );
    }

    company->AddRevenue(
        selectedLoad->GetPay()
    );

    selectedTruck->SetMileage(
        selectedTruck->GetMileage() +
        selectedLoad->GetDistanceMiles()
    );

    selectedLoad->
        SetStatus("Completed");

    selectedDriver->
        SetStatus("Available");

    selectedTruck->
        SetStatus("Available");

    return true;
}