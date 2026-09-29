#include "Driver.h"

Driver::Driver()
{
    driverID = 0;
    name = "";
    status = "Available";
}

int Driver::GetDriverID() const
{
    return driverID;
}

std::string Driver::GetName() const
{
    return name;
}

std::string Driver::GetStatus() const
{
    return status;
}

void Driver::SetDriverID(int newDriverID)
{
    driverID = newDriverID;
}

void Driver::SetName(const std::string& newName)
{
    name = newName;
}

void Driver::SetStatus(const std::string& newStatus)
{
    status = newStatus;
}