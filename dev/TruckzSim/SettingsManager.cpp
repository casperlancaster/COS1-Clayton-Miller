#include "SettingsManager.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "HOSManager.h"
#include "InputManager.h"

void SettingsManager::ApplyDateTimeSettings(
    const Company& company)
{
    HOSManager::SetDateFormat(
        company.GetDateFormat()
    );

    HOSManager::SetTimeFormat(
        company.GetTimeFormat()
    );
}

void SettingsManager::ShowMenu()
{
    std::cout << "\n";
    std::cout << "========== SETTINGS ==========\n";
    std::cout << "1. Date / Time Format\n";
    std::cout << "2. View Current Settings\n";
    std::cout << "3. Edit Truck\n";
    std::cout << "4. Edit Driver\n";
    std::cout << "5. Edit Load\n";
    std::cout << "6. Reset Display Settings\n";
    std::cout << "0. Return to Main Menu\n";
    std::cout << "==============================\n";
}

void SettingsManager::ShowCurrentSettings(
    const Company& company)
{
    std::cout
        << "\n========== CURRENT SETTINGS ==========\n";

    std::cout
        << "Date Format: "
        << company.GetDateFormat()
        << "\n";

    std::cout
        << "Time Format: "
        << company.GetTimeFormat()
        << "\n";

    std::cout
        << "Date Entry Example: "
        << HOSManager::GetDateInputExample()
        << "\n";

    std::cout
        << "Time Entry Example: "
        << HOSManager::GetTimeInputExample()
        << "\n";

    std::cout
        << "Flexible entry is enabled. Dates may use "
        << "slashes, dashes, spaces, or numbers only.\n";

    std::cout
        << "Time colons are optional. AM/PM is accepted, "
        << "and 24-hour time is also accepted.\n";

    std::cout
        << "======================================\n";
}

void SettingsManager::ConfigureDateTime(
    Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        std::cout
            << "\n========== DATE / TIME FORMAT ==========\n";
        std::cout
            << "Current Date Format: "
            << company.GetDateFormat()
            << "\n";
        std::cout
            << "Current Time Format: "
            << company.GetTimeFormat()
            << "\n\n";
        std::cout << "1. Change Date Format\n";
        std::cout << "2. Change Time Format\n";
        std::cout << "0. Return to Settings\n";
        std::cout << "========================================\n";

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                2
            );

        if (choice == 1)
        {
            std::cout
                << "\n1. MM/DD/YYYY   Example: 10/22/2026\n";
            std::cout
                << "2. YYYY-MM-DD   Example: 2026-10-22\n";

            int dateChoice =
                InputManager::ReadMenuChoice(
                    "Select date format: ",
                    1,
                    2
                );

            company.SetDateFormat(
                dateChoice == 1
                ? "MM/DD/YYYY"
                : "YYYY-MM-DD"
            );

            ApplyDateTimeSettings(
                company
            );

            std::cout
                << "Date format updated to "
                << company.GetDateFormat()
                << ".\n";
        }
        else if (choice == 2)
        {
            std::cout
                << "\n1. 12-hour AM/PM   Example: 8:30 PM\n";
            std::cout
                << "2. 24-hour          Example: 20:30\n";

            int timeChoice =
                InputManager::ReadMenuChoice(
                    "Select time format: ",
                    1,
                    2
                );

            company.SetTimeFormat(
                timeChoice == 1
                ? "12-hour"
                : "24-hour"
            );

            ApplyDateTimeSettings(
                company
            );

            std::cout
                << "Time format updated to "
                << company.GetTimeFormat()
                << ".\n";
        }
    }
}

long long SettingsManager::ReadDeadline()
{
    while (true)
    {
        std::cout
            << "\nPreferred Date Format: "
            << HOSManager::GetDateFormat()
            << "\n";

        std::cout
            << "Preferred Time Format: "
            << HOSManager::GetTimeFormat()
            << "\n";

        std::string dateText =
            InputManager::ReadRequiredText(
                "Due date: ",
                "Due date"
            );

        std::string timeText =
            InputManager::ReadRequiredText(
                "Due time: ",
                "Due time"
            );

        long long deadlineEpoch = 0;

        if (!HOSManager::ParseDateTime(
            dateText,
            timeText,
            deadlineEpoch))
        {
            std::cout
                << "Invalid date or time. Try "
                << HOSManager::GetDateInputExample()
                << " with "
                << HOSManager::GetTimeInputExample()
                << ".\n";

            continue;
        }

        if (deadlineEpoch <=
            HOSManager::GetCurrentEpoch())
        {
            std::cout
                << "The delivery deadline must be in the future.\n";

            continue;
        }

        return deadlineEpoch;
    }
}

