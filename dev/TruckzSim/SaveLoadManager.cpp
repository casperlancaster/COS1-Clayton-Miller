#include "SaveLoadManager.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "Company.h"
#include "Driver.h"
#include "Load.h"
#include "Truck.h"

namespace
{
    const std::string SAVE_DIRECTORY =
        "saves";

    const std::string SAVE_EXTENSION =
        ".txt";

    const std::string SAVE_FILE_HEADER_V1 =
        "TRUCKZSIM_SAVE_V1";

    const std::string SAVE_FILE_HEADER_V2 =
        "TRUCKZSIM_SAVE_V2";

    constexpr std::size_t MAX_SAVE_RECORDS =
        10000;
}

std::string SaveLoadManager::CreateSaveFileName(
    const std::string& companyName)
{
    std::string fileName;

    for (char character : companyName)
    {
        if (std::isalnum(
            static_cast<unsigned char>(
                character)))
        {
            fileName += character;
        }
        else if (character == ' ' ||
            character == '-' ||
            character == '_')
        {
            fileName += '_';
        }
    }

    if (fileName.empty())
    {
        fileName = "company";
    }

    return SAVE_DIRECTORY +
        "/" +
        fileName +
        SAVE_EXTENSION;
}

bool SaveLoadManager::HasSavedCompanies()
{
    return !GetSavedCompanies().empty();
}

std::vector<std::string>
SaveLoadManager::GetSavedCompanies()
{
    std::vector<std::string> companyNames;

    namespace fs = std::filesystem;

    if (!fs::exists(
        SAVE_DIRECTORY))
    {
        return companyNames;
    }

    for (const fs::directory_entry& entry :
        fs::directory_iterator(
            SAVE_DIRECTORY))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        if (entry.path().extension() !=
            SAVE_EXTENSION)
        {
            continue;
        }

        std::ifstream file(
            entry.path()
        );

        if (!file)
        {
            continue;
        }

        std::string header;

        std::getline(
            file,
            header
        );

        if (header !=
            SAVE_FILE_HEADER_V1 &&
            header !=
            SAVE_FILE_HEADER_V2)
        {
            continue;
        }

        std::string companyName;

        if (file >>
            std::quoted(
                companyName))
        {
            companyNames.push_back(
                companyName
            );
        }
    }

    std::sort(
        companyNames.begin(),
        companyNames.end()
    );

    return companyNames;
}

bool SaveLoadManager::SaveCompany(
    const Company& company)
{
    namespace fs = std::filesystem;

    try
    {
        fs::create_directories(
            SAVE_DIRECTORY
        );
    }
    catch (...)
    {
        return false;
    }

    std::string fileName =
        CreateSaveFileName(
            company.GetCompanyName()
        );

    std::ofstream file(
        fileName,
        std::ios::trunc
    );

    if (!file)
    {
        return false;
    }

    file
        << SAVE_FILE_HEADER_V2
        << "\n";

    file
        << std::quoted(
            company.GetCompanyName()
        )
        << "\n";

    file
        << std::setprecision(17)
        << company.GetRevenue()
        << " "
        << company.GetExpenses()
        << "\n";

    const std::vector<Truck>& trucks =
        company.GetTrucks();

    file
        << trucks.size()
        << "\n";

    for (const Truck& truck : trucks)
    {
        file
            << truck.GetUnitNumber()
            << " "
            << std::setprecision(17)
            << truck.GetMileage()
            << " "
            << std::quoted(
                truck.GetStatus()
            )
            << " "
            << truck.GetLastServiceMileage()
            << " "
            << truck.GetServiceIntervalMiles()
            << " "
            << truck.GetFuelTankCapacity()
            << " "
            << truck.GetFuelGallons()
            << " "
            << truck.GetMilesPerGallon()
            << "\n";
    }

    const std::vector<Driver>& drivers =
        company.GetDrivers();

    file
        << drivers.size()
        << "\n";

    for (const Driver& driver :
        drivers)
    {
        file
            << driver.GetDriverID()
            << " "
            << std::quoted(
                driver.GetName()
            )
            << " "
            << std::quoted(
                driver.GetStatus()
            )
            << "\n";
    }

    const std::vector<Load>& loads =
        company.GetLoads();

    file
        << loads.size()
        << "\n";

    for (const Load& load : loads)
    {
        file
            << load.GetLoadNumber()
            << " "
            << std::quoted(
                load.GetOrigin()
            )
            << " "
            << std::quoted(
                load.GetDestination()
            )
            << " "
            << std::setprecision(17)
            << load.GetDistanceMiles()
            << " "
            << load.GetPay()
            << " "
            << std::quoted(
                load.GetStatus()
            )
            << " "
            << load.GetAssignedDriverID()
            << " "
            << load.GetAssignedTruckUnitNumber()
            << "\n";
    }

    return static_cast<bool>(
        file
        );
}

