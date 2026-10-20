#include "ConsoleTheme.h"
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
    std::cout << ConsoleTheme::FormatLiteral("\n");
    std::cout << ConsoleTheme::FormatLiteral("========== SETTINGS ==========\n");
    std::cout << ConsoleTheme::FormatLiteral("1. Date / Time Format\n");
    std::cout << ConsoleTheme::FormatLiteral("2. View Current Settings\n");
    std::cout << ConsoleTheme::FormatLiteral("3. Edit Truck\n");
    std::cout << ConsoleTheme::FormatLiteral("4. Edit Driver\n");
    std::cout << ConsoleTheme::FormatLiteral("5. Edit Load\n");
    std::cout << ConsoleTheme::FormatLiteral("6. Reset Display Settings\n");
    std::cout << ConsoleTheme::FormatLiteral("0. Return to Main Menu\n");
    std::cout << ConsoleTheme::FormatLiteral("==============================\n");
}

void SettingsManager::ShowCurrentSettings(
    const Company& company)
{
    std::cout
        << ConsoleTheme::FormatLiteral("\n========== CURRENT SETTINGS ==========\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Date Format: ")
        << company.GetDateFormat()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Time Format: ")
        << company.GetTimeFormat()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Date Entry Example: ")
        << HOSManager::GetDateInputExample()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Time Entry Example: ")
        << HOSManager::GetTimeInputExample()
        << ConsoleTheme::FormatLiteral("\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Flexible entry is enabled. Dates may use ")
        << ConsoleTheme::FormatLiteral("slashes, dashes, spaces, or numbers only.\n");

    std::cout
        << ConsoleTheme::FormatLiteral("Time colons are optional. AM/PM is accepted, ")
        << ConsoleTheme::FormatLiteral("and 24-hour time is also accepted.\n");

    std::cout
        << ConsoleTheme::FormatLiteral("======================================\n");
}

void SettingsManager::ConfigureDateTime(
    Company& company)
{
    int choice = -1;

    while (choice != 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\n========== DATE / TIME FORMAT ==========\n");
        std::cout
            << ConsoleTheme::FormatLiteral("Current Date Format: ")
            << company.GetDateFormat()
            << ConsoleTheme::FormatLiteral("\n");
        std::cout
            << ConsoleTheme::FormatLiteral("Current Time Format: ")
            << company.GetTimeFormat()
            << ConsoleTheme::FormatLiteral("\n\n");
        std::cout << ConsoleTheme::FormatLiteral("1. Change Date Format\n");
        std::cout << ConsoleTheme::FormatLiteral("2. Change Time Format\n");
        std::cout << ConsoleTheme::FormatLiteral("0. Return to Settings\n");
        std::cout << ConsoleTheme::FormatLiteral("========================================\n");

        choice =
            InputManager::ReadMenuChoice(
                "Select an option: ",
                0,
                2
            );

        if (choice == 1)
        {
            std::cout
                << ConsoleTheme::FormatLiteral("\n1. MM/DD/YYYY   Example: 10/22/2026\n");
            std::cout
                << ConsoleTheme::FormatLiteral("2. YYYY-MM-DD   Example: 2026-10-22\n");

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
                << ConsoleTheme::FormatLiteral("Date format updated to ")
                << company.GetDateFormat()
                << ConsoleTheme::FormatLiteral(".\n");
        }
        else if (choice == 2)
        {
            std::cout
                << ConsoleTheme::FormatLiteral("\n1. 12-hour AM/PM   Example: 8:30 PM\n");
            std::cout
                << ConsoleTheme::FormatLiteral("2. 24-hour          Example: 20:30\n");

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
                << ConsoleTheme::FormatLiteral("Time format updated to ")
                << company.GetTimeFormat()
                << ConsoleTheme::FormatLiteral(".\n");
        }
    }
}

