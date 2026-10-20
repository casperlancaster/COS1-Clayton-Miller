# Milestone 4

## Final Project Development

Milestone 4 focused on completing TruckzSim, connecting the major systems together, improving usability, resolving remaining bugs, and performing final testing across the entire application.

TruckzSim has grown from a basic console application into a multi-class trucking company management simulation with persistent company data, dispatching, Hours of Service, fuel management, maintenance, breakdowns, financial tracking, driver payroll, and other management systems.

---

## Final Features Added

### Company Save and Load

TruckzSim now supports multiple saved companies.

Players can create a new company or select an existing saved company from the startup menu. Company information, trucks, drivers, loads, financial history, settings, truck locations, Hours of Service data, fuel data, maintenance information, and breakdown information are preserved between sessions.

Older save versions remain compatible with the final version of the program.

---

### Fuel Management

Each truck now tracks:

- Fuel tank capacity
- Current fuel level
- Fuel economy
- Estimated fuel range
- Required delivery fuel reserve

TruckzSim calculates the amount of fuel required for a load before dispatch.

If additional fuel is required during the trip, the required fuel purchase is calculated and recorded as a company expense when the load is completed.

Manual refueling is also available through the Fuel Management menu.

---

### Driver Hours of Service

Drivers now track:

- Remaining driving hours
- Accumulated rest time
- Scheduled availability

TruckzSim calculates whether a driver can legally complete a load before its delivery deadline.

Solo trips automatically account for required rest periods.

If a solo driver cannot legally meet the deadline, TruckzSim can evaluate a two-driver team operation.

Team drivers alternate driving and resting while the truck continues moving.

---

### Driver Rest Management

Driver Management now allows the player to manually manage driver rest.

A single available driver can:

- Rest until a full Hours of Service reset
- Advance a selected amount of off-duty time

The player can also rest all eligible available drivers until their Hours of Service resets.

Drivers assigned to active loads cannot be manually rested.

---

### Truck Breakdown and Repair System

Trucks can experience random mechanical failures after completing loads.

Breakdowns include several possible repair types with different:

- Parts costs
- Labor costs
- Service call charges
- Repair times

A truck experiencing a breakdown becomes unavailable for dispatch.

The Breakdown / Repair Center allows the player to review active breakdowns and repair trucks before returning them to service.

---

### Financial Transaction History

TruckzSim now records individual company financial transactions instead of only storing total revenue and expenses.

Transactions include:

- Load revenue
- Fuel purchases
- Maintenance expenses
- Repair expenses
- Driver payroll

Banking allows the player to view:

- Account summary
- Income history
- Expense history
- All transactions
- Driver payroll history

Transactions include identifying information such as truck numbers, load numbers, and driver IDs when applicable.

---

### Settings and Data Correction

A Settings menu was added to improve usability and allow corrections to user-entered information.

The player can change the preferred:

- Date format
- Time format

TruckzSim supports both:

- MM/DD/YYYY
- YYYY-MM-DD

Time can be displayed using either:

- 12-hour time
- 24-hour time

Input was also made more flexible so dates and times can be entered using multiple formats.

The Settings menu also allows the player to correct information for available trucks, drivers, and loads.

Important ID changes automatically update related references.

---

### System Validation

A System Validation feature was added to check the current company for inconsistent or invalid data.

Validation checks include:

- Duplicate IDs
- Invalid truck information
- Invalid driver information
- Invalid Hours of Service information
- Invalid load assignments
- Missing truck or driver references
- Invalid financial transactions
- Revenue and expense totals
- Breakdown information
- Fuel information
- Truck locations

The validator can be run manually and is also used during important save and load operations.

---

### Truck Location Tracking

Every truck now stores its current location.

A truck begins at the location entered when it is added to the fleet.

When a load is completed, the truck automatically moves to the destination of that load.

Truck locations are displayed in Fleet Management and the Company Overview and are preserved through save and load.

---

### Company Overview Dashboard

The Company Overview was expanded into a more useful management dashboard.

The dashboard displays information about:

- Total trucks
- Available trucks
- Dispatched trucks
- Trucks in maintenance
- Truck breakdowns
- Maintenance due
- Total drivers
- Available drivers
- Dispatched drivers
- Available loads
- Assigned loads
- Completed loads
- Company revenue
- Company expenses
- Company balance
- Truck locations