void SettingsManager::EditTruck(
    Company& company)
{
    if (company.GetTrucks().empty())
    {
        std::cout
            << "\nNo trucks have been added yet.\n";
        return;
    }

    std::cout
        << "\n========== EDIT TRUCK ==========\n";

    for (const Truck& truck : company.GetTrucks())
    {
        std::cout
            << "Truck #"
            << truck.GetUnitNumber()
            << " | Status: "
            << truck.GetStatus()
            << " | Mileage: "
            << truck.GetMileage()
            << "\n";
    }

    int unitNumber =
        InputManager::ReadPositiveInt(
            "Enter truck unit number to edit: ",
            "truck unit number"
        );

    Truck* selectedTruck = nullptr;

    for (Truck& truck : company.GetTrucks())
    {
        if (truck.GetUnitNumber() == unitNumber)
        {
            selectedTruck = &truck;
            break;
        }
    }

    if (selectedTruck == nullptr)
    {
        std::cout
            << "Truck #"
            << unitNumber
            << " does not exist.\n";
        return;
    }

    if (selectedTruck->GetStatus() != "Available")
    {
        std::cout
            << "Truck #"
            << unitNumber
            << " is currently "
            << selectedTruck->GetStatus()
            << " and cannot be edited until it is Available.\n";
        return;
    }

    int choice = -1;

    while (choice != 0)
    {
        std::cout
            << "\nEditing Truck #"
            << selectedTruck->GetUnitNumber()
            << "\n";
        std::cout << "1. Unit Number\n";
        std::cout << "2. Mileage\n";
        std::cout << "3. Fuel Tank Capacity\n";
        std::cout << "4. Fuel Economy (MPG)\n";
        std::cout << "5. Current Fuel Gallons\n";
        std::cout << "0. Return to Settings\n";

        choice =
            InputManager::ReadMenuChoice(
                "Select field to edit: ",
                0,
                5
            );

        if (choice == 1)
        {
            int oldUnitNumber =
                selectedTruck->GetUnitNumber();

            int newUnitNumber =
                InputManager::ReadPositiveInt(
                    "Enter new unit number: ",
                    "unit number"
                );

            if (company.ChangeTruckUnitNumber(
                oldUnitNumber,
                newUnitNumber))
            {
                std::cout
                    << "Truck unit number updated to #"
                    << newUnitNumber
                    << ". Existing load and financial references were updated.\n";
            }
            else
            {
                std::cout
                    << "Unable to change the unit number. "
                    << "The new number may already be in use.\n";
            }
        }
        else if (choice == 2)
        {
            double newMileage =
                InputManager::ReadNonNegativeDouble(
                    "Enter corrected mileage: ",
                    "mileage"
                );

            double correctedLastService =
                std::min(
                    selectedTruck->GetLastServiceMileage(),
                    newMileage
                );

            selectedTruck->SetMileage(
                newMileage
            );

            selectedTruck->RestoreMaintenanceSchedule(
                correctedLastService,
                selectedTruck->GetServiceIntervalMiles()
            );

            std::cout
                << "Truck mileage updated.\n";
        }
        else if (choice == 3)
        {
            double newCapacity =
                InputManager::ReadPositiveDouble(
                    "Enter corrected tank capacity: ",
                    "fuel tank capacity"
                );

            double currentFuel =
                std::min(
                    selectedTruck->GetFuelGallons(),
                    newCapacity
                );

            selectedTruck->RestoreFuelState(
                newCapacity,
                currentFuel,
                selectedTruck->GetMilesPerGallon()
            );

            std::cout
                << "Fuel tank capacity updated.\n";
        }
        else if (choice == 4)
        {
            double newMpg =
                InputManager::ReadPositiveDouble(
                    "Enter corrected MPG: ",
                    "fuel economy"
                );

            selectedTruck->RestoreFuelState(
                selectedTruck->GetFuelTankCapacity(),
                selectedTruck->GetFuelGallons(),
                newMpg
            );

            std::cout
                << "Fuel economy updated.\n";
        }
        else if (choice == 5)
        {
            while (true)
            {
                double newFuel =
                    InputManager::ReadNonNegativeDouble(
                        "Enter corrected current fuel gallons: ",
                        "fuel amount"
                    );

                if (newFuel <=
                    selectedTruck->GetFuelTankCapacity())
                {
                    selectedTruck->RestoreFuelState(
                        selectedTruck->GetFuelTankCapacity(),
                        newFuel,
                        selectedTruck->GetMilesPerGallon()
                    );

                    std::cout
                        << "Current fuel level updated.\n";
                    break;
                }

                std::cout
                    << "Fuel cannot exceed the truck's tank capacity of "
                    << selectedTruck->GetFuelTankCapacity()
                    << " gallons.\n";
            }
        }
    }
}

