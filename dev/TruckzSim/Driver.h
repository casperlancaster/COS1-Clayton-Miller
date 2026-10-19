#pragma once

#include <string>

class Driver
{
private:
    int driverID;
    std::string name;
    std::string status;

    double drivingHoursRemaining;
    double restHoursAccumulated;

    long long availableAtEpoch;

public:
    Driver();

    int GetDriverID() const;
    std::string GetName() const;
    std::string GetStatus() const;

    void SetDriverID(int newDriverID);
    void SetName(const std::string& newName);
    void SetStatus(const std::string& newStatus);

    double GetDrivingHoursRemaining() const;

    void SetDrivingHoursRemaining(
        double newHoursRemaining
    );

    double GetRestHoursAccumulated() const;

    void SetRestHoursAccumulated(
        double newRestHours
    );

    long long GetAvailableAtEpoch() const;

    void SetAvailableAtEpoch(
        long long newAvailableAtEpoch
    );

    double GetProjectedDrivingHoursAt(
        long long epochTime
    ) const;

    double GetProjectedRestHoursAt(
        long long epochTime
    ) const;

    void SetHOSState(
        double newDrivingHoursRemaining,
        double newRestHoursAccumulated,
        long long stateTimeEpoch
    );

    void ResetDrivingHours();
};