This provides the player with a quick view of the current condition of the company.

---

### Confirmation and Safety Prompts

Confirmation prompts were added before important actions that modify company data.

Confirmations are used for actions such as:

- Dispatching a load
- Completing a load
- Purchasing fuel
- Performing maintenance
- Repairing a truck
- Changing important IDs
- Scheduling driver rest
- Exiting the application

Cancelling an action leaves the related company data unchanged.

---

### Driver Payroll

Driver payroll was added as the final financial system for the school version of TruckzSim.

Solo drivers earn:

$0.60 per mile

Team drivers each earn 70 percent of the solo rate:

$0.42 per mile per driver

Payroll is processed only after a load is successfully completed.

Each driver payment is recorded as an individual company expense transaction.

Driver payroll is allowed even if the payment causes the company balance to become negative.

---

## Refactoring Improvements

Several areas of the program were refactored during the final milestone.

Object access was improved by using references and pointers to access trucks, drivers, and loads stored inside the Company object.

Reusable lookup functions were added for finding:

- Trucks
- Drivers
- Loads

Input handling was centralized through InputManager.

Menu input now uses consistent range validation.

Repeated financial operations were consolidated through the transaction history system.

The program was divided into specialized manager classes instead of allowing the main application file to contain most of the program logic.

The project was also reviewed for syntax that had not been covered during the course. Remaining `static_cast` usage was removed before final submission.

---

## Bug Fixes

Several bugs and edge cases were addressed during final development.

Duplicate truck numbers, driver IDs, and load numbers are prevented.

Invalid numeric input is rejected.

Negative values and zero values are rejected where they are not valid.

Non-finite numeric values are rejected.

Truck mileage is only added once when a load is completed.

Fuel purchases are not recorded until the related action is confirmed.

Driver payroll is not charged until the load is completed.

Cancelled load completion does not modify:

- Fuel
- Mileage
- Driver HOS
- Revenue
- Payroll
- Truck location

Trucks experiencing breakdowns cannot be dispatched until repaired.

Drivers and trucks assigned to active loads cannot be reused for another load.

Maintenance and repair actions respect simulated availability times.

Financial transactions use the correct simulated transaction time.

Changing truck or driver IDs updates related load and financial references.

Assigned and completed loads cannot be edited in ways that would invalidate completed simulation data.

Older save files remain compatible with newer save versions.

---

## Usability Improvements

TruckzSim received several final usability improvements.

Menus use consistent navigation and input validation.

Invalid menu selections are rejected and reprompted.

Text input is trimmed to prevent accidental spaces from causing incorrect data.

Date input accepts multiple formats.

Time input accepts 12-hour and 24-hour formats.

Time entry does not require the user to manually enter a colon.

Important actions display clear confirmation screens before making changes.

Driver Management clearly displays Hours of Service and rest progress.

Fleet Management displays truck fuel information, availability, maintenance status, and current location.

Banking provides separate views for different financial transaction types.

The Company Overview provides a single dashboard for viewing major company information.

System Validation provides an easy way to confirm that the company's systems remain consistent.

---

## Final Testing

Final testing was performed across the entire application instead of only testing the newest feature.

Testing included:

- Creating new companies
- Loading saved companies
- Saving company data
- Restarting and reloading saved data
- Adding trucks
- Adding drivers
- Adding loads
- Duplicate ID handling
- Invalid menu input
- Invalid numeric input
- Flexible date and time input
- Solo dispatch
- Team dispatch
- Driver Hours of Service
- Partial driver rest
- Full driver rest
- Resting all eligible drivers
- Fuel planning
- Manual refueling
- Load completion
- Truck mileage updates
- Truck location updates
- Random truck breakdowns
- Truck repair
- Preventive maintenance
- Financial transaction history
- Driver payroll
- Negative company balances
- Settings changes
- Editing entered information
- Confirmation cancellation behavior
- Company Overview
- System Validation
- Application exit behavior

Previously completed features were retested after final changes to make sure new development did not break existing functionality.

The final version successfully compiled and completed full-system validation.

---

## Final Project Status

TruckzSim is now a complete and functional C++ console application that demonstrates the development completed throughout the four-week course.

The final application uses multiple classes and systems, references and pointers, persistent data, input validation, menus, loops, financial management, gameplay simulation, and structured project organization.

The project is ready for final presentation and submission.