void SettingsManager::EditDriver(
    Company& company)
{
    if (company.GetDrivers().empty())
    {
        std::cout
            << "\nNo drivers have been added yet.\n";
        return;
    }

    std::cout
        << "\n========== EDIT DRIVER ==========\n";

    for (const Driver& driver : company.GetDrivers())
    {
        std::cout
            << "Driver #"
            << driver.GetDriverID()
            << " | "
            << driver.GetName()
            << " | Status: "
            << driver.GetStatus()
            << "\n";
    }

    int driverID =
        InputManager::ReadPositiveInt(
            "Enter driver ID to edit: ",
            "driver ID"
        );

    Driver* selectedDriver = nullptr;

    for (Driver& driver : company.GetDrivers())
    {
        if (driver.GetDriverID() == driverID)
        {
            selectedDriver = &driver;
            break;
        }
    }

    if (selectedDriver == nullptr)
    {
        std::cout
            << "Driver #"
            << driverID
            << " does not exist.\n";
        return;
    }

    if (selectedDriver->GetStatus() != "Available")
    {
        std::cout
            << "Driver #"
            << driverID
            << " is currently "
            << selectedDriver->GetStatus()
            << " and cannot be edited until Available.\n";
        return;
    }

    int choice = -1;

    while (choice != 0)
    {
        std::cout
            << "\nEditing Driver #"
            << selectedDriver->GetDriverID()
            << " - "
            << selectedDriver->GetName()
            << "\n";
        std::cout << "1. Driver ID\n";
        std::cout << "2. Driver Name\n";
        std::cout << "0. Return to Settings\n";

        choice =
            InputManager::ReadMenuChoice(
                "Select field to edit: ",
                0,
                2
            );

        if (choice == 1)
        {
            int oldDriverID =
                selectedDriver->GetDriverID();

            int newDriverID =
                InputManager::ReadPositiveInt(
                    "Enter new Driver ID: ",
                    "driver ID"
                );

            if (company.ChangeDriverID(
                oldDriverID,
                newDriverID))
            {
                std::cout
                    << "Driver ID updated to #"
                    << newDriverID
                    << ". Existing load and financial references were updated.\n";
            }
            else
            {
                std::cout
                    << "Unable to change the Driver ID. "
                    << "The new ID may already be in use.\n";
            }
        }
        else if (choice == 2)
        {
            std::string newName =
                InputManager::ReadRequiredText(
                    "Enter corrected driver name: ",
                    "Driver name"
                );

            selectedDriver->SetName(
                newName
            );

            std::cout
                << "Driver name updated.\n";
        }
    }
}

