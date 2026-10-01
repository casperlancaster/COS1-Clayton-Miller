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

public:
    Load();

    int GetLoadNumber() const;
    std::string GetOrigin() const;
    std::string GetDestination() const;
    double GetPay() const;
    std::string GetStatus() const;
    int GetAssignedDriverID() const;
    int GetAssignedTruckUnitNumber() const;

    void SetLoadNumber(int newLoadNumber);
    void SetOrigin(const std::string& newOrigin);
    void SetDestination(const std::string& newDestination);
    void SetPay(double newPay);
    void SetStatus(const std::string& newStatus);
    void SetAssignedDriverID(int newDriverID);
    void SetAssignedTruckUnitNumber(int newTruckUnitNumber);
};