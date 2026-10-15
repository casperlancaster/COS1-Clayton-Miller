# Milestone 3 - TruckzSim

**Student:** Clayton Miller  
**Course:** COS119 - Project & Portfolio I  
**Project:** TruckzSim  
**Milestone:** 3

## Overview

During Milestone 3, I focused heavily on refactoring TruckzSim while continuing to add new features. The project had grown significantly during the first two milestones, and much of the program logic was still located inside TruckzSim.cpp.

My main goal this week was to improve the structure of the program by separating different responsibilities into dedicated classes. I also expanded several simulation systems, including truck mileage, maintenance, company finances, and company setup.

The project continues to run as a console-based trucking management simulation, but the internal structure is now much easier to maintain and expand.

## Features Added

### Duplicate Identifier Protection

I added validation to prevent duplicate truck unit numbers, driver IDs, and load numbers.

Before this change, it was possible to create multiple trucks, drivers, or loads using the same identifier. This could create problems when the dispatch system attempted to locate a specific resource.

The program now checks existing company data before accepting a new identifier and asks the player to enter another value if a duplicate is found.

### Truck Mileage Tracking

Loads now include a distance in miles.

When a load is completed, the distance of the load is added to the mileage of the assigned truck.

For example, if a truck begins with 47 miles and completes a 2,790-mile load, the truck's new mileage becomes 2,837 miles.

This creates a connection between the Freight, Dispatch, Truck, and Maintenance systems.

### Custom Company Name

Players can now enter the name of their trucking company when the program starts.

The company name is stored in the Company object and is displayed throughout parts of the program.

This makes the simulation feel more personalized and gives the Company class another meaningful responsibility.

### Truck Maintenance System

I added a Maintenance system that allows the player to view truck maintenance information, send trucks to maintenance, and complete maintenance services.

Each truck has a service interval of 10,000 miles.

The system tracks:

- Current truck mileage
- Last service mileage
- Next service mileage
- Miles remaining until service
- Whether maintenance is currently due
- Truck availability status

When a truck is sent to maintenance, its status changes so it cannot be used by the Dispatch system.

Completing maintenance returns the truck to Available status and records a $500 maintenance expense for the company.

### Company Banking System

I added a Banking system that tracks the financial state of the trucking company.

The Banking system can display:

- Company revenue
- Company expenses
- Current company balance

Revenue is currently earned by completing loads.

Expenses are currently created by maintenance services.

The company balance is calculated using:

`Balance = Revenue - Expenses`

This creates a foundation for future operating expenses such as fuel, repairs, driver pay, and other company costs.

## System Design Updates

### InputManager

Input validation was moved into a dedicated InputManager class.

Instead of repeating input checking throughout different menus, the program can now use reusable input functions for integers, positive values, currency, text, and other input types.

This reduces duplicated code and makes input handling more consistent throughout the project.

### FleetManager

Fleet-related menu logic was moved out of TruckzSim.cpp and into FleetManager.

FleetManager is responsible for operations related to adding and viewing trucks.

This gives the fleet system a clearer responsibility and keeps truck-management code separate from the main application flow.

### DriverManager

Driver-management functions were moved into DriverManager.

This class handles operations such as adding and displaying drivers.

Separating this logic makes the program easier to maintain as the Driver system continues to grow.

### FreightManager

Freight Market functionality was moved into FreightManager.

The FreightManager handles load creation and load information, including the new load-distance value used for mileage tracking.

This keeps freight-related operations separate from the main program.

### DispatchCenter

The user interface and menu logic for Dispatch were moved into a new DispatchCenter class.

DispatchCenter handles player interaction with the Dispatch system, while DispatchManager continues to handle the underlying dispatch logic.

This separates the presentation and control flow from the business logic of assigning and completing loads.

### TruckzSimApp

A TruckzSimApp class was created to coordinate the overall application.

Before this refactor, TruckzSim.cpp contained the main menu, management functions, input handling, and much of the program's control flow.

TruckzSim.cpp originally contained approximately 990 lines of code.

After the refactoring, TruckzSim.cpp now contains only the program entry point and starts the TruckzSimApp.

The main file is now approximately:

```cpp
#include "TruckzSimApp.h"

int main()
{
    TruckzSimApp app;
    app.Run();

    return 0;
}