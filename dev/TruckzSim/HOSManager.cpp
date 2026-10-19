#include "HOSManager.h"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace
{
    constexpr double MAX_DRIVING_HOURS = 11.0;
    constexpr double REQUIRED_REST_HOURS = 10.0;
    constexpr double AVERAGE_SPEED_MPH = 55.0;
    constexpr double EPSILON = 0.000001;

    double ClampDrivingHours(double hours)
    {
        if (hours < 0.0)
        {
            return 0.0;
        }

        if (hours > MAX_DRIVING_HOURS)
        {
            return MAX_DRIVING_HOURS;
        }

        return hours;
    }

    double ClampRestHours(double hours)
    {
        if (hours < 0.0)
        {
            return 0.0;
        }

        if (hours > REQUIRED_REST_HOURS)
        {
            return REQUIRED_REST_HOURS;
        }

        return hours;
    }

    bool IsDigit(char character)
    {
        return character >= '0' &&
            character <= '9';
    }
}

double HOSManager::GetMaxDrivingHours()
{
    return MAX_DRIVING_HOURS;
}

double HOSManager::GetRequiredRestHours()
{
    return REQUIRED_REST_HOURS;
}

double HOSManager::GetAveragePlanningSpeedMph()
{
    return AVERAGE_SPEED_MPH;
}

long long HOSManager::GetCurrentEpoch()
{
    return static_cast<long long>(
        std::time(nullptr)
        );
}

bool HOSManager::ParseDateTime(
    const std::string& dateText,
    const std::string& timeText,
    long long& epochResult)
{
    epochResult = 0;

    if (dateText.size() != 10 ||
        timeText.size() != 5)
    {
        return false;
    }

    if (dateText[4] != '-' ||
        dateText[7] != '-' ||
        timeText[2] != ':')
    {
        return false;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            continue;
        }

        if (!IsDigit(dateText[i]))
        {
            return false;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        if (i == 2)
        {
            continue;
        }

        if (!IsDigit(timeText[i]))
        {
            return false;
        }
    }

    int year =
        std::stoi(dateText.substr(0, 4));

    int month =
        std::stoi(dateText.substr(5, 2));

    int day =
        std::stoi(dateText.substr(8, 2));

    int hour =
        std::stoi(timeText.substr(0, 2));

    int minute =
        std::stoi(timeText.substr(3, 2));

    if (month < 1 ||
        month > 12 ||
        day < 1 ||
        day > 31 ||
        hour < 0 ||
        hour > 23 ||
        minute < 0 ||
        minute > 59)
    {
        return false;
    }

    std::tm dateTime = {};

    dateTime.tm_year = year - 1900;
    dateTime.tm_mon = month - 1;
    dateTime.tm_mday = day;
    dateTime.tm_hour = hour;
    dateTime.tm_min = minute;
    dateTime.tm_sec = 0;
    dateTime.tm_isdst = -1;

    std::time_t converted =
        std::mktime(&dateTime);

    if (converted ==
        static_cast<std::time_t>(-1))
    {
        return false;
    }

    if (dateTime.tm_year != year - 1900 ||
        dateTime.tm_mon != month - 1 ||
        dateTime.tm_mday != day ||
        dateTime.tm_hour != hour ||
        dateTime.tm_min != minute)
    {
        return false;
    }

    epochResult =
        static_cast<long long>(
            converted
            );

    return true;
}

std::string HOSManager::FormatDateTime(
    long long epochTime)
{
    if (epochTime <= 0)
    {
        return "Available Now";
    }

    std::time_t timeValue =
        static_cast<std::time_t>(
            epochTime
            );

    std::tm localTime = {};

#ifdef _WIN32
    if (localtime_s(
        &localTime,
        &timeValue) != 0)
    {
        return "Invalid date/time";
    }
#else
    if (localtime_r(
        &timeValue,
        &localTime) == nullptr)
    {
        return "Invalid date/time";
    }
#endif

    char buffer[80];

    if (std::strftime(
        buffer,
        sizeof(buffer),
        "%b %d, %Y - %I:%M %p",
        &localTime) == 0)
    {
        return "Invalid date/time";
    }

    return std::string(buffer);
}

