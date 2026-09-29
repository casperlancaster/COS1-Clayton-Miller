#pragma once
#include <string>

class Driver
{
private:
    int driverID;
    std::string name;
    std::string status;

public:
    Driver();

    int GetDriverID() const;
    std::string GetName() const;
    std::string GetStatus() const;

    void SetDriverID(int newDriverID);
    void SetName(const std::string& newName);
    void SetStatus(const std::string& newStatus);
};