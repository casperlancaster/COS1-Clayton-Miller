#pragma once

#include <string>

class Load
{
private:
    int loadNumber;

    std::string origin;
    std::string destination;

    double pay;
    std::string status;

    int assignedDriverID;
    int assignedSecondDriverID;
    int assignedTruckUnitNumber;

    double distanceMiles;

    long long deliveryDeadlineEpoch;
    long long dispatchStartEpoch;
    long long estimatedDeliveryEpoch;

public:
    Load();

    int GetLoadNumber() const;
    void SetLoadNumber(int newLoadNumber);

    std::string GetOrigin() const;
    void SetOrigin(
        const std::string& newOrigin
    );

    std::string GetDestination() const;
    void SetDestination(
        const std::string& newDestination
    );

    double GetPay() const;
    void SetPay(double newPay);

    std::string GetStatus() const;
    void SetStatus(
        const std::string& newStatus
    );

    int GetAssignedDriverID() const;
    void SetAssignedDriverID(
        int newAssignedDriverID
    );

    int GetAssignedSecondDriverID() const;
    void SetAssignedSecondDriverID(
        int newAssignedSecondDriverID
    );

    int GetAssignedTruckUnitNumber() const;
    void SetAssignedTruckUnitNumber(
        int newAssignedTruckUnitNumber
    );

    double GetDistanceMiles() const;
    void SetDistanceMiles(
        double newDistanceMiles
    );

    long long GetDeliveryDeadlineEpoch()
        const;

    void SetDeliveryDeadlineEpoch(
        long long newDeadlineEpoch
    );

    long long GetDispatchStartEpoch()
        const;

    void SetDispatchStartEpoch(
        long long newDispatchStartEpoch
    );

    long long GetEstimatedDeliveryEpoch()
        const;

    void SetEstimatedDeliveryEpoch(
        long long newEstimatedDeliveryEpoch
    );

    bool IsTeamLoad() const;
};