long long SettingsManager::ReadDeadline()
{
    while (true)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nPreferred Date Format: ")
            << HOSManager::GetDateFormat()
            << ConsoleTheme::FormatLiteral("\n");

        std::cout
            << ConsoleTheme::FormatLiteral("Preferred Time Format: ")
            << HOSManager::GetTimeFormat()
            << ConsoleTheme::FormatLiteral("\n");

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
                << ConsoleTheme::FormatLiteral("Invalid date or time. Try ")
                << HOSManager::GetDateInputExample()
                << ConsoleTheme::FormatLiteral(" with ")
                << HOSManager::GetTimeInputExample()
                << ConsoleTheme::FormatLiteral(".\n");

            continue;
        }

        if (deadlineEpoch <=
            HOSManager::GetCurrentEpoch())
        {
            std::cout
                << ConsoleTheme::FormatLiteral("The delivery deadline must be in the future.\n");

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
            << ConsoleTheme::FormatLiteral("\nNo trucks have been added yet.\n");
        return;
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== EDIT TRUCK ==========\n");

    for (const Truck& truck : company.GetTrucks())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Truck #")
            << truck.GetUnitNumber()
            << ConsoleTheme::FormatLiteral(" | Status: ")
            << ConsoleTheme::Status(truck.GetStatus())
            << ConsoleTheme::FormatLiteral(" | Mileage: ")
            << truck.GetMileage()
            << ConsoleTheme::FormatLiteral(" | Location: ")
            << truck.GetCurrentLocation()
            << ConsoleTheme::FormatLiteral("\n");
    }

    int unitNumber =
        InputManager::ReadPositiveInt(
            "Enter truck unit number to edit: ",
            "truck unit number"
        );

    Truck* selectedTruck =
        company.FindTruck(
            unitNumber
        );

    if (selectedTruck == nullptr)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Truck #")
            << unitNumber
            << ConsoleTheme::FormatLiteral(" does not exist.\n");
        return;
    }

    if (selectedTruck->GetStatus() != "Available")
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Truck #")
            << unitNumber
            << ConsoleTheme::FormatLiteral(" is currently ")
            << ConsoleTheme::Status(selectedTruck->GetStatus())
            << ConsoleTheme::FormatLiteral(" and cannot be edited until it is Available.\n");
        return;
    }

    int choice = -1;

    while (choice != 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nEditing Truck #")
            << selectedTruck->GetUnitNumber()
            << ConsoleTheme::FormatLiteral("\n");
        std::cout << ConsoleTheme::FormatLiteral("1. Unit Number\n");
        std::cout << ConsoleTheme::FormatLiteral("2. Mileage\n");
        std::cout << ConsoleTheme::FormatLiteral("3. Fuel Tank Capacity\n");
        std::cout << ConsoleTheme::FormatLiteral("4. Fuel Economy (MPG)\n");
        std::cout << ConsoleTheme::FormatLiteral("5. Current Fuel Gallons\n");
        std::cout << ConsoleTheme::FormatLiteral("6. Current Location\n");
        std::cout << ConsoleTheme::FormatLiteral("0. Return to Settings\n");

        choice =
            InputManager::ReadMenuChoice(
                "Select field to edit: ",
                0,
                6
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

            std::cout
                << ConsoleTheme::FormatLiteral("\nTruck Unit Number Change\n")
                << ConsoleTheme::FormatLiteral("Current: #")
                << oldUnitNumber
                << ConsoleTheme::FormatLiteral("\n")
                << ConsoleTheme::FormatLiteral("New: #")
                << newUnitNumber
                << ConsoleTheme::FormatLiteral("\n")
                << ConsoleTheme::FormatLiteral("Existing load and financial references ")
                << ConsoleTheme::FormatLiteral("will also be updated.\n");

            if (!InputManager::ReadYesNo(
                "Confirm unit number change? (Y/N): "))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Truck unit number change cancelled.\n");

                continue;
            }

            if (company.ChangeTruckUnitNumber(
                oldUnitNumber,
                newUnitNumber))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Truck unit number updated to #")
                    << newUnitNumber
                    << ConsoleTheme::FormatLiteral(". Existing load and financial references were updated.\n");
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Unable to change the unit number. ")
                    << ConsoleTheme::FormatLiteral("The new number may already be in use.\n");
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
                << ConsoleTheme::FormatLiteral("Truck mileage updated.\n");
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
                << ConsoleTheme::FormatLiteral("Fuel tank capacity updated.\n");
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
                << ConsoleTheme::FormatLiteral("Fuel economy updated.\n");
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
                        << ConsoleTheme::FormatLiteral("Current fuel level updated.\n");
                    break;
                }

                std::cout
                    << ConsoleTheme::FormatLiteral("Fuel cannot exceed the truck's tank capacity of ")
                    << selectedTruck->GetFuelTankCapacity()
                    << ConsoleTheme::FormatLiteral(" gallons.\n");
            }
        }
        else if (choice == 6)
        {
            std::string newLocation =
                InputManager::ReadRequiredText(
                    "Enter corrected current location: ",
                    "current location"
                );

            selectedTruck->SetCurrentLocation(
                newLocation
            );

            std::cout
                << ConsoleTheme::FormatLiteral("Truck location updated to ")
                << selectedTruck->GetCurrentLocation()
                << ConsoleTheme::FormatLiteral(".\n");
        }
    }
}