HOSPlan HOSManager::CreateSoloPlan(
    double driverHoursRemaining,
    double driverRestHours,
    double distanceMiles,
    long long tripStartEpoch,
    long long deliveryDeadlineEpoch)
{
    HOSPlan plan = {};

    plan.valid = false;
    plan.teamOperation = false;
    plan.meetsDeadline = false;

    if (distanceMiles < 0.0 ||
        AVERAGE_SPEED_MPH <= 0.0)
    {
        return plan;
    }

    if (tripStartEpoch <= 0)
    {
        tripStartEpoch =
            GetCurrentEpoch();
    }

    double drivingHours =
        distanceMiles /
        AVERAGE_SPEED_MPH;

    double remainingHours =
        ClampDrivingHours(
            driverHoursRemaining
        );

    double restProgress =
        ClampRestHours(
            driverRestHours
        );

    if (restProgress >=
        REQUIRED_REST_HOURS -
        EPSILON)
    {
        remainingHours =
            MAX_DRIVING_HOURS;

        restProgress =
            REQUIRED_REST_HOURS;
    }

    double drivingLeft =
        drivingHours;

    double elapsedHours = 0.0;
    double stoppedRestHours = 0.0;
    int restPeriods = 0;

    while (drivingLeft > EPSILON)
    {
        if (remainingHours <= EPSILON)
        {
            double restNeeded =
                REQUIRED_REST_HOURS -
                restProgress;

            if (restNeeded < 0.0)
            {
                restNeeded = 0.0;
            }

            elapsedHours +=
                restNeeded;

            stoppedRestHours +=
                restNeeded;

            restPeriods++;

            remainingHours =
                MAX_DRIVING_HOURS;

            restProgress =
                REQUIRED_REST_HOURS;
        }

        restProgress = 0.0;

        double drivingChunk =
            std::min(
                remainingHours,
                drivingLeft
            );

        remainingHours -=
            drivingChunk;

        drivingLeft -=
            drivingChunk;

        elapsedHours +=
            drivingChunk;

        if (remainingHours <
            EPSILON)
        {
            remainingHours = 0.0;
        }

        if (drivingLeft <
            EPSILON)
        {
            drivingLeft = 0.0;
        }
    }

    long long tripSeconds =
        static_cast<long long>(
            std::llround(
                elapsedHours *
                3600.0
            )
            );

    plan.valid = true;

    plan.drivingHoursRequired =
        drivingHours;

    plan.soloRestPeriods =
        restPeriods;

    plan.stoppedRestHours =
        stoppedRestHours;

    plan.totalTripHours =
        elapsedHours;

    plan.primaryFinalHours =
        remainingHours;

    plan.primaryFinalRestHours =
        0.0;

    plan.secondFinalHours =
        0.0;

    plan.secondFinalRestHours =
        0.0;

    plan.estimatedDeliveryEpoch =
        tripStartEpoch +
        tripSeconds;

    if (deliveryDeadlineEpoch <= 0)
    {
        plan.meetsDeadline = true;
    }
    else
    {
        plan.meetsDeadline =
            plan.estimatedDeliveryEpoch <=
            deliveryDeadlineEpoch;
    }

    return plan;
}

