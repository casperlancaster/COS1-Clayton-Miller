#include "Load.h"

Load::Load()
{
    loadNumber = 0;
    origin = "";
    destination = "";
    pay = 0.0;
    status = "Available";
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