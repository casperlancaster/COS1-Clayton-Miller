#include "Load.h"

Load::Load()
{
    loadNumber = 0;
    origin = "";
    destination = "";
    pay = 0.0;
    status = "Available";
    assignedDriverID = 0;
    assignedTruckUnitNumber = 0;
}

int Load::GetLoadNumber() const
{
    return loadNumber;
}

std::string Load::GetOrigin() const
{
    return origin;
}

std::string Load::GetDestination() const
{
    return destination;
}

double Load::GetPay() const
{
    return pay;
}

std::string Load::GetStatus() const
{
    return status;
}

int Load::GetAssignedDriverID() const
{
    return assignedDriverID;
}

int Load::GetAssignedTruckUnitNumber() const
{
    return assignedTruckUnitNumber;
}

void Load::SetLoadNumber(int newLoadNumber)
{
    loadNumber = newLoadNumber;
}

void Load::SetOrigin(const std::string& newOrigin)
{
    origin = newOrigin;
}

void Load::SetDestination(const std::string& newDestination)
{
    destination = newDestination;
}

void Load::SetPay(double newPay)
{
    pay = newPay;
}

void Load::SetStatus(const std::string& newStatus)
{
    status = newStatus;
}

void Load::SetAssignedDriverID(int newDriverID)
{
    assignedDriverID = newDriverID;
}

void Load::SetAssignedTruckUnitNumber(int newTruckUnitNumber)
{
    assignedTruckUnitNumber = newTruckUnitNumber;
}
