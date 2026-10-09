# Milestone 2 Project Planning Worksheet

> Use this worksheet to plan the next phase of your project **before you begin coding**  
> Be clear, specific, and intentional—this will guide your development this week.

---

## 📌 Project Overview

**Project Name:**  
→ TruckzSim

**What does your program currently do? (1–3 sentences)**  
→ TruckzSim is a C++ trucking company simulation that allows the user to create and manage trucks, drivers, and freight loads. The program can assign available drivers and trucks to loads through the Dispatch Center, track their statuses, and display company information through a persistent console menu.

---

## 🔍 Current Progress Check

**What is working right now?**  
→ The main menu and submenus are working, and the user can create and view trucks, drivers, and loads. The DispatchManager can assign an available driver and truck to a load, update their statuses, and prevent the same resources from being double-dispatched.

**What is NOT working or incomplete?**  
→ The Dispatch Center does not yet provide enough information about which loads, drivers, and trucks are available before an assignment is made. Loads also cannot yet be completed, drivers and trucks cannot be released after delivery, and company revenue is not being tracked.

**What feels confusing or messy in your code?**  
→ Input handling is repeated throughout the program using `std::cin.fail()`, `std::cin.clear()`, and `std::cin.ignore()`. I want to make input handling more consistent by reading input with `getline()` and converting numeric values with `stoi()` or `stod()` inside `try/catch`.

---

## 🚀 Feature Planning

List the features you plan to add or improve this week.

### Feature 1
**Name:**  
→ Improved Input Handling

**What does this feature do?**  
→ Refactors menu and numeric input so the program reads input with `getline()` and converts it using `stoi()` or `stod()` with exception handling. Invalid input will produce a clear message and allow the user to try again.

**Why is this feature important?**  
→ It improves usability, prevents broken input streams, reduces repeated error-handling code, and directly addresses feedback from Milestone 1.

---

### Feature 2
**Name:**  
→ Improved Dispatch Center

**What does this feature do?**  
→ Displays available loads, drivers, and trucks before the user makes an assignment. It also provides more specific feedback when a selected resource does not exist or is unavailable.

**Why is this feature important?**  
→ The user should not have to guess which resources can be assigned. Showing availability makes the dispatch process easier to understand and reduces invalid selections.

---

### Feature 3 (optional)
**Name:**  
→ Load Completion, Equipment Release, and Revenue Tracking

**What does this feature do?**  
→ Allows an assigned load to be completed, changes its status to Completed, returns its driver and truck to Available status, and adds the completed load payment to company revenue.

**Why is this feature important?**  
→ This creates a complete gameplay loop instead of ending at assignment. It also begins connecting the dispatch system with the company economy.

---

## 🧩 System Design Updates

**Will you need to create any new classes? If so, which ones?**  
→ No new classes are required for this phase. The existing Company, DispatchManager, Load, Driver, and Truck classes can support the planned features.

**Will you modify any existing classes? How?**  
→ I will expand `DispatchManager` with load-completion logic and update `Company` to track revenue. I will also improve the main TruckzSim program flow and input-handling functions.

**What data structures will you use (vectors, 2D vectors, etc.)?**  
→ The program will continue using `std::vector<Truck>`, `std::vector<Driver>`, and `std::vector<Load>` inside the Company class. The DispatchManager will search and update objects stored in these vectors.

---

## 🔄 Program Flow

**Describe how a user interacts with your program:**

1. Program starts → The TruckzSim main menu is displayed.  
2. User chooses → The user selects Dispatch Center, Fleet Management, Driver Management, Freight Market, Company Overview, Banking, Maintenance, or Exit.  
3. Program responds → The selected system displays its options, validates the user's input, performs the requested action, and provides clear feedback.  
4. Loop/next step → The program returns to the appropriate menu and continues until the user chooses Exit.

---

## 🎯 Usability Improvements

How will you make your program easier to use this week?

- Clearer prompts:  
→ Prompts will explain exactly what value the user should enter, and the Dispatch Center will show available resources before asking for IDs.

- Better error handling:  
→ Numeric input will use `getline()` with `stoi()` or `stod()` and `try/catch`, allowing invalid input to be handled without breaking the input stream.

- Improved menu/navigation:  
→ The Dispatch Center will include options for viewing available resources and completing assigned loads while continuing to return the user to the menu after each action.

---

## ⚠️ Potential Challenges

**What do you think will be the hardest part this week?**  
→ The hardest part will be changing input handling throughout the program without breaking existing functionality, while also making sure completing a load correctly updates the load, driver, truck, and company revenue.

**What is your plan if you get stuck?**  
→ I will test one feature at a time, compile frequently, review the Day 3 lecture examples, use debugger/output testing to trace incorrect states, and compare each change against the working Milestone 1 version.

---

## 📈 Level Up Goal

**What skill are you focusing on improving this week?**  
→ Input validation, error handling, and connecting multiple classes together through cleaner program logic.

**What will you do to improve it?**  
→ I will review the lecture examples for `getline()`, `stoi()`, `stod()`, and `try/catch`, practice applying them throughout TruckzSim, test invalid input and edge cases, and continue working with pointers and object interactions through DispatchManager.

---

## 🗓️ Task Breakdown (GitHub Issues Planning)

List the tasks you plan to create as GitHub Issues:

- [x] Refactor input handling with getline and try/catch
- [x] Improve Dispatch Center availability and assignment flow
- [x] Add load completion and equipment release
- [x] Add company revenue tracking for completed loads

---

## 🔥 Final Check

Before you start coding, ask yourself:

- [x] Do I know what I’m building this week?  
- [x] Do I know where to start?  
- [x] Did I break my work into small tasks?

If yes → start coding 🚀  
If no → refine your plan first

---

## 😈 Final Thought

> Plan it now… or debug it later.
