#include <iostream>
#include <string>
#include <iomanip>

#include "Truck.h"
#include "Driver.h"
#include "Load.h"
#include "Company.h"
#include "DispatchManager.h"


bool TryParseInt(const std::string& input, int& value)
{
    try
    {
        std::size_t charactersProcessed = 0;
        value = std::stoi(input, &charactersProcessed);

        return charactersProcessed == input.length();
    }
    catch (...)
    {
        return false;
    }
}


bool TryParseDouble(const std::string& input, double& value)
{
    try
    {
        std::size_t charactersProcessed = 0;
        value = std::stod(input, &charactersProcessed);

        return charactersProcessed == input.length();
    }
    catch (...)
    {
        return false;
    }
}


int ReadInt(const std::string& prompt)
{
    while (true)
    {
        std::string input;
        int value = 0;

        std::cout << prompt;
        std::getline(std::cin, input);

        if (TryParseInt(input, value))
        {
            return value;
        }

        std::cout << "Invalid input. Please enter a whole number.\n";
    }
}


int ReadPositiveInt(const std::string& prompt, const std::string& fieldName)
{
    while (true)
    {
        int value = ReadInt(prompt);

        if (value > 0)
        {
            return value;
        }

        std::cout << "Invalid " << fieldName
            << ". Please enter a number greater than 0.\n";
    }
}


double ReadNonNegativeDouble(const std::string& prompt, const std::string& fieldName)
{
    while (true)
    {
        std::string input;
        double value = 0.0;

        std::cout << prompt;
        std::getline(std::cin, input);

        if (TryParseDouble(input, value) && value >= 0.0)
        {
            return value;
        }

        std::cout << "Invalid " << fieldName
            << ". Please enter a number that is 0 or greater.\n";
    }
}


std::string ReadRequiredText(const std::string& prompt, const std::string& fieldName)
{
    while (true)
    {
        std::string input;

        std::cout << prompt;
        std::getline(std::cin, input);

        if (!input.empty())
        {
            return input;
        }

        std::cout << fieldName << " cannot be empty. Please try again.\n";
    }
}


double ReadCurrency(const std::string& prompt)
{
    while (true)
    {
        std::string payInput;
        std::string cleanedPay;
        double pay = 0.0;

        std::cout << prompt;
        std::getline(std::cin, payInput);

        for (char character : payInput)
        {
            if (character != ',' && character != '$')
            {
                cleanedPay += character;
            }
        }

        if (TryParseDouble(cleanedPay, pay) && pay >= 0.0)
        {
            return pay;
        }

        std::cout << "Invalid load pay. Please enter a valid amount.\n";
    }
}


void ShowMainMenu()
{
    std::cout << "=============================================\n";
    std::cout << "               T R U C K Z S I M             \n";
    std::cout << "=============================================\n";
    std::cout << "         Trucking Company Simulator           \n";
    std::cout << "---------------------------------------------\n";
    std::cout << "1. Dispatch Center\n";
    std::cout << "2. Fleet Management\n";
    std::cout << "3. Driver Management\n";
    std::cout << "4. Freight Market\n";
    std::cout << "5. Company Overview\n";
    std::cout << "6. Banking\n";
    std::cout << "7. Maintenance\n";
    std::cout << "0. Exit\n";
    std::cout << "---------------------------------------------\n";
}


void ShowFleetMenu()
{
    std::cout << "\n";
    std::cout << "========== FLEET MANAGEMENT ==========\n";
    std::cout << "1. Add Truck\n";
    std::cout << "2. View Trucks\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "======================================\n";
}


void ShowDriverMenu()
{
    std::cout << "\n";
    std::cout << "========== DRIVER MANAGEMENT ==========\n";
    std::cout << "1. Add Driver\n";
    std::cout << "2. View Drivers\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=======================================\n";
}


void ShowFreightMarketMenu()
{
    std::cout << "\n";
    std::cout << "========== FREIGHT MARKET ==========\n";
    std::cout << "1. Add Load\n";
    std::cout << "2. View Loads\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "====================================\n";
}


