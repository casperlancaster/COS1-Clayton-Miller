#include "Driver.h"

namespace
{
    constexpr double MAX_DRIVING_HOURS =
        11.0;

    constexpr double REQUIRED_REST_HOURS =
        10.0;

    constexpr double EPSILON =
        0.000001;
}

Driver::Driver()
{
    driverID = 0;
    name = "";
    status = "Available";

    drivingHoursRemaining =
        MAX_DRIVING_HOURS;

    restHoursAccumulated = 0.0;

    availableAtEpoch = 0;
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

void Driver::SetDriverID(
    int newDriverID)
{
    driverID = newDriverID;
}

void Driver::SetName(
    const std::string& newName)
{
    name = newName;
}

void Driver::SetStatus(
    const std::string& newStatus)
{
    status = newStatus;
}

double Driver::GetDrivingHoursRemaining()
const
{
    return drivingHoursRemaining;
}

void Driver::SetDrivingHoursRemaining(
    double newHoursRemaining)
{
    if (newHoursRemaining < 0.0)
    {
        drivingHoursRemaining = 0.0;
    }
    else if (newHoursRemaining >
        MAX_DRIVING_HOURS)
    {
        drivingHoursRemaining =
            MAX_DRIVING_HOURS;
    }
    else
    {
        drivingHoursRemaining =
            newHoursRemaining;
    }
}

double Driver::GetRestHoursAccumulated()
const
{
    return restHoursAccumulated;
}

void Driver::SetRestHoursAccumulated(
    double newRestHours)
{
    if (newRestHours < 0.0)
    {
        restHoursAccumulated = 0.0;
    }
    else if (newRestHours >
        REQUIRED_REST_HOURS)
    {
        restHoursAccumulated =
            REQUIRED_REST_HOURS;
    }
    else
    {
        restHoursAccumulated =
            newRestHours;
    }
}

long long Driver::GetAvailableAtEpoch()
const
{
    return availableAtEpoch;
}

void Driver::SetAvailableAtEpoch(
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

double Driver::GetProjectedRestHoursAt(
    long long epochTime) const
{
    double projectedRest =
        restHoursAccumulated;

    if (availableAtEpoch <= 0 ||
        epochTime <= availableAtEpoch)
    {
        return projectedRest;
    }

    double elapsedHours =
        (epochTime -
            availableAtEpoch) /
        3600.0;

    projectedRest +=
        elapsedHours;

    if (projectedRest >
        REQUIRED_REST_HOURS)
    {
        projectedRest =
            REQUIRED_REST_HOURS;
    }

    return projectedRest;
}

double Driver::GetProjectedDrivingHoursAt(
    long long epochTime) const
{
    double projectedRest =
        GetProjectedRestHoursAt(
            epochTime
        );

    if (projectedRest >=
        REQUIRED_REST_HOURS -
        EPSILON)
    {
        return MAX_DRIVING_HOURS;
    }

    return drivingHoursRemaining;
}

void Driver::SetHOSState(
    double newDrivingHoursRemaining,
    double newRestHoursAccumulated,
    long long stateTimeEpoch)
{
    SetDrivingHoursRemaining(
        newDrivingHoursRemaining
    );

    SetRestHoursAccumulated(
        newRestHoursAccumulated
    );

    SetAvailableAtEpoch(
        stateTimeEpoch
    );
}

void Driver::ResetDrivingHours()
{
    drivingHoursRemaining =
        MAX_DRIVING_HOURS;

    restHoursAccumulated =
        0.0;

    availableAtEpoch = 0;
}