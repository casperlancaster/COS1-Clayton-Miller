#include "Truck.h"

Truck::Truck()
{
    unitNumber = 0;
    status = "Available";
    mileage = 0.0;
    currentLocation = "Unknown";

    lastServiceMileage = 0.0;
    serviceIntervalMiles = 10000.0;

    fuelTankCapacity = 200.0;
    fuelGallons = 200.0;
    milesPerGallon = 7.0;

    availableAtEpoch = 0;

    breakdownActive = false;
    breakdownType = "";

    breakdownPartsCost = 0.0;
    breakdownLaborCost = 0.0;
    breakdownServiceCallCharge = 0.0;
    breakdownRepairHours = 0.0;
}

int Truck::GetUnitNumber() const
{
    return unitNumber;
}

void Truck::SetUnitNumber(
    int newUnitNumber)
{
    unitNumber =
        newUnitNumber;
}

std::string Truck::GetStatus() const
{
    return status;
}

void Truck::SetStatus(
    const std::string& newStatus)
{
    status =
        newStatus;
}

double Truck::GetMileage() const
{
    return mileage;
}

void Truck::SetMileage(
    double newMileage)
{
    mileage =
        newMileage;
}

const std::string& Truck::GetCurrentLocation() const
{
    return currentLocation;
}

void Truck::SetCurrentLocation(
    const std::string& newCurrentLocation)
{
    if (newCurrentLocation.empty())
    {
        currentLocation =
            "Unknown";
    }
    else
    {
        currentLocation =
            newCurrentLocation;
    }
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
    return lastServiceMileage +
        serviceIntervalMiles;
}

double Truck::GetMilesUntilService() const
{
    double milesRemaining =
        GetNextServiceMileage() -
        mileage;

    if (milesRemaining < 0.0)
    {
        return 0.0;
    }

    return milesRemaining;
}

bool Truck::IsMaintenanceDue() const
{
    return mileage >=
        GetNextServiceMileage();
}

void Truck::InitializeMaintenanceSchedule()
{
    lastServiceMileage =
        mileage;
}

void Truck::CompleteMaintenance()
{
    lastServiceMileage =
        mileage;

    status =
        "Available";
}

void Truck::RestoreMaintenanceSchedule(
    double newLastServiceMileage,
    double newServiceIntervalMiles)
{
    lastServiceMileage =
        newLastServiceMileage;

    serviceIntervalMiles =
        newServiceIntervalMiles;
}

double Truck::GetFuelTankCapacity() const
{
    return fuelTankCapacity;
}

double Truck::GetFuelGallons() const
{
    return fuelGallons;
}

double Truck::GetMilesPerGallon() const
{
    return milesPerGallon;
}

double Truck::GetFuelPercentage() const
{
    if (fuelTankCapacity <= 0.0)
    {
        return 0.0;
    }

    return
        (fuelGallons /
            fuelTankCapacity) *
        100.0;
}

double Truck::GetFuelRangeMiles() const
{
    return fuelGallons *
        milesPerGallon;
}

double Truck::GetFuelNeededForDistance(
    double distanceMiles) const
{
    if (distanceMiles <= 0.0 ||
        milesPerGallon <= 0.0)
    {
        return 0.0;
    }

    return distanceMiles /
        milesPerGallon;
}

bool Truck::HasEnoughFuelForDistance(
    double distanceMiles) const
{
    double fuelNeeded =
        GetFuelNeededForDistance(
            distanceMiles
        );

    return fuelGallons +
        0.000001 >=
        fuelNeeded;
}

void Truck::InitializeFuelSystem(
    double newFuelTankCapacity,
    double newMilesPerGallon)
{
    if (newFuelTankCapacity <= 0.0 ||
        newMilesPerGallon <= 0.0)
    {
        return;
    }

    fuelTankCapacity =
        newFuelTankCapacity;

    milesPerGallon =
        newMilesPerGallon;

    fuelGallons =
        fuelTankCapacity;
}

void Truck::RestoreFuelState(
    double newFuelTankCapacity,
    double newFuelGallons,
    double newMilesPerGallon)
{
    if (newFuelTankCapacity <= 0.0 ||
        newMilesPerGallon <= 0.0)
    {
        return;
    }

    fuelTankCapacity =
        newFuelTankCapacity;

    milesPerGallon =
        newMilesPerGallon;

    if (newFuelGallons < 0.0)
    {
        fuelGallons = 0.0;
    }
    else if (newFuelGallons >
        fuelTankCapacity)
    {
        fuelGallons =
            fuelTankCapacity;
    }
    else
    {
        fuelGallons =
            newFuelGallons;
    }
}

double Truck::AddFuel(
    double gallons)
{
    if (gallons <= 0.0)
    {
        return 0.0;
    }

    double availableSpace =
        fuelTankCapacity -
        fuelGallons;

    if (availableSpace <= 0.0)
    {
        return 0.0;
    }

    double gallonsAdded =
        gallons;

    if (gallonsAdded >
        availableSpace)
    {
        gallonsAdded =
            availableSpace;
    }

    fuelGallons +=
        gallonsAdded;

    return gallonsAdded;
}

