#include "Company.h"

Company::Company()
{
    companyName = "";
    revenue = 0.0;
    expenses = 0.0;

    dateFormat = "MM/DD/YYYY";
    timeFormat = "12-hour";
}

std::string Company::GetCompanyName() const
{
    return companyName;
}

void Company::SetCompanyName(
    const std::string& newCompanyName)
{
    companyName = newCompanyName;
}

double Company::GetRevenue() const
{
    return revenue;
}

void Company::AddRevenue(double amount)
{
    if (amount > 0.0)
    {
        revenue += amount;
    }
}

double Company::GetExpenses() const
{
    return expenses;
}

void Company::AddExpense(double amount)
{
    if (amount > 0.0)
    {
        expenses += amount;
    }
}

double Company::GetBalance() const
{
    return revenue - expenses;
}

std::string Company::GetDateFormat() const
{
    return dateFormat;
}

void Company::SetDateFormat(
    const std::string& newDateFormat)
{
    if (newDateFormat == "MM/DD/YYYY" ||
        newDateFormat == "YYYY-MM-DD")
    {
        dateFormat = newDateFormat;
    }
}

std::string Company::GetTimeFormat() const
{
    return timeFormat;
}

void Company::SetTimeFormat(
    const std::string& newTimeFormat)
{
    if (newTimeFormat == "12-hour" ||
        newTimeFormat == "24-hour")
    {
        timeFormat = newTimeFormat;
    }
}

void Company::RecordIncomeTransaction(
    const FinancialTransaction& transaction)
{
    if (transaction.amount <= 0.0)
    {
        return;
    }

    FinancialTransaction storedTransaction =
        transaction;

    storedTransaction.type = "Income";

    revenue += storedTransaction.amount;

    financialTransactions.push_back(
        storedTransaction
    );
}

void Company::RecordExpenseTransaction(
    const FinancialTransaction& transaction)
{
    if (transaction.amount <= 0.0)
    {
        return;
    }

    FinancialTransaction storedTransaction =
        transaction;

    storedTransaction.type = "Expense";

    expenses += storedTransaction.amount;

    financialTransactions.push_back(
        storedTransaction
    );
}

void Company::RestoreFinancialTransaction(
    const FinancialTransaction& transaction)
{
    if (transaction.amount <= 0.0)
    {
        return;
    }

    if (transaction.type != "Income" &&
        transaction.type != "Expense")
    {
        return;
    }

    financialTransactions.push_back(
        transaction
    );
}

void Company::AddTruck(const Truck& newTruck)
{
    trucks.push_back(newTruck);
}

void Company::AddDriver(const Driver& newDriver)
{
    drivers.push_back(newDriver);
}

void Company::AddLoad(const Load& newLoad)
{
    loads.push_back(newLoad);
}

bool Company::HasTruckUnitNumber(
    int unitNumber) const
{
    for (const Truck& truck : trucks)
    {
        if (truck.GetUnitNumber() == unitNumber)
        {
            return true;
        }
    }

    return false;
}

bool Company::HasDriverID(int driverID) const
{
    for (const Driver& driver : drivers)
    {
        if (driver.GetDriverID() == driverID)
        {
            return true;
        }
    }

    return false;
}

bool Company::HasLoadNumber(
    int loadNumber) const
{
    for (const Load& load : loads)
    {
        if (load.GetLoadNumber() == loadNumber)
        {
            return true;
        }
    }

    return false;
}

bool Company::ChangeTruckUnitNumber(
    int oldUnitNumber,
    int newUnitNumber)
{
    if (oldUnitNumber <= 0 ||
        newUnitNumber <= 0)
    {
        return false;
    }

    if (oldUnitNumber == newUnitNumber)
    {
        return HasTruckUnitNumber(oldUnitNumber);
    }

    if (HasTruckUnitNumber(newUnitNumber))
    {
        return false;
    }

    Truck* selectedTruck = nullptr;

    for (Truck& truck : trucks)
    {
        if (truck.GetUnitNumber() == oldUnitNumber)
        {
            selectedTruck = &truck;
            break;
        }
    }

    if (selectedTruck == nullptr)
    {
        return false;
    }

    selectedTruck->SetUnitNumber(newUnitNumber);

    for (Load& load : loads)
    {
        if (load.GetAssignedTruckUnitNumber() == oldUnitNumber)
        {
            load.SetAssignedTruckUnitNumber(newUnitNumber);
        }
    }

    for (FinancialTransaction& transaction : financialTransactions)
    {
        if (transaction.truckUnitNumber == oldUnitNumber)
        {
            transaction.truckUnitNumber = newUnitNumber;
        }
    }

    return true;
}

bool Company::ChangeDriverID(
    int oldDriverID,
    int newDriverID)
{
    if (oldDriverID <= 0 ||
        newDriverID <= 0)
    {
        return false;
    }

    if (oldDriverID == newDriverID)
    {
        return HasDriverID(oldDriverID);
    }

    if (HasDriverID(newDriverID))
    {
        return false;
    }

    Driver* selectedDriver = nullptr;

    for (Driver& driver : drivers)
    {
        if (driver.GetDriverID() == oldDriverID)
        {
            selectedDriver = &driver;
            break;
        }
    }

    if (selectedDriver == nullptr)
    {
        return false;
    }

    selectedDriver->SetDriverID(newDriverID);

    for (Load& load : loads)
    {
        if (load.GetAssignedDriverID() == oldDriverID)
        {
            load.SetAssignedDriverID(newDriverID);
        }

        if (load.GetAssignedSecondDriverID() == oldDriverID)
        {
            load.SetAssignedSecondDriverID(newDriverID);
        }
    }

    for (FinancialTransaction& transaction : financialTransactions)
    {
        if (transaction.primaryDriverID == oldDriverID)
        {
            transaction.primaryDriverID = newDriverID;
        }

        if (transaction.secondDriverID == oldDriverID)
        {
            transaction.secondDriverID = newDriverID;
        }
    }

    return true;
}

bool Company::ChangeLoadNumber(
    int oldLoadNumber,
    int newLoadNumber)
{
    if (oldLoadNumber <= 0 ||
        newLoadNumber <= 0)
    {
        return false;
    }

    if (oldLoadNumber == newLoadNumber)
    {
        return HasLoadNumber(oldLoadNumber);
    }

    if (HasLoadNumber(newLoadNumber))
    {
        return false;
    }

    Load* selectedLoad = nullptr;

    for (Load& load : loads)
    {
        if (load.GetLoadNumber() == oldLoadNumber)
        {
            selectedLoad = &load;
            break;
        }
    }

    if (selectedLoad == nullptr)
    {
        return false;
    }

    selectedLoad->SetLoadNumber(newLoadNumber);

    for (FinancialTransaction& transaction : financialTransactions)
    {
        if (transaction.loadNumber == oldLoadNumber)
        {
            transaction.loadNumber = newLoadNumber;
        }
    }

    return true;
}

std::vector<Truck>& Company::GetTrucks()
{
    return trucks;
}

std::vector<Driver>& Company::GetDrivers()
{
    return drivers;
}

std::vector<Load>& Company::GetLoads()
{
    return loads;
}

const std::vector<Truck>&
Company::GetTrucks() const
{
    return trucks;
}

const std::vector<Driver>&
Company::GetDrivers() const
{
    return drivers;
}

const std::vector<Load>&
Company::GetLoads() const
{
    return loads;
}

const std::vector<FinancialTransaction>&
Company::GetFinancialTransactions() const
{
    return financialTransactions;
}
