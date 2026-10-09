# Project & Portfolio 1

### Clayton Miller

Hello, my name is Clayton Miller. I am a Game Development bachelor's student at Full Sail University. The purpose of this repository is to practice development using version control while building TruckzSim, a C++ trucking company simulation. This work will help me continue developing my programming and game development skills while building portfolio-ready experience that can be shared in the future.

<br>

## 📢 Weekly Stand Up

Each week I will summarize my milestone activity and progress by writing a stand-up. A stand-up is meant to be a succinct update on how things are going. Use these prompts as a guide on what to write about:

⚙️ Overview - What I worked on this past week  
<br>
🌵 Challenges - What problems did I have & how I'm addressing them  
<br>
🏆 Accomplishments - What is something I "leveled up" on this week  
<br>
🔮 Next Steps - What I plan to prioritize and do next

<br>

### Week 1

#### ⚙️ Overview

This week I made a lot of progress on TruckzSim. I completed the initial Truck, Driver, Load, Company, and DispatchManager classes and built a persistent main menu for the program. I also added working systems for creating and displaying trucks, drivers, and loads. The Dispatch Center can now assign an available driver and truck to a load, update their statuses, and prevent the same driver or truck from being assigned to multiple active loads.

#### 🌵 Challenges

One of the biggest challenges this week was getting more comfortable with how the different classes should interact with each other. I also ran into a linker error while adding new Company getters and had to trace it back to missing function definitions in the .cpp file. Another issue I found during testing was that entering a load payment with a comma caused part of the value to be ignored, so I updated the input handling to accept formatted currency correctly.

#### 🏆 Accomplishments

I feel like I leveled up the most in program structure, debugging, and Git/GitHub workflow this week. TruckzSim went from a basic project structure to a working simulation foundation with multiple classes, vectors, pointers, input validation, record management, and connected systems. I also became more comfortable working on the dev branch, making meaningful commits, pushing changes, managing Issues, and moving tasks through the Project board as they were completed.

#### 🔮 Next Steps

For the next milestone, I plan to continue expanding the core TruckzSim systems and make the simulation more interactive. I want to improve the Dispatch Center, add more company and economy functionality, expand the fleet and driver systems, improve input validation, and continue organizing the code so the different systems stay separated and easier to maintain as the project grows.

### Week 2

#### ⚙️ Overview

This week I focused on expanding TruckzSim’s Dispatch Center and improving the program’s usability. I refactored the input handling to use getline() with stoi() and stod() inside try/catch instead of relying on repeated cin.fail(), clear(), and ignore() logic. I also improved the Dispatch Center so it displays available loads, drivers, and trucks before assignment. In addition, I added load completion, equipment release, and company revenue tracking.

#### 🌵 Challenges

One of the main challenges this week was changing the input-handling system throughout the program without breaking functionality that was already working from Milestone 1. I also had to make sure completing a load correctly updated several connected objects at once, including the Load, Driver, Truck, and Company. I tested each change individually to make sure the program stayed stable.

#### 🏆 Accomplishments

I improved my understanding of input validation, exception handling, pointers, and how multiple classes can interact with each other. TruckzSim now has a more complete dispatch cycle where a load can be assigned, completed, and recorded as revenue while the driver and truck become available again. I also made the program easier to use by giving clearer prompts and showing available resources before assignment.

#### 🔮 Next Steps

Before Week 3, I plan to continue expanding the company and economy systems, improve code organization as the project grows, and add more depth to the simulation. I also want to continue improving testing and input validation while preparing additional systems such as banking and maintenance.

### Week 3

Stay tuned, this stand up is coming soon...

### Week 4

My final stand up...