bool Truck::UseFuelForDistance(
    double distanceMiles)
{
    if (!HasEnoughFuelForDistance(
        distanceMiles))
    {
        return false;
    }

    double fuelUsed =
        GetFuelNeededForDistance(
            distanceMiles
        );

    fuelGallons -=
        fuelUsed;

    if (fuelGallons < 0.0)
    {
        fuelGallons = 0.0;
    }

    return true;
}

double Truck::GetMinimumArrivalFuelGallons()
const
{
    return fuelTankCapacity *
        0.50;
}

double Truck::GetFuelPurchaseNeededForTrip(
    double distanceMiles) const
{
    if (distanceMiles < 0.0 ||
        milesPerGallon <= 0.0 ||
        fuelTankCapacity <= 0.0)
    {
        return 0.0;
    }

    double tripFuelNeeded =
        GetFuelNeededForDistance(
            distanceMiles
        );

    double arrivalReserve =
        GetMinimumArrivalFuelGallons();

    double totalFuelRequired =
        tripFuelNeeded +
        arrivalReserve;

    double gallonsToPurchase =
        totalFuelRequired -
        fuelGallons;

    if (gallonsToPurchase < 0.0)
    {
        gallonsToPurchase = 0.0;
    }

    return gallonsToPurchase;
}

bool Truck::CompleteTripFuel(
    double distanceMiles,
    double& gallonsPurchased)
{
    gallonsPurchased = 0.0;

    if (distanceMiles < 0.0 ||
        milesPerGallon <= 0.0 ||
        fuelTankCapacity <= 0.0)
    {
        return false;
    }

    double tripFuelNeeded =
        GetFuelNeededForDistance(
            distanceMiles
        );

    gallonsPurchased =
        GetFuelPurchaseNeededForTrip(
            distanceMiles
        );

    double finalFuel =
        fuelGallons +
        gallonsPurchased -
        tripFuelNeeded;

    double minimumArrivalFuel =
        GetMinimumArrivalFuelGallons();

    if (finalFuel <
        minimumArrivalFuel)
    {
        finalFuel =
            minimumArrivalFuel;
    }

    if (finalFuel >
        fuelTankCapacity)
    {
        finalFuel =
            fuelTankCapacity;
    }

    fuelGallons =
        finalFuel;

    return true;
}

long long Truck::GetAvailableAtEpoch() const
{
    return availableAtEpoch;
}

void Truck::SetAvailableAtEpoch(
    long long newAvailableAtEpoch)
{
    if (newAvailableAtEpoch < 0)
    {
        availableAtEpoch = 0;
    }
    else
    {
        availableAtEpoch =
            newAvailableAtEpoch;
    }
}

bool Truck::HasBreakdown() const
{
    return breakdownActive;
}

std::string Truck::GetBreakdownType() const
{
    return breakdownType;
}

double Truck::GetBreakdownPartsCost() const
{
    return breakdownPartsCost;
}

double Truck::GetBreakdownLaborCost() const
{
    return breakdownLaborCost;
}

double Truck::GetBreakdownServiceCallCharge()
const
{
    return breakdownServiceCallCharge;
}

double Truck::GetBreakdownRepairHours() const
{
    return breakdownRepairHours;
}

double Truck::GetBreakdownTotalCost() const
{
    return breakdownPartsCost +
        breakdownLaborCost +
        breakdownServiceCallCharge;
}

void Truck::SetBreakdown(
    const std::string& newBreakdownType,
    double newPartsCost,
    double newLaborCost,
    double newServiceCallCharge,
    double newRepairHours)
{
    breakdownActive = true;

    breakdownType =
        newBreakdownType;

    breakdownPartsCost =
        newPartsCost;

    breakdownLaborCost =
        newLaborCost;

    breakdownServiceCallCharge =
        newServiceCallCharge;

    breakdownRepairHours =
        newRepairHours;
}

void Truck::RestoreBreakdownState(
    bool newBreakdownActive,
    const std::string& newBreakdownType,
    double newPartsCost,
    double newLaborCost,
    double newServiceCallCharge,
    double newRepairHours)
{
    breakdownActive =
        newBreakdownActive;

    breakdownType =
        newBreakdownType;

    breakdownPartsCost =
        newPartsCost;

    breakdownLaborCost =
        newLaborCost;

    breakdownServiceCallCharge =
        newServiceCallCharge;

    breakdownRepairHours =
        newRepairHours;

    if (breakdownActive)
    {
        status =
            "Breakdown";
    }
}

void Truck::ClearBreakdown()
{
    breakdownActive = false;

    breakdownType = "";

    breakdownPartsCost = 0.0;
    breakdownLaborCost = 0.0;
    breakdownServiceCallCharge = 0.0;
    breakdownRepairHours = 0.0;
}