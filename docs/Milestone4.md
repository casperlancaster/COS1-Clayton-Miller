# Milestone 4 - TruckzSim

**Student:** Clayton Miller  
**Course:** COS119 - Project & Portfolio I  
**Project:** TruckzSim  
**Milestone:** 4

## Final Features Added

During Milestone 4, TruckzSim received several final gameplay and management systems that completed the console application.

### Company Save and Load
A multi-company save and load system was implemented so company data can persist between program sessions. Saved companies can be selected from the startup menu, and older save versions remain compatible with newer versions of the application.

### Fuel Management
Trucks now have individual fuel tank capacities, current fuel levels, and fuel economy ratings. Loads calculate required trip fuel and maintain a required delivery reserve. Fuel purchased during a trip is recorded as a company expense. Manual refueling is also available through the Fuel Management menu.

### Driver Hours of Service
Drivers now track available driving hours, accumulated rest, and simulated availability time. Solo trips calculate required rest periods, and team operations can be used when a solo driver cannot legally meet a delivery deadline.

Driver Management was also expanded with manual rest controls. Individual drivers can be rested until their Hours of Service reset or have off-duty time advanced manually. All eligible available drivers can also be rested together.

### Breakdown and Repair System
Trucks can experience random post-trip mechanical breakdowns. Several breakdown types include different parts costs, labor costs, service charges, and repair times. Broken trucks are blocked from dispatch until repaired through the Breakdown / Repair Center.

### Financial Transaction History
TruckzSim now records company income and expenses as individual financial transactions. Transactions include load revenue, fuel purchases, maintenance, repairs, and driver payroll. Banking provides separate views for income, expenses, all transactions, and driver payroll.

### Settings and Data Correction
A Settings menu was added to improve usability. Players can select date and time display formats and correct truck, driver, and available-load information when entry mistakes occur.

Date and time input was also made more flexible. The program accepts multiple date formats and allows time entry with or without a colon.

### System Validation
A System Validation feature was added to inspect the company's trucks, drivers, loads, finances, assignments, statuses, and other stored data for inconsistent or invalid values. Companies are also validated during important save and load operations.

### Company Overview Dashboard
The Company Overview was expanded into a management dashboard showing fleet, driver, freight, financial, maintenance, and breakdown information.

Trucks now track their current location. A truck begins at the location entered when it is created and automatically moves to the destination of a completed load. Truck locations persist through save and load.

### Confirmation and Safety Prompts
Confirmation prompts were added before important actions such as dispatching freight, completing loads, purchasing fuel, performing repairs, completing maintenance, changing important IDs, and exiting the application.

Cancelling an action leaves the related company data unchanged.

### Driver Payroll
The final payroll system records driver compensation when freight is completed.

Solo drivers earn $0.60 per mile.

Team drivers each earn 70 percent of the solo rate, or $0.42 per mile per driver.

Payroll is recorded as a company expense and appears in the financial transaction history. Payroll is processed only after a load is completed and can result in a negative company balance.

## Refactoring Improvements

Object access was improved by using references and pointers to work directly with trucks, drivers, and loads stored by the Company class.

Reusable object lookup functions were added for finding trucks, drivers, and loads.

Input handling was centralized through InputManager to reduce repeated validation logic.

Menu input was standardized using range-checked menu choices.

Repeated financial operations were consolidated through the financial transaction system.

The project was also reviewed for unsupported or unnecessary casting syntax, and remaining static_cast usage was removed before final submission.

## Bug Fixes

Duplicate truck, driver, and load identifiers are prevented.

Invalid numeric input, negative values, zero values where inappropriate, and non-finite numeric values are rejected.

Truck mileage is updated only once when a load is completed.

Completed-load fuel, HOS, revenue, truck location, and payroll updates occur only after confirmation.

Drivers and trucks assigned to active loads cannot be incorrectly reused for another load.

Breakdown trucks cannot be dispatched until repaired.

Maintenance and repair scheduling respects simulated availability time.

Financial transactions preserve their correct simulated transaction time.

Settings edits update related load and transaction references when truck or driver IDs change.

Assigned or completed loads are protected from unsafe editing.

Older company save files continue loading after newer save formats were introduced.

## Usability Updates

Menus provide clearer prompts and navigation.

Invalid menu selections are rejected and reprompted.

Date input supports multiple formats.

Time input supports both 12-hour and 24-hour formats and does not require the user to type a colon.

Important actions provide confirmation screens before modifying company data.

Company Overview gives the user a single dashboard for viewing the current condition of the company.

Driver Management provides clear HOS and rest information.

Banking provides dedicated financial history views, including payroll history.

System Validation gives the user an immediate way to confirm that major company systems remain consistent.

## Final Testing

The completed program was tested across its major systems rather than only testing the newest feature.

Testing included:

- Creating and loading companies
- Adding and editing trucks, drivers, and loads
- Invalid and duplicate input
- Solo and team dispatch
- Driver Hours of Service and rest
- Fuel consumption and manual refueling
- Load completion
- Truck location updates
- Random breakdowns and repairs
- Preventive maintenance
- Financial transactions
- Driver payroll
- Confirmation and cancellation behavior
- Settings and date/time formats
- Company save and reload persistence
- System Validation

The final application successfully compiled and completed full-system testing before submission.