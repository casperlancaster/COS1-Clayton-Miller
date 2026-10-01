#include <iostream>
#include <string>
#include <limits>

#include "Truck.h"
#include "Driver.h"
#include "Load.h"
#include "Company.h"
#include "DispatchManager.h"

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
    std::cout << "Select an option: ";
}

void ShowFleetMenu()
{
    std::cout << "\n";
    std::cout << "========== FLEET MANAGEMENT ==========\n";
    std::cout << "1. Add Truck\n";
    std::cout << "2. View Trucks\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "======================================\n";
    std::cout << "Select an option: ";
}

void ShowDriverMenu()
{
    std::cout << "\n";
    std::cout << "========== DRIVER MANAGEMENT ==========\n";
    std::cout << "1. Add Driver\n";
    std::cout << "2. View Drivers\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=======================================\n";
    std::cout << "Select an option: ";
}

void ShowFreightMarketMenu()
{
    std::cout << "\n";
    std::cout << "========== FREIGHT MARKET ==========\n";
    std::cout << "1. Add Load\n";
    std::cout << "2. View Loads\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "====================================\n";
    std::cout << "Select an option: ";
}

void ShowDispatchMenu()
{
    std::cout << "\n";
    std::cout << "========== DISPATCH CENTER ==========\n";
    std::cout << "1. Assign Load\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "=====================================\n";
    std::cout << "Select an option: ";
}

void AddTruck(Company& company)
{
    Truck newTruck;

    int unitNumber = 0;
    double mileage = 0.0;

    std::cout << "\n========== ADD TRUCK ==========\n";

    std::cout << "Enter unit number: ";
    std::cin >> unitNumber;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid unit number.\n";
        return;

    }

    std::cout << "Enter current mileage: ";
    std::cin >> mileage;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid mileage.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    newTruck.SetUnitNumber(unitNumber);
    newTruck.SetMileage(mileage);
    newTruck.SetStatus("Available");

    company.AddTruck(newTruck);

    std::cout << "\nTruck #" << unitNumber << " added successfully.\n";

}

void DispalyTrucks(const Company& company)
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

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            choice = -1;

            std::cout << "\nInvalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            AddTruck(company);
            break;

        case 2:
            DispalyTrucks(company);
            break;

        case 0:
            break;

        default:
            std::cout << "\nInvalid selection.\n";
            break;
        }
    }
}

void AddDriver(Company& company)
{
    Driver newDriver;

    int driverID = 0;
    std::string driverName;

    std::cout << "\n========== ADD DRIVER ==========\n";

    std::cout << "Enter Driver ID: ";
    std::cin >> driverID;

    if (std::cin.fail() || driverID <= 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "Invalid driver ID.\n";
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter driver name: ";
    std::getline(std::cin, driverName);

    if (driverName.empty())
    {
        std::cout << "Driver name cannot be empty.\n";
        return;
    }

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

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            choice = -1;

            std::cout << "\nInvalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
            std::cout << "\nInvalid selection.\n";
            break;
        }
    }
}

void AddLoad(Company& company)
{
    Load newLoad;

    int loadNumber = 0;
    std::string origin;
    std::string destination;
    double pay = 0.0;

    std::cout << "\n========== ADD LOAD ==========\n";

    std::cout << "Enter load number: ";
    std::cin >> loadNumber;

    if (std::cin.fail() || loadNumber <= 0)
    {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Invalid load number.\n";
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::cout << "Enter origin: ";
    std::getline(std::cin, origin);

    if (origin.empty())
    {
        std::cout << "Origin cannot be empty.\n";
        return;
    }

    std::cout << "Enter destination: ";
    std::getline(std::cin, destination);

    if (destination.empty())
    {
        std::cout << "Destination cannot be empty.\n";
        return;
    }

    std::cout << "Enter load pay: $";
    std::cin >> pay;

    if (std::cin.fail() || pay < 0.0)
    {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Invalid load pay.\n";
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    newLoad.SetLoadNumber(loadNumber);
    newLoad.SetOrigin(origin);
    newLoad.SetDestination(destination);
    newLoad.SetPay(pay);
    newLoad.SetStatus("Available");

    company.AddLoad(newLoad);

    std::cout << "\nLoad #" << loadNumber
        << " added successfully.\n";
}

void DisplayLoads(const Company & company)
{
    const std::vector<Load>& loads = company.GetLoads();

    std::cout << "\n========== AVAILABLE LOADS ==========\n";

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
            std::cout << "Pay: $" << loads[i].GetPay() << "\n";
            std::cout << "Status: " << loads[i].GetStatus() << "\n";

            if (loads[i].GetStatus() == "Assigned")
            {
                std::cout << "Assigned Driver ID: " << loads[i].GetAssignedDriverID() << "\n";
                std::cout << "Assigned Truck: " << loads[i].GetAssignedTruckUnitNumber() << "\n";
            }
        }

        
    }

    std::cout << "=====================================\n";
}

void RunFreightMarket(Company & company)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowFreightMarketMenu();

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            choice = -1;

            std::cout << "\nInvalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

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
            std::cout << "\nInvalid selection.\n";
            break;
        }
    }
}

void RunDispatchCenter(DispatchManager& dispatchManager)
{
    int choice = -1;

    while (choice != 0)
    {
        ShowDispatchMenu();

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            choice = -1;

            std::cout << "\nInvalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            int loadNumber = 0;
            int driverID = 0;
            int truckUnitNumber = 0;

            std::cout << "\n========== ASSIGN LOAD ==========\n";

            std::cout << "Enter a load number: ";
            std::cin >> loadNumber;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Invalid load number.\n";
                break;
            }

            std::cout << "Enter Driver ID: ";
            std::cin >> driverID;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Invalid Driver ID.\n";
                break;
            }

            std::cout << "Enter Truck Unit Number: ";
            std::cin >> truckUnitNumber;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Invalid Truck Unit Number.\n";
                break;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            bool assigned = dispatchManager.AssignLoad(loadNumber, driverID, truckUnitNumber);

            if (assigned)
            {
                std::cout << "\nLoad #" << loadNumber << " assigned successfully.\n";
            }
            else
            {
                std::cout << "\nUnable to assign load. Check that the load, " << "driver, and truck exist and are available.\n";
            }
            break;
        }

        case 0:
            break;

        default:
            std::cout << "\nInvalid selection.\n";
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

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

            choice = -1;

            std::cout << "\nInvalid input. Please enter a number from 0 to 7.\n\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
            std::cout << "Invalid selection. Please try again.\n";
            break;
        }

        std::cout << "\n";
    }

    return 0;
}