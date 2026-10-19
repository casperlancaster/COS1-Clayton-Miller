#pragma once

#include <string>

struct HOSPlan
{
    bool valid;
    bool teamOperation;
    bool meetsDeadline;

    double drivingHoursRequired;
    int soloRestPeriods;
    double stoppedRestHours;
    double totalTripHours;

    double primaryFinalHours;
    double primaryFinalRestHours;

    double secondFinalHours;
    double secondFinalRestHours;

    long long estimatedDeliveryEpoch;
};

class HOSManager
{
public:
    static double GetMaxDrivingHours();
    static double GetRequiredRestHours();
    static double GetAveragePlanningSpeedMph();

    static long long GetCurrentEpoch();

    static void SetDateFormat(
        const std::string& newDateFormat
    );

    static void SetTimeFormat(
        const std::string& newTimeFormat
    );

    static std::string GetDateFormat();
    static std::string GetTimeFormat();

    static std::string GetDateInputExample();
    static std::string GetTimeInputExample();

    static bool ParseDateTime(
        const std::string& dateText,
        const std::string& timeText,
        long long& epochResult
    );

    static std::string FormatDateTime(
        long long epochTime
    );

    static HOSPlan CreateSoloPlan(
        double driverHoursRemaining,
        double driverRestHours,
        double distanceMiles,
        long long tripStartEpoch,
        long long deliveryDeadlineEpoch
    );

    static HOSPlan CreateTeamPlan(
        double primaryHoursRemaining,
        double primaryRestHours,
        double secondHoursRemaining,
        double secondRestHours,
        double distanceMiles,
        long long tripStartEpoch,
        long long deliveryDeadlineEpoch
    );
};
