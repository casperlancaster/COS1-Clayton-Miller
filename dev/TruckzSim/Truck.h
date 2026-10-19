#pragma once

#include <string>

class Truck
{
private:
    int unitNumber;
    std::string status;
    double mileage;

    double lastServiceMileage;
    double serviceIntervalMiles;

    double fuelTankCapacity;
    double fuelGallons;
    double milesPerGallon;

public:
    Truck();

    int GetUnitNumber() const;
    void SetUnitNumber(int newUnitNumber);

    std::string GetStatus() const;
    void SetStatus(const std::string& newStatus);

    double GetMileage() const;
    void SetMileage(double newMileage);

    double GetLastServiceMileage() const;
    double GetServiceIntervalMiles() const;
    double GetNextServiceMileage() const;
    double GetMilesUntilService() const;

    bool IsMaintenanceDue() const;

    void InitializeMaintenanceSchedule();
    void CompleteMaintenance();

    void RestoreMaintenanceSchedule(
        double newLastServiceMileage,
        double newServiceIntervalMiles
    );

    double GetFuelTankCapacity() const;
    double GetFuelGallons() const;
    double GetMilesPerGallon() const;

    double GetFuelPercentage() const;
    double GetFuelRangeMiles() const;
    double GetFuelNeededForDistance(double distanceMiles) const;

    bool HasEnoughFuelForDistance(double distanceMiles) const;

    void InitializeFuelSystem(
        double newFuelTankCapacity,
        double newMilesPerGallon
    );

    void RestoreFuelState(
        double newFuelTankCapacity,
        double newFuelGallons,
        double newMilesPerGallon
    );

    double AddFuel(double gallons);
    bool UseFuelForDistance(double distanceMiles);

    double GetMinimumArrivalFuelGallons() const;

    double GetFuelPurchaseNeededForTrip(
        double distanceMiles
    ) const;

    bool CompleteTripFuel(
        double distanceMiles,
        double& gallonsPurchased
    );
};