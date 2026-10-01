#pragma once
#include <string>

class Truck
{
private:
	int unitNumber;
	std::string status;
	double mileage;

public:
	Truck();

	int GetUnitNumber() const;
	std::string GetStatus() const;
	double GetMileage() const;

	void SetUnitNumber(int newUnitNumber);
	void SetStatus(const std::string& newStatus);
	void SetMileage(double newMileage);
};