void SettingsManager::EditLoad(
    Company& company)
{
    if (company.GetLoads().empty())
    {
        std::cout
            << "\nNo loads have been added yet.\n";
        return;
    }

    std::cout
        << "\n========== EDIT LOAD ==========\n";

    for (const Load& load : company.GetLoads())
    {
        std::cout
            << "Load #"
            << load.GetLoadNumber()
            << " | "
            << load.GetOrigin()
            << " -> "
            << load.GetDestination()
            << " | Status: "
            << load.GetStatus()
            << "\n";
    }

    int loadNumber =
        InputManager::ReadPositiveInt(
            "Enter load number to edit: ",
            "load number"
        );

    Load* selectedLoad = nullptr;

    for (Load& load : company.GetLoads())
    {
        if (load.GetLoadNumber() == loadNumber)
        {
            selectedLoad = &load;
            break;
        }
    }

    if (selectedLoad == nullptr)
    {
        std::cout
            << "Load #"
            << loadNumber
            << " does not exist.\n";
        return;
    }

    if (selectedLoad->GetStatus() != "Available")
    {
        std::cout
            << "Load #"
            << loadNumber
            << " is "
            << selectedLoad->GetStatus()
            << ". Only Available loads can be edited.\n";
        std::cout
            << "This protects mileage, HOS, fuel, revenue, and transaction history from becoming inconsistent.\n";
        return;
    }

    int choice = -1;

    while (choice != 0)
    {
        std::cout
            << "\nEditing Load #"
            << selectedLoad->GetLoadNumber()
            << "\n";
        std::cout << "1. Load Number\n";
        std::cout << "2. Origin\n";
        std::cout << "3. Destination\n";
        std::cout << "4. Distance\n";
        std::cout << "5. Pay\n";
        std::cout << "6. Delivery Deadline\n";
        std::cout << "0. Return to Settings\n";

        choice =
            InputManager::ReadMenuChoice(
                "Select field to edit: ",
                0,
                6
            );

        if (choice == 1)
        {
            int oldLoadNumber =
                selectedLoad->GetLoadNumber();

            int newLoadNumber =
                InputManager::ReadPositiveInt(
                    "Enter new load number: ",
                    "load number"
                );

            if (company.ChangeLoadNumber(
                oldLoadNumber,
                newLoadNumber))
            {
                std::cout
                    << "Load number updated to #"
                    << newLoadNumber
                    << ".\n";
            }
            else
            {
                std::cout
                    << "Unable to change the load number. "
                    << "The new number may already be in use.\n";
            }
        }
        else if (choice == 2)
        {
            selectedLoad->SetOrigin(
                InputManager::ReadRequiredText(
                    "Enter corrected origin: ",
                    "Origin"
                )
            );

            std::cout
                << "Load origin updated.\n";
        }
        else if (choice == 3)
        {
            selectedLoad->SetDestination(
                InputManager::ReadRequiredText(
                    "Enter corrected destination: ",
                    "Destination"
                )
            );

            std::cout
                << "Load destination updated.\n";
        }
        else if (choice == 4)
        {
            selectedLoad->SetDistanceMiles(
                InputManager::ReadPositiveDouble(
                    "Enter corrected distance in miles: ",
                    "load distance"
                )
            );

            std::cout
                << "Load distance updated.\n";
        }
        else if (choice == 5)
        {
            selectedLoad->SetPay(
                InputManager::ReadCurrency(
                    "Enter corrected load pay: $"
                )
            );

            std::cout
                << "Load pay updated.\n";
        }
        else if (choice == 6)
        {
            selectedLoad->SetDeliveryDeadlineEpoch(
                ReadDeadline()
            );

            std::cout
                << "Delivery deadline updated to "
                << HOSManager::FormatDateTime(
                    selectedLoad->GetDeliveryDeadlineEpoch()
                )
                << ".\n";
        }
    }
}

void SettingsManager::ResetDisplaySettings(
    Company& company)
{
    company.SetDateFormat(
        "MM/DD/YYYY"
    );

    company.SetTimeFormat(
        "12-hour"
    );

    ApplyDateTimeSettings(
        company
    );

    std::cout
        << "\nDisplay settings reset to MM/DD/YYYY and 12-hour AM/PM time.\n";
}

void SettingsManager::Run(
    Company& company)
{
    ApplyDateTimeSettings(
        company
    );

    int choice = -1;

    while (choice != 0)
    {
        ShowMenu();

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                6
            );

        switch (choice)
        {
        case 1:
            ConfigureDateTime(
                company
            );
            break;

        case 2:
            ShowCurrentSettings(
                company
            );
            break;

        case 3:
            EditTruck(
                company
            );
            break;

        case 4:
            EditDriver(
                company
            );
            break;

        case 5:
            EditLoad(
                company
            );
            break;

        case 6:
            ResetDisplaySettings(
                company
            );
            break;

        case 0:
            break;
        }
    }
}