void SettingsManager::EditDriver(
    Company& company)
{
    if (company.GetDrivers().empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nNo drivers have been added yet.\n");
        return;
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== EDIT DRIVER ==========\n");

    for (const Driver& driver : company.GetDrivers())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver #")
            << driver.GetDriverID()
            << ConsoleTheme::FormatLiteral(" | ")
            << driver.GetName()
            << ConsoleTheme::FormatLiteral(" | Status: ")
            << ConsoleTheme::Status(driver.GetStatus())
            << ConsoleTheme::FormatLiteral("\n");
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
            << ConsoleTheme::FormatLiteral("Driver #")
            << driverID
            << ConsoleTheme::FormatLiteral(" does not exist.\n");
        return;
    }

    if (selectedDriver->GetStatus() != "Available")
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Driver #")
            << driverID
            << ConsoleTheme::FormatLiteral(" is currently ")
            << ConsoleTheme::Status(selectedDriver->GetStatus())
            << ConsoleTheme::FormatLiteral(" and cannot be edited until Available.\n");
        return;
    }

    int choice = -1;

    while (choice != 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nEditing Driver #")
            << selectedDriver->GetDriverID()
            << ConsoleTheme::FormatLiteral(" - ")
            << selectedDriver->GetName()
            << ConsoleTheme::FormatLiteral("\n");
        std::cout << ConsoleTheme::FormatLiteral("1. Driver ID\n");
        std::cout << ConsoleTheme::FormatLiteral("2. Driver Name\n");
        std::cout << ConsoleTheme::FormatLiteral("0. Return to Settings\n");

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

            std::cout
                << ConsoleTheme::FormatLiteral("\nDriver ID Change\n")
                << ConsoleTheme::FormatLiteral("Current: #")
                << oldDriverID
                << ConsoleTheme::FormatLiteral("\n")
                << ConsoleTheme::FormatLiteral("New: #")
                << newDriverID
                << ConsoleTheme::FormatLiteral("\n")
                << ConsoleTheme::FormatLiteral("Existing load and financial references ")
                << ConsoleTheme::FormatLiteral("will also be updated.\n");

            if (!InputManager::ReadYesNo(
                "Confirm Driver ID change? (Y/N): "))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Driver ID change cancelled.\n");

                continue;
            }

            if (company.ChangeDriverID(
                oldDriverID,
                newDriverID))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Driver ID updated to #")
                    << newDriverID
                    << ConsoleTheme::FormatLiteral(". Existing load and financial references were updated.\n");
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Unable to change the Driver ID. ")
                    << ConsoleTheme::FormatLiteral("The new ID may already be in use.\n");
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
                << ConsoleTheme::FormatLiteral("Driver name updated.\n");
        }
    }
}