void ShowDispatchMenu()
{
    std::cout << "\n";
    std::cout << "========== DISPATCH CENTER ==========\n";
    std::cout << "1. Assign Load\n";
    std::cout << "2. View Available Resources\n";
    std::cout << "3. Complete Load\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=====================================\n";
}


void AddTruck(Company& company)
{
    Truck newTruck;

    std::cout << "\n========== ADD TRUCK ==========\n";

    int unitNumber = ReadPositiveInt(
        "Enter unit number: ",
        "unit number"
    );

    double mileage = ReadNonNegativeDouble(
        "Enter current mileage: ",
        "mileage"
    );

    newTruck.SetUnitNumber(unitNumber);
    newTruck.SetMileage(mileage);
    newTruck.SetStatus("Available");

    company.AddTruck(newTruck);

    std::cout << "\nTruck #" << unitNumber << " added successfully.\n";
}


void DisplayTrucks(const Company& company)
{
    const std::vector<Truck>& trucks = company.GetTrucks();

    std::cout << "\n========== COMPANY FLEET ==========\n";

    if (trucks.empty())
    {
        std::cout << "No trucks have been added yet.\n";
    }
    else
    {
        for (int i = 0; i < trucks.size(); i++)
        {
            std::cout << "\nTruck #" << trucks[i].GetUnitNumber() << "\n";
            std::cout << "Mileage: " << trucks[i].GetMileage() << "\n";
            std::cout << "Status: " << trucks[i].GetStatus() << "\n";
        }
    }

    std::cout << "==================================\n";
}


void RunFleetManagement(Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowFleetMenu();
        choice = ReadInt("Select an option: ");

        switch (choice)
        {
        case 1:
            AddTruck(company);
            break;

        case 2:
            DisplayTrucks(company);
            break;

        case 0:
            break;

        default:
            std::cout << "\nInvalid selection. Please choose 0, 1, or 2.\n";
            break;
        }
    }
}


void AddDriver(Company& company)
{
    Driver newDriver;

    std::cout << "\n========== ADD DRIVER ==========\n";

    int driverID = ReadPositiveInt(
        "Enter Driver ID: ",
        "driver ID"
    );

    std::string driverName = ReadRequiredText(
        "Enter driver name: ",
        "Driver name"
    );

    newDriver.SetDriverID(driverID);
    newDriver.SetName(driverName);
    newDriver.SetStatus("Available");

    company.AddDriver(newDriver);

    std::cout << "\nDriver " << driverName << " added successfully.\n";
}


void DisplayDrivers(const Company& company)
{
    const std::vector<Driver>& drivers = company.GetDrivers();

    std::cout << "\n========== COMPANY DRIVERS ==========\n";

    if (drivers.empty())
    {
        std::cout << "No drivers have been added yet.\n";
    }
    else
    {
        for (int i = 0; i < drivers.size(); i++)
        {
            std::cout << "\nDriver ID: " << drivers[i].GetDriverID() << "\n";
            std::cout << "Name: " << drivers[i].GetName() << "\n";
            std::cout << "Status: " << drivers[i].GetStatus() << "\n";
        }
    }

    std::cout << "===============================================\n";
}


void RunDriverManagement(Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowDriverMenu();
        choice = ReadInt("Select an option: ");

        switch (choice)
        {
        case 1:
            AddDriver(company);
            break;

        case 2:
            DisplayDrivers(company);
            break;

        case 0:
            break;

        default:
            std::cout << "\nInvalid selection. Please choose 0, 1, or 2.\n";
            break;
        }
    }
}


void AddLoad(Company& company)
{
    Load newLoad;

    std::cout << "\n========== ADD LOAD ==========\n";

    int loadNumber = ReadPositiveInt(
        "Enter load number: ",
        "load number"
    );

    std::string origin = ReadRequiredText(
        "Enter origin: ",
        "Origin"
    );

    std::string destination = ReadRequiredText(
        "Enter destination: ",
        "Destination"
    );

    double pay = ReadCurrency("Enter load pay: $");

    newLoad.SetLoadNumber(loadNumber);
    newLoad.SetOrigin(origin);
    newLoad.SetDestination(destination);
    newLoad.SetPay(pay);
    newLoad.SetStatus("Available");

    company.AddLoad(newLoad);

    std::cout << "\nLoad #" << loadNumber
        << " added successfully.\n";
}


void DisplayLoads(const Company& company)
{
    const std::vector<Load>& loads = company.GetLoads();

    std::cout << "\n========== COMPANY LOADS ==========\n";

    if (loads.empty())
    {
        std::cout << "No loads have been added yet.\n";
    }
    else
    {
        for (int i = 0; i < loads.size(); i++)
        {
            std::cout << "\nLoad #" << loads[i].GetLoadNumber() << "\n";
            std::cout << "Origin: " << loads[i].GetOrigin() << "\n";
            std::cout << "Destination: " << loads[i].GetDestination() << "\n";
            std::cout << "Pay: $"
                << std::fixed
                << std::setprecision(2)
                << loads[i].GetPay()
                << std::defaultfloat
                << std::setprecision(6)
                << "\n";
            std::cout << "Status: " << loads[i].GetStatus() << "\n";

            if (loads[i].GetAssignedDriverID() > 0 &&
                loads[i].GetAssignedTruckUnitNumber() > 0)
            {
                std::cout << "Assigned Driver ID: "
                    << loads[i].GetAssignedDriverID() << "\n";
                std::cout << "Assigned Truck: "
                    << loads[i].GetAssignedTruckUnitNumber() << "\n";
            }
        }
    }

    std::cout << "=====================================\n";
}


void RunFreightMarket(Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowFreightMarketMenu();
        choice = ReadInt("Select an option: ");

        switch (choice)
        {
        case 1:
            AddLoad(company);
            break;

        case 2:
            DisplayLoads(company);
            break;

        case 0:
            break;

        default:
            std::cout << "\nInvalid selection. Please choose 0, 1, or 2.\n";
            break;
        }
    }
}


const Load* FindLoad(const Company& company, int loadNumber)
{
    for (const Load& load : company.GetLoads())
    {
        if (load.GetLoadNumber() == loadNumber)
        {
            return &load;
        }
    }

    return nullptr;
}


const Driver* FindDriver(const Company& company, int driverID)
{
    for (const Driver& driver : company.GetDrivers())
    {
        if (driver.GetDriverID() == driverID)
        {
            return &driver;
        }
    }

    return nullptr;
}


const Truck* FindTruck(const Company& company, int truckUnitNumber)
{
    for (const Truck& truck : company.GetTrucks())
    {
        if (truck.GetUnitNumber() == truckUnitNumber)
        {
            return &truck;
        }
    }

    return nullptr;
}


bool HasAvailableLoad(const Company& company)
{
    for (const Load& load : company.GetLoads())
    {
        if (load.GetStatus() == "Available")
        {
            return true;
        }
    }

    return false;
}


bool HasAvailableDriver(const Company& company)
{
    for (const Driver& driver : company.GetDrivers())
    {
        if (driver.GetStatus() == "Available")
        {
            return true;
        }
    }

    return false;
}


bool HasAvailableTruck(const Company& company)
{
    for (const Truck& truck : company.GetTrucks())
    {
        if (truck.GetStatus() == "Available")
        {
            return true;
        }
    }

    return false;
}


void DisplayAvailableDispatchResources(const Company& company)
{
    std::cout << "\n========== AVAILABLE LOADS ==========\n";

    bool foundLoad = false;

    for (const Load& load : company.GetLoads())
    {
        if (load.GetStatus() == "Available")
        {
            foundLoad = true;

            std::cout << "Load #" << load.GetLoadNumber()
                << " | " << load.GetOrigin()
                << " -> " << load.GetDestination()
                << " | Pay: $"
                << std::fixed
                << std::setprecision(2)
                << load.GetPay()
                << std::defaultfloat
                << std::setprecision(6)
                << "\n";
        }
    }

    if (!foundLoad)
    {
        std::cout << "No available loads.\n";
    }

    std::cout << "\n========== AVAILABLE DRIVERS ==========\n";

    bool foundDriver = false;

    for (const Driver& driver : company.GetDrivers())
    {
        if (driver.GetStatus() == "Available")
        {
            foundDriver = true;

            std::cout << "Driver ID " << driver.GetDriverID()
                << " | " << driver.GetName()
                << "\n";
        }
    }

    if (!foundDriver)
    {
        std::cout << "No available drivers.\n";
    }

    std::cout << "\n========== AVAILABLE TRUCKS ==========\n";

    bool foundTruck = false;

    for (const Truck& truck : company.GetTrucks())
    {
        if (truck.GetStatus() == "Available")
        {
            foundTruck = true;

            std::cout << "Truck #" << truck.GetUnitNumber()
                << " | Mileage: " << truck.GetMileage()
                << "\n";
        }
    }

    if (!foundTruck)
    {
        std::cout << "No available trucks.\n";
    }

    std::cout << "========================================\n";
}


void DisplayAssignedLoads(const Company& company)
{
    std::cout << "\n========== ASSIGNED LOADS ==========\n";

    bool foundAssignedLoad = false;

    for (const Load& load : company.GetLoads())
    {
        if (load.GetStatus() == "Assigned")
        {
            foundAssignedLoad = true;

            std::cout << "Load #" << load.GetLoadNumber()
                << " | " << load.GetOrigin()
                << " -> " << load.GetDestination()
                << " | Driver ID " << load.GetAssignedDriverID()
                << " | Truck #" << load.GetAssignedTruckUnitNumber()
                << " | Pay: $"
                << std::fixed
                << std::setprecision(2)
                << load.GetPay()
                << std::defaultfloat
                << std::setprecision(6)
                << "\n";
        }
    }

    if (!foundAssignedLoad)
    {
        std::cout << "No loads are currently assigned.\n";
    }

    std::cout << "====================================\n";
}


void RunDispatchCenter(DispatchManager& dispatchManager)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowDispatchMenu();
        choice = ReadInt("Select an option: ");

        switch (choice)
        {
        case 1:
        {
            if (!dispatchManager.HasCompany())
            {
                std::cout << "\nDispatch Center is not connected to a company.\n";
                break;
            }

            Company* company = dispatchManager.GetCompany();

            DisplayAvailableDispatchResources(*company);

            bool canDispatch = true;

            if (!HasAvailableLoad(*company))
            {
                std::cout << "\nA load cannot be assigned because there are no available loads.\n";
                canDispatch = false;
            }

            if (!HasAvailableDriver(*company))
            {
                std::cout << "A load cannot be assigned because there are no available drivers.\n";
                canDispatch = false;
            }

            if (!HasAvailableTruck(*company))
            {
                std::cout << "A load cannot be assigned because there are no available trucks.\n";
                canDispatch = false;
            }

            if (!canDispatch)
            {
                std::cout << "Add or free the required resources before trying again.\n";
                break;
            }

            std::cout << "\n========== ASSIGN LOAD ==========\n";

            int loadNumber = ReadPositiveInt(
                "Enter a load number: ",
                "load number"
            );

            const Load* selectedLoad = FindLoad(*company, loadNumber);

            if (selectedLoad == nullptr)
            {
                std::cout << "Load #" << loadNumber << " does not exist.\n";
                break;
            }

            if (selectedLoad->GetStatus() != "Available")
            {
                std::cout << "Load #" << loadNumber
                    << " is currently " << selectedLoad->GetStatus()
                    << " and cannot be assigned.\n";
                break;
            }

            int driverID = ReadPositiveInt(
                "Enter Driver ID: ",
                "Driver ID"
            );

            const Driver* selectedDriver = FindDriver(*company, driverID);

            if (selectedDriver == nullptr)
            {
                std::cout << "Driver ID " << driverID << " does not exist.\n";
                break;
            }

            if (selectedDriver->GetStatus() != "Available")
            {
                std::cout << "Driver ID " << driverID
                    << " is currently " << selectedDriver->GetStatus()
                    << " and cannot be assigned.\n";
                break;
            }

            int truckUnitNumber = ReadPositiveInt(
                "Enter Truck Unit Number: ",
                "Truck Unit Number"
            );

            const Truck* selectedTruck = FindTruck(*company, truckUnitNumber);

            if (selectedTruck == nullptr)
            {
                std::cout << "Truck #" << truckUnitNumber << " does not exist.\n";
                break;
            }

            if (selectedTruck->GetStatus() != "Available")
            {
                std::cout << "Truck #" << truckUnitNumber
                    << " is currently " << selectedTruck->GetStatus()
                    << " and cannot be assigned.\n";
                break;
            }

            bool assigned = dispatchManager.AssignLoad(
                loadNumber,
                driverID,
                truckUnitNumber
            );

            if (assigned)
            {
                std::cout << "\nLoad #" << loadNumber
                    << " assigned successfully to Driver ID "
                    << driverID << " using Truck #"
                    << truckUnitNumber << ".\n";
            }
            else
            {
                std::cout
                    << "\nUnable to assign load. The dispatch state changed "
                    << "before the assignment could be completed.\n";
            }

            break;
        }

        case 2:
        {
            if (!dispatchManager.HasCompany())
            {
                std::cout << "\nDispatch Center is not connected to a company.\n";
                break;
            }

            DisplayAvailableDispatchResources(*dispatchManager.GetCompany());
            break;
        }

        case 3:
        {
            if (!dispatchManager.HasCompany())
            {
                std::cout << "\nDispatch Center is not connected to a company.\n";
                break;
            }

            Company* company = dispatchManager.GetCompany();

            DisplayAssignedLoads(*company);

            bool hasAssignedLoad = false;

            for (const Load& load : company->GetLoads())
            {
                if (load.GetStatus() == "Assigned")
                {
                    hasAssignedLoad = true;
                    break;
                }
            }

            if (!hasAssignedLoad)
            {
                std::cout << "\nThere are no assigned loads to complete.\n";
                break;
            }

            int loadNumber = ReadPositiveInt(
                "\nEnter the load number to complete: ",
                "load number"
            );

            const Load* selectedLoad = FindLoad(*company, loadNumber);

            if (selectedLoad == nullptr)
            {
                std::cout << "Load #" << loadNumber << " does not exist.\n";
                break;
            }

            if (selectedLoad->GetStatus() != "Assigned")
            {
                std::cout << "Load #" << loadNumber
                    << " is currently " << selectedLoad->GetStatus()
                    << " and cannot be completed.\n";
                break;
            }

            int driverID = selectedLoad->GetAssignedDriverID();
            int truckUnitNumber = selectedLoad->GetAssignedTruckUnitNumber();

            bool completed = dispatchManager.CompleteLoad(loadNumber);

            if (completed)
            {
                std::cout << "\nLoad #" << loadNumber
                    << " completed successfully.\n";
                std::cout << "Driver ID " << driverID
                    << " is now Available.\n";
                std::cout << "Truck #" << truckUnitNumber
                    << " is now Available.\n";
            }
            else
            {
                std::cout
                    << "\nUnable to complete load. Check the dispatch state "
                    << "and try again.\n";
            }

            break;
        }

        case 0:
            break;

        default:
            std::cout << "\nInvalid selection. Please choose 0, 1, 2, or 3.\n";
            break;
        }
    }
}


int main()
{
    Company company;
    company.SetCompanyName("TruckzSim Logistics");

    DispatchManager dispatchManager(&company);

    int choice = -1;

    while (choice != 0)
    {
        ShowMainMenu();
        choice = ReadInt("Select an option: ");

        std::cout << "\n";

        switch (choice)
        {
        case 1:
            RunDispatchCenter(dispatchManager);
            break;

        case 2:
            RunFleetManagement(company);
            break;

        case 3:
            RunDriverManagement(company);
            break;

        case 4:
            RunFreightMarket(company);
            break;

        case 5:
            std::cout << "========== COMPANY OVERVIEW ==========\n";
            std::cout << "Company: " << company.GetCompanyName() << "\n";
            std::cout << "Trucks: " << company.GetTrucks().size() << "\n";
            std::cout << "Drivers: " << company.GetDrivers().size() << "\n";
            std::cout << "Loads: " << company.GetLoads().size() << "\n";
            std::cout << "========================================\n";
            break;

        case 6:
            std::cout << "Banking is under development.\n";
            break;

        case 7:
            std::cout << "Maintenance is under development.\n";
            break;

        case 0:
            std::cout << "Exiting TruckzSim...\n";
            break;

        default:
            std::cout << "Invalid selection. Please enter a number from 0 to 7.\n";
            break;
        }

        std::cout << "\n";
    }

    return 0;
}
