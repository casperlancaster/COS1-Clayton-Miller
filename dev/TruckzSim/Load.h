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
    int assignedTruckUnitNumber;
    double distanceMiles;

public:
    Load();

    int GetLoadNumber() const;
    void SetLoadNumber(int newLoadNumber);

    std::string GetOrigin() const;
    void SetOrigin(const std::string& newOrigin);

    std::string GetDestination() const;
    void SetDestination(const std::string& newDestination);

    double GetPay() const;
    void SetPay(double newPay);

    std::string GetStatus() const;
    void SetStatus(const std::string& newStatus);

    int GetAssignedDriverID() const;
    void SetAssignedDriverID(int newAssignedDriverID);

    int GetAssignedTruckUnitNumber() const;
    void SetAssignedTruckUnitNumber(int newAssignedTruckUnitNumber);

    double GetDistanceMiles() const;
    void SetDistanceMiles(double newDistanceMiles);
};
