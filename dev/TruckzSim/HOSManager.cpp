#include "HOSManager.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    constexpr double MAX_DRIVING_HOURS = 11.0;
    constexpr double REQUIRED_REST_HOURS = 10.0;
    constexpr double AVERAGE_SPEED_MPH = 55.0;
    constexpr double EPSILON = 0.000001;

    std::string currentDateFormat = "MM/DD/YYYY";
    std::string currentTimeFormat = "12-hour";

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

    std::string Trim(const std::string& input)
    {
        std::size_t start = 0;

        while (start < input.size() &&
            std::isspace(
                static_cast<unsigned char>(
                    input[start])))
        {
            start++;
        }

        std::size_t end = input.size();

        while (end > start &&
            std::isspace(
                static_cast<unsigned char>(
                    input[end - 1])))
        {
            end--;
        }

        return input.substr(
            start,
            end - start
        );
    }

    std::string ToUpper(std::string text)
    {
        for (char& character : text)
        {
            character = static_cast<char>(
                std::toupper(
                    static_cast<unsigned char>(
                        character))
                );
        }

        return text;
    }

    bool ContainsOnlyDigits(
        const std::string& text)
    {
        if (text.empty())
        {
            return false;
        }

        for (char character : text)
        {
            if (!std::isdigit(
                static_cast<unsigned char>(
                    character)))
            {
                return false;
            }
        }

        return true;
    }

    bool ParseDateText(
        const std::string& dateText,
        int& year,
        int& month,
        int& day)
    {
        std::string cleaned = Trim(dateText);

        if (cleaned.empty())
        {
            return false;
        }

        std::string digitsOnly;

        for (char character : cleaned)
        {
            if (std::isdigit(
                static_cast<unsigned char>(
                    character)))
            {
                digitsOnly += character;
            }
        }

        if (digitsOnly.size() != 8)
        {
            return false;
        }

        bool hadSeparator =
            cleaned.find('/') != std::string::npos ||
            cleaned.find('-') != std::string::npos ||
            cleaned.find(' ') != std::string::npos;

        if (hadSeparator)
        {
            std::string normalized = cleaned;

            for (char& character : normalized)
            {
                if (character == '/' ||
                    character == '-')
                {
                    character = ' ';
                }
            }

            std::istringstream stream(normalized);
            std::vector<std::string> parts;
            std::string part;

            while (stream >> part)
            {
                parts.push_back(part);
            }

            if (parts.size() != 3 ||
                !ContainsOnlyDigits(parts[0]) ||
                !ContainsOnlyDigits(parts[1]) ||
                !ContainsOnlyDigits(parts[2]))
            {
                return false;
            }

            if (parts[0].size() == 4)
            {
                year = std::stoi(parts[0]);
                month = std::stoi(parts[1]);
                day = std::stoi(parts[2]);
            }
            else if (parts[2].size() == 4)
            {
                month = std::stoi(parts[0]);
                day = std::stoi(parts[1]);
                year = std::stoi(parts[2]);
            }
            else
            {
                return false;
            }
        }
        else
        {
            int firstFour =
                std::stoi(
                    digitsOnly.substr(0, 4)
                );

            if (firstFour >= 1900 &&
                firstFour <= 9999)
            {
                year = firstFour;
                month = std::stoi(
                    digitsOnly.substr(4, 2)
                );
                day = std::stoi(
                    digitsOnly.substr(6, 2)
                );
            }
            else
            {
                month = std::stoi(
                    digitsOnly.substr(0, 2)
                );
                day = std::stoi(
                    digitsOnly.substr(2, 2)
                );
                year = std::stoi(
                    digitsOnly.substr(4, 4)
                );
            }
        }

        return true;
    }

    bool ParseTimeText(
        const std::string& timeText,
        int& hour,
        int& minute)
    {
        std::string cleaned =
            ToUpper(
                Trim(timeText)
            );

        if (cleaned.empty())
        {
            return false;
        }

        bool hasAM = false;
        bool hasPM = false;

        std::string compact;

        for (char character : cleaned)
        {
            if (!std::isspace(
                static_cast<unsigned char>(
                    character)))
            {
                compact += character;
            }
        }

        if (compact.size() >= 2)
        {
            std::string suffix =
                compact.substr(
                    compact.size() - 2
                );

            if (suffix == "AM")
            {
                hasAM = true;
                compact.erase(
                    compact.size() - 2
                );
            }
            else if (suffix == "PM")
            {
                hasPM = true;
                compact.erase(
                    compact.size() - 2
                );
            }
        }

        if (compact.empty())
        {
            return false;
        }

        std::size_t colonPosition =
            compact.find(':');

        if (colonPosition != std::string::npos)
        {
            if (compact.find(
                ':',
                colonPosition + 1) !=
                std::string::npos)
            {
                return false;
            }

            std::string hourText =
                compact.substr(
                    0,
                    colonPosition
                );

            std::string minuteText =
                compact.substr(
                    colonPosition + 1
                );

            if (!ContainsOnlyDigits(hourText) ||
                !ContainsOnlyDigits(minuteText) ||
                minuteText.size() != 2)
            {
                return false;
            }

            hour = std::stoi(hourText);
            minute = std::stoi(minuteText);
        }
        else
        {
            if (!ContainsOnlyDigits(compact) ||
                compact.size() > 4)
            {
                return false;
            }

            if (compact.size() <= 2)
            {
                hour = std::stoi(compact);
                minute = 0;
            }
            else if (compact.size() == 3)
            {
                hour = std::stoi(
                    compact.substr(0, 1)
                );
                minute = std::stoi(
                    compact.substr(1, 2)
                );
            }
            else
            {
                hour = std::stoi(
                    compact.substr(0, 2)
                );
                minute = std::stoi(
                    compact.substr(2, 2)
                );
            }
        }

        if (minute < 0 ||
            minute > 59)
        {
            return false;
        }

        if (hasAM || hasPM)
        {
            if (hour < 1 ||
                hour > 12)
            {
                return false;
            }

            if (hasAM)
            {
                if (hour == 12)
                {
                    hour = 0;
                }
            }
            else
            {
                if (hour != 12)
                {
                    hour += 12;
                }
            }
        }
        else
        {
            if (hour < 0 ||
                hour > 23)
            {
                return false;
            }
        }

        return true;
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

void HOSManager::SetDateFormat(
    const std::string& newDateFormat)
{
    if (newDateFormat == "MM/DD/YYYY" ||
        newDateFormat == "YYYY-MM-DD")
    {
        currentDateFormat =
            newDateFormat;
    }
}

void HOSManager::SetTimeFormat(
    const std::string& newTimeFormat)
{
    if (newTimeFormat == "12-hour" ||
        newTimeFormat == "24-hour")
    {
        currentTimeFormat =
            newTimeFormat;
    }
}

std::string HOSManager::GetDateFormat()
{
    return currentDateFormat;
}

std::string HOSManager::GetTimeFormat()
{
    return currentTimeFormat;
}

std::string HOSManager::GetDateInputExample()
{
    if (currentDateFormat == "YYYY-MM-DD")
    {
        return "2026-10-22";
    }

    return "10/22/2026";
}

std::string HOSManager::GetTimeInputExample()
{
    if (currentTimeFormat == "24-hour")
    {
        return "20:30 or 2030";
    }

    return "8:30 PM or 830 PM";
}

bool HOSManager::ParseDateTime(
    const std::string& dateText,
    const std::string& timeText,
    long long& epochResult)
{
    epochResult = 0;

    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;

    if (!ParseDateText(
        dateText,
        year,
        month,
        day))
    {
        return false;
    }

    if (!ParseTimeText(
        timeText,
        hour,
        minute))
    {
        return false;
    }

    if (year < 1900 ||
        month < 1 ||
        month > 12 ||
        day < 1 ||
        day > 31)
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

    char dateBuffer[32];
    char timeBuffer[32];

    const char* datePattern =
        currentDateFormat == "YYYY-MM-DD"
        ? "%Y-%m-%d"
        : "%m/%d/%Y";

    const char* timePattern =
        currentTimeFormat == "24-hour"
        ? "%H:%M"
        : "%I:%M %p";

    if (std::strftime(
        dateBuffer,
        sizeof(dateBuffer),
        datePattern,
        &localTime) == 0)
    {
        return "Invalid date/time";
    }

    if (std::strftime(
        timeBuffer,
        sizeof(timeBuffer),
        timePattern,
        &localTime) == 0)
    {
        return "Invalid date/time";
    }

    std::string timeString(
        timeBuffer
    );

    if (currentTimeFormat == "12-hour" &&
        !timeString.empty() &&
        timeString[0] == '0')
    {
        timeString.erase(0, 1);
    }

    return std::string(dateBuffer) +
        " - " +
        timeString;
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