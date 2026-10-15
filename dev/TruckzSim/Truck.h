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
};