void SettingsManager::EditLoad(
    Company& company)
{
    if (company.GetLoads().empty())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nNo loads have been added yet.\n");
        return;
    }

    std::cout
        << ConsoleTheme::FormatLiteral("\n========== EDIT LOAD ==========\n");

    for (const Load& load : company.GetLoads())
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Load #")
            << load.GetLoadNumber()
            << ConsoleTheme::FormatLiteral(" | ")
            << load.GetOrigin()
            << ConsoleTheme::FormatLiteral(" -> ")
            << load.GetDestination()
            << ConsoleTheme::FormatLiteral(" | Status: ")
            << ConsoleTheme::Status(load.GetStatus())
            << ConsoleTheme::FormatLiteral("\n");
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
            << ConsoleTheme::FormatLiteral("Load #")
            << loadNumber
            << ConsoleTheme::FormatLiteral(" does not exist.\n");
        return;
    }

    if (selectedLoad->GetStatus() != "Available")
    {
        std::cout
            << ConsoleTheme::FormatLiteral("Load #")
            << loadNumber
            << ConsoleTheme::FormatLiteral(" is ")
            << ConsoleTheme::Status(selectedLoad->GetStatus())
            << ConsoleTheme::FormatLiteral(". Only Available loads can be edited.\n");
        std::cout
            << ConsoleTheme::FormatLiteral("This protects mileage, HOS, fuel, revenue, and transaction history from becoming inconsistent.\n");
        return;
    }

    int choice = -1;

    while (choice != 0)
    {
        std::cout
            << ConsoleTheme::FormatLiteral("\nEditing Load #")
            << selectedLoad->GetLoadNumber()
            << ConsoleTheme::FormatLiteral("\n");
        std::cout << ConsoleTheme::FormatLiteral("1. Load Number\n");
        std::cout << ConsoleTheme::FormatLiteral("2. Origin\n");
        std::cout << ConsoleTheme::FormatLiteral("3. Destination\n");
        std::cout << ConsoleTheme::FormatLiteral("4. Distance\n");
        std::cout << ConsoleTheme::FormatLiteral("5. Pay\n");
        std::cout << ConsoleTheme::FormatLiteral("6. Delivery Deadline\n");
        std::cout << ConsoleTheme::FormatLiteral("0. Return to Settings\n");

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

            std::cout
                << ConsoleTheme::FormatLiteral("\nLoad Number Change\n")
                << ConsoleTheme::FormatLiteral("Current: #")
                << oldLoadNumber
                << ConsoleTheme::FormatLiteral("\n")
                << ConsoleTheme::FormatLiteral("New: #")
                << newLoadNumber
                << ConsoleTheme::FormatLiteral("\n")
                << ConsoleTheme::FormatLiteral("Existing financial references will also ")
                << ConsoleTheme::FormatLiteral("be updated.\n");

            if (!InputManager::ReadYesNo(
                "Confirm load number change? (Y/N): "))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Load number change cancelled.\n");

                continue;
            }

            if (company.ChangeLoadNumber(
                oldLoadNumber,
                newLoadNumber))
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Load number updated to #")
                    << newLoadNumber
                    << ConsoleTheme::FormatLiteral(".\n");
            }
            else
            {
                std::cout
                    << ConsoleTheme::FormatLiteral("Unable to change the load number. ")
                    << ConsoleTheme::FormatLiteral("The new number may already be in use.\n");
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
                << ConsoleTheme::FormatLiteral("Load origin updated.\n");
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
                << ConsoleTheme::FormatLiteral("Load destination updated.\n");
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
                << ConsoleTheme::FormatLiteral("Load distance updated.\n");
        }
        else if (choice == 5)
        {
            selectedLoad->SetPay(
                InputManager::ReadCurrency(
                    "Enter corrected load pay: $"
                )
            );

            std::cout
                << ConsoleTheme::FormatLiteral("Load pay updated.\n");
        }
        else if (choice == 6)
        {
            selectedLoad->SetDeliveryDeadlineEpoch(
                ReadDeadline()
            );

            std::cout
                << ConsoleTheme::FormatLiteral("Delivery deadline updated to ")
                << HOSManager::FormatDateTime(
                    selectedLoad->GetDeliveryDeadlineEpoch()
                )
                << ConsoleTheme::FormatLiteral(".\n");
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
        << ConsoleTheme::FormatLiteral("\nDisplay settings reset to MM/DD/YYYY and 12-hour AM/PM time.\n");
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