HOSPlan HOSManager::CreateTeamPlan(
    double primaryHoursRemaining,
    double primaryRestHours,
    double secondHoursRemaining,
    double secondRestHours,
    double distanceMiles,
    long long tripStartEpoch,
    long long deliveryDeadlineEpoch)
{
    HOSPlan plan = {};

    plan.valid = false;
    plan.teamOperation = true;
    plan.meetsDeadline = false;

    if (distanceMiles < 0.0 ||
        AVERAGE_SPEED_MPH <= 0.0)
    {
        return plan;
    }

    if (tripStartEpoch <= 0)
    {
        tripStartEpoch =
            GetCurrentEpoch();
    }

    double drivingHours =
        distanceMiles /
        AVERAGE_SPEED_MPH;

    double remainingHours[2] =
    {
        ClampDrivingHours(
            primaryHoursRemaining
        ),

        ClampDrivingHours(
            secondHoursRemaining
        )
    };

    double restProgress[2] =
    {
        ClampRestHours(
            primaryRestHours
        ),

        ClampRestHours(
            secondRestHours
        )
    };

    for (int i = 0; i < 2; i++)
    {
        if (restProgress[i] >=
            REQUIRED_REST_HOURS -
            EPSILON)
        {
            remainingHours[i] =
                MAX_DRIVING_HOURS;

            restProgress[i] =
                REQUIRED_REST_HOURS;
        }
    }

    double drivingLeft =
        drivingHours;

    double elapsedHours = 0.0;
    double stoppedRestHours = 0.0;

    int activeDriver = 0;

    while (drivingLeft > EPSILON)
    {
        for (int i = 0; i < 2; i++)
        {
            if (restProgress[i] >=
                REQUIRED_REST_HOURS -
                EPSILON)
            {
                remainingHours[i] =
                    MAX_DRIVING_HOURS;

                restProgress[i] =
                    REQUIRED_REST_HOURS;
            }
        }

        if (remainingHours[
            activeDriver] <= EPSILON)
        {
            int otherDriver =
                1 - activeDriver;

            if (remainingHours[
                otherDriver] > EPSILON)
            {
                activeDriver =
                    otherDriver;

                restProgress[
                    activeDriver] = 0.0;

                continue;
            }

            double primaryRestNeeded =
                std::max(
                    0.0,
                    REQUIRED_REST_HOURS -
                    restProgress[0]
                );

            double secondRestNeeded =
                std::max(
                    0.0,
                    REQUIRED_REST_HOURS -
                    restProgress[1]
                );

            double waitTime =
                std::min(
                    primaryRestNeeded,
                    secondRestNeeded
                );

            if (waitTime <= EPSILON)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (restProgress[i] >=
                        REQUIRED_REST_HOURS -
                        EPSILON)
                    {
                        remainingHours[i] =
                            MAX_DRIVING_HOURS;

                        restProgress[i] =
                            REQUIRED_REST_HOURS;
                    }
                }
            }
            else
            {
                elapsedHours +=
                    waitTime;

                stoppedRestHours +=
                    waitTime;

                restProgress[0] +=
                    waitTime;

                restProgress[1] +=
                    waitTime;

                for (int i = 0; i < 2; i++)
                {
                    if (restProgress[i] >=
                        REQUIRED_REST_HOURS -
                        EPSILON)
                    {
                        remainingHours[i] =
                            MAX_DRIVING_HOURS;

                        restProgress[i] =
                            REQUIRED_REST_HOURS;
                    }
                }
            }

            if (remainingHours[0] >
                EPSILON)
            {
                activeDriver = 0;
            }
            else if (remainingHours[1] >
                EPSILON)
            {
                activeDriver = 1;
            }
            else
            {
                return plan;
            }

            restProgress[
                activeDriver] = 0.0;

            continue;
        }

        restProgress[
            activeDriver] = 0.0;

        int restingDriver =
            1 - activeDriver;

        double drivingChunk =
            std::min(
                remainingHours[
                    activeDriver],
                    drivingLeft
                    );

        remainingHours[
            activeDriver] -=
            drivingChunk;

            drivingLeft -=
                drivingChunk;

            elapsedHours +=
                drivingChunk;

            restProgress[
                restingDriver] +=
                drivingChunk;

                if (restProgress[
                    restingDriver] >=
                    REQUIRED_REST_HOURS -
                        EPSILON)
                {
                    remainingHours[
                        restingDriver] =
                        MAX_DRIVING_HOURS;

                        restProgress[
                            restingDriver] =
                            REQUIRED_REST_HOURS;
                }

                if (remainingHours[
                    activeDriver] <
                    EPSILON)
                {
                    remainingHours[
                        activeDriver] = 0.0;
                }

                if (drivingLeft <
                    EPSILON)
                {
                    drivingLeft = 0.0;
                }

                if (drivingLeft > EPSILON &&
                    remainingHours[
                        activeDriver] <=
                    EPSILON &&
                            remainingHours[
                                restingDriver] >
                            EPSILON)
                {
                    activeDriver =
                        restingDriver;

                    restProgress[
                        activeDriver] = 0.0;
                }
    }

    long long tripSeconds =
        static_cast<long long>(
            std::llround(
                elapsedHours *
                3600.0
            )
            );

    plan.valid = true;

    plan.drivingHoursRequired =
        drivingHours;

    plan.soloRestPeriods = 0;

    plan.stoppedRestHours =
        stoppedRestHours;

    plan.totalTripHours =
        elapsedHours;

    plan.primaryFinalHours =
        remainingHours[0];

    plan.primaryFinalRestHours =
        ClampRestHours(
            restProgress[0]
        );

    plan.secondFinalHours =
        remainingHours[1];

    plan.secondFinalRestHours =
        ClampRestHours(
            restProgress[1]
        );

    plan.estimatedDeliveryEpoch =
        tripStartEpoch +
        tripSeconds;

    if (deliveryDeadlineEpoch <= 0)
    {
        plan.meetsDeadline = true;
    }
    else
    {
        plan.meetsDeadline =
            plan.estimatedDeliveryEpoch <=
            deliveryDeadlineEpoch;
    }

    return plan;
}