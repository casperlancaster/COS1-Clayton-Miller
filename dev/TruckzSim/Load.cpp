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
    distanceMiles = 0.0;
}

int Load::GetLoadNumber() const
{
    return loadNumber;
}

void Load::SetLoadNumber(int newLoadNumber)
{
    loadNumber = newLoadNumber;
}

std::string Load::GetOrigin() const
{
    return origin;
}

void Load::SetOrigin(const std::string& newOrigin)
{
    origin = newOrigin;
}

std::string Load::GetDestination() const
{
    return destination;
}

void Load::SetDestination(const std::string& newDestination)
{
    destination = newDestination;
}

double Load::GetPay() const
{
    return pay;
}

void Load::SetPay(double newPay)
{
    pay = newPay;
}

std::string Load::GetStatus() const
{
    return status;
}

void Load::SetStatus(const std::string& newStatus)
{
    status = newStatus;
}

int Load::GetAssignedDriverID() const
{
    return assignedDriverID;
}

void Load::SetAssignedDriverID(int newAssignedDriverID)
{
    assignedDriverID = newAssignedDriverID;
}

int Load::GetAssignedTruckUnitNumber() const
{
    return assignedTruckUnitNumber;
}

void Load::SetAssignedTruckUnitNumber(int newAssignedTruckUnitNumber)
{
    assignedTruckUnitNumber = newAssignedTruckUnitNumber;
}

double Load::GetDistanceMiles() const
{
    return distanceMiles;
}

void Load::SetDistanceMiles(double newDistanceMiles)
{
    distanceMiles = newDistanceMiles;
}