bool SaveLoadManager::LoadCompany(
    Company& company,
    const std::string& companyName)
{
    std::string fileName =
        CreateSaveFileName(
            companyName
        );

    std::ifstream file(
        fileName
    );

    if (!file)
    {
        return false;
    }

    std::string fileHeader;

    std::getline(
        file,
        fileHeader
    );

    bool hasFuelData = false;

    if (fileHeader ==
        SAVE_FILE_HEADER_V2)
    {
        hasFuelData = true;
    }
    else if (fileHeader ==
        SAVE_FILE_HEADER_V1)
    {
        hasFuelData = false;
    }
    else
    {
        return false;
    }

    Company loadedCompany;

    std::string loadedCompanyName;
    double revenue = 0.0;
    double expenses = 0.0;

    if (!(file >>
        std::quoted(
            loadedCompanyName)))
    {
        return false;
    }

    if (!(file >>
        revenue >>
        expenses))
    {
        return false;
    }

    if (revenue < 0.0 ||
        expenses < 0.0)
    {
        return false;
    }

    loadedCompany.SetCompanyName(
        loadedCompanyName
    );

    loadedCompany.AddRevenue(
        revenue
    );

    loadedCompany.AddExpense(
        expenses
    );

    std::size_t truckCount = 0;

    if (!(file >> truckCount) ||
        truckCount >
        MAX_SAVE_RECORDS)
    {
        return false;
    }

    for (std::size_t i = 0;
        i < truckCount;
        i++)
    {
        int unitNumber = 0;
        double mileage = 0.0;
        std::string status;

        double lastServiceMileage = 0.0;
        double serviceIntervalMiles = 0.0;

        double fuelTankCapacity = 200.0;
        double fuelGallons = 200.0;
        double milesPerGallon = 7.0;

        if (!(file
            >> unitNumber
            >> mileage
            >> std::quoted(
                status)
            >> lastServiceMileage
            >> serviceIntervalMiles))
        {
            return false;
        }

        if (hasFuelData)
        {
            if (!(file
                >> fuelTankCapacity
                >> fuelGallons
                >> milesPerGallon))
            {
                return false;
            }
        }

        if (unitNumber <= 0 ||
            mileage < 0.0 ||
            lastServiceMileage < 0.0 ||
            lastServiceMileage >
            mileage ||
            serviceIntervalMiles <= 0.0 ||
            loadedCompany.
            HasTruckUnitNumber(
                unitNumber))
        {
            return false;
        }

        if (fuelTankCapacity <= 0.0 ||
            fuelGallons < 0.0 ||
            fuelGallons >
            fuelTankCapacity ||
            milesPerGallon <= 0.0)
        {
            return false;
        }

        Truck truck;

        truck.SetUnitNumber(
            unitNumber
        );

        truck.SetMileage(
            mileage
        );

        truck.SetStatus(
            status
        );

        truck.RestoreMaintenanceSchedule(
            lastServiceMileage,
            serviceIntervalMiles
        );

        truck.RestoreFuelState(
            fuelTankCapacity,
            fuelGallons,
            milesPerGallon
        );

        loadedCompany.AddTruck(
            truck
        );
    }

    std::size_t driverCount = 0;

    if (!(file >> driverCount) ||
        driverCount >
        MAX_SAVE_RECORDS)
    {
        return false;
    }

    for (std::size_t i = 0;
        i < driverCount;
        i++)
    {
        int driverID = 0;
        std::string name;
        std::string status;

        if (!(file
            >> driverID
            >> std::quoted(
                name)
            >> std::quoted(
                status)))
        {
            return false;
        }

        if (driverID <= 0 ||
            name.empty() ||
            loadedCompany.
            HasDriverID(
                driverID))
        {
            return false;
        }

        Driver driver;

        driver.SetDriverID(
            driverID
        );

        driver.SetName(
            name
        );

        driver.SetStatus(
            status
        );

        loadedCompany.AddDriver(
            driver
        );
    }

    std::size_t loadCount = 0;

    if (!(file >> loadCount) ||
        loadCount >
        MAX_SAVE_RECORDS)
    {
        return false;
    }

    for (std::size_t i = 0;
        i < loadCount;
        i++)
    {
        int loadNumber = 0;
        std::string origin;
        std::string destination;
        double distanceMiles = 0.0;
        double pay = 0.0;
        std::string status;
        int assignedDriverID = 0;
        int assignedTruckUnitNumber = 0;

        if (!(file
            >> loadNumber
            >> std::quoted(
                origin)
            >> std::quoted(
                destination)
            >> distanceMiles
            >> pay
            >> std::quoted(
                status)
            >> assignedDriverID
            >> assignedTruckUnitNumber))
        {
            return false;
        }

        if (loadNumber <= 0 ||
            origin.empty() ||
            destination.empty() ||
            distanceMiles < 0.0 ||
            pay < 0.0 ||
            assignedDriverID < 0 ||
            assignedTruckUnitNumber < 0 ||
            loadedCompany.
            HasLoadNumber(
                loadNumber))
        {
            return false;
        }

        if (assignedDriverID > 0 &&
            !loadedCompany.
            HasDriverID(
                assignedDriverID))
        {
            return false;
        }

        if (assignedTruckUnitNumber > 0 &&
            !loadedCompany.
            HasTruckUnitNumber(
                assignedTruckUnitNumber))
        {
            return false;
        }

        Load load;

        load.SetLoadNumber(
            loadNumber
        );

        load.SetOrigin(
            origin
        );

        load.SetDestination(
            destination
        );

        load.SetDistanceMiles(
            distanceMiles
        );

        load.SetPay(
            pay
        );

        load.SetStatus(
            status
        );

        load.SetAssignedDriverID(
            assignedDriverID
        );

        load.SetAssignedTruckUnitNumber(
            assignedTruckUnitNumber
        );

        loadedCompany.AddLoad(
            load
        );
    }

    if (!file)
    {
        return false;
    }

    company =
        loadedCompany;

    return true;
}