#include "Truck.h"

Truck::Truck()
{
    unitNumber = 0;
    status = "Available";
    mileage = 0.0;
}

int Truck::GetUnitNumber() const
{
    return unitNumber;
}

std::string Truck::GetStatus() const
{
    return status;
}

double Truck::GetMileage() const
{
    return mileage;
}

void Truck::SetUnitNumber(int newUnitNumber)
{
    unitNumber = newUnitNumber;
}

void Truck::SetStatus(const std::string& newStatus)
{
    status = newStatus;
}

void Truck::SetMileage(double newMileage)
{
    mileage = newMileage;
}