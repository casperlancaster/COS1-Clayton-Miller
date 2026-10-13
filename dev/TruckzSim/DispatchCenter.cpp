#include "DispatchCenter.h"

#include <iomanip>
#include <iostream>

#include "Driver.h"
#include "InputManager.h"
#include "Load.h"
#include "Truck.h"

void DispatchCenter::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== DISPATCH CENTER ==========\n";
    std::cout << "1. Assign Load\n";
    std::cout << "2. View Available Resources\n";
    std::cout << "3. Complete Load\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=====================================\n";
}

const Load* DispatchCenter::FindLoad(const Company& company, int loadNumber)
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

const Driver* DispatchCenter::FindDriver(const Company& company, int driverID)
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

const Truck* DispatchCenter::FindTruck(
    const Company& company,
    int truckUnitNumber)
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

bool DispatchCenter::HasAvailableLoad(const Company& company)
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

bool DispatchCenter::HasAvailableDriver(const Company& company)
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

bool DispatchCenter::HasAvailableTruck(const Company& company)
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

void DispatchCenter::DisplayAvailableResources(const Company& company)
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

void DispatchCenter::DisplayAssignedLoads(const Company& company)
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

void DispatchCenter::Run(DispatchManager& dispatchManager)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();
        choice = InputManager::ReadInt("Select an option: ");

        switch (choice)
        {
        case 1:
        {
            if (!dispatchManager.HasCompany())
            {
                std::cout
                    << "\nDispatch Center is not connected to a company.\n";
                break;
            }

            Company* company = dispatchManager.GetCompany();

            DisplayAvailableResources(*company);

            bool canDispatch = true;

            if (!HasAvailableLoad(*company))
            {
                std::cout
                    << "\nA load cannot be assigned because there are no available loads.\n";
                canDispatch = false;
            }

            if (!HasAvailableDriver(*company))
            {
                std::cout
                    << "A load cannot be assigned because there are no available drivers.\n";
                canDispatch = false;
            }

            if (!HasAvailableTruck(*company))
            {
                std::cout
                    << "A load cannot be assigned because there are no available trucks.\n";
                canDispatch = false;
            }

            if (!canDispatch)
            {
                std::cout
                    << "Add or free the required resources before trying again.\n";
                break;
            }

            std::cout << "\n========== ASSIGN LOAD ==========\n";

            int loadNumber = InputManager::ReadPositiveInt(
                "Enter a load number: ",
                "load number"
            );

            const Load* selectedLoad = FindLoad(*company, loadNumber);

            if (selectedLoad == nullptr)
            {
                std::cout
                    << "Load #" << loadNumber << " does not exist.\n";
                break;
            }

            if (selectedLoad->GetStatus() != "Available")
            {
                std::cout << "Load #" << loadNumber
                    << " is currently " << selectedLoad->GetStatus()
                    << " and cannot be assigned.\n";
                break;
            }

            int driverID = InputManager::ReadPositiveInt(
                "Enter Driver ID: ",
                "Driver ID"
            );

            const Driver* selectedDriver = FindDriver(*company, driverID);

            if (selectedDriver == nullptr)
            {
                std::cout
                    << "Driver ID " << driverID << " does not exist.\n";
                break;
            }

            if (selectedDriver->GetStatus() != "Available")
            {
                std::cout << "Driver ID " << driverID
                    << " is currently " << selectedDriver->GetStatus()
                    << " and cannot be assigned.\n";
                break;
            }

            int truckUnitNumber = InputManager::ReadPositiveInt(
                "Enter Truck Unit Number: ",
                "Truck Unit Number"
            );

            const Truck* selectedTruck =
                FindTruck(*company, truckUnitNumber);

            if (selectedTruck == nullptr)
            {
                std::cout
                    << "Truck #" << truckUnitNumber << " does not exist.\n";
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
                std::cout
                    << "\nDispatch Center is not connected to a company.\n";
                break;
            }

            DisplayAvailableResources(*dispatchManager.GetCompany());
            break;
        }

        case 3:
        {
            if (!dispatchManager.HasCompany())
            {
                std::cout
                    << "\nDispatch Center is not connected to a company.\n";
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
                std::cout
                    << "\nThere are no assigned loads to complete.\n";
                break;
            }

            int loadNumber = InputManager::ReadPositiveInt(
                "\nEnter the load number to complete: ",
                "load number"
            );

            const Load* selectedLoad = FindLoad(*company, loadNumber);

            if (selectedLoad == nullptr)
            {
                std::cout
                    << "Load #" << loadNumber << " does not exist.\n";
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
            int truckUnitNumber =
                selectedLoad->GetAssignedTruckUnitNumber();

            bool completed =
                dispatchManager.CompleteLoad(loadNumber);

            if (completed)
            {
                std::cout
                    << "\nLoad #" << loadNumber
                    << " completed successfully.\n";
                std::cout
                    << "Driver ID " << driverID
                    << " is now Available.\n";
                std::cout
                    << "Truck #" << truckUnitNumber
                    << " is now Available.\n";
                std::cout
                    << "Company revenue is now $"
                    << std::fixed
                    << std::setprecision(2)
                    << company->GetRevenue()
                    << std::defaultfloat
                    << std::setprecision(6)
                    << ".\n";
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
            std::cout
                << "\nInvalid selection. Please choose 0, 1, 2, or 3.\n";
            break;
        }
    }
}
