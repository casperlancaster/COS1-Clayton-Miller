#pragma once

#include "Company.h"

class DispatchManager
{
private:
	Company* company;

public:
	DispatchManager();
	DispatchManager(Company* newCompany);

	Company* GetCompany() const;
	void SetCompany(Company* newCompany);

	bool HasCompany() const;
	bool AssignLoad(int loadNumber, int driverID, int truckUnitNumber);
	bool CompleteLoad(int loadNumber);
};