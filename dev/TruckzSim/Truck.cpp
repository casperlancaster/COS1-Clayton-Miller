#include "Truck.h"

Truck::Truck()
{
    unitNumber = 0;
    status = "Available";
    mileage = 0.0;

    lastServiceMileage = 0.0;
    serviceIntervalMiles = 10000.0;
}

int Truck::GetUnitNumber() const
{
    return unitNumber;
}

void Truck::SetUnitNumber(int newUnitNumber)
{
    unitNumber = newUnitNumber;
}

std::string Truck::GetStatus() const
{
    return status;
}

void Truck::SetStatus(const std::string& newStatus)
{
    status = newStatus;
}

double Truck::GetMileage() const
{
    return mileage;
}

void Truck::SetMileage(double newMileage)
{
    mileage = newMileage;
}

double Truck::GetLastServiceMileage() const
{
    return lastServiceMileage;
}

double Truck::GetServiceIntervalMiles() const
{
    return serviceIntervalMiles;
}

double Truck::GetNextServiceMileage() const
{
    return lastServiceMileage + serviceIntervalMiles;
}

double Truck::GetMilesUntilService() const
{
    double milesRemaining = GetNextServiceMileage() - mileage;

    if (milesRemaining < 0.0)
    {
        return 0.0;
    }

    return milesRemaining;
}

bool Truck::IsMaintenanceDue() const
{
    return mileage >= GetNextServiceMileage();
}

void Truck::InitializeMaintenanceSchedule()
{
    lastServiceMileage = mileage;
}

void Truck::CompleteMaintenance()
{
    lastServiceMileage = mileage;
    status = "Available";
}
