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

## Week 3 Project Stand-Up

⚙️ **Overview**  
This week I focused on refactoring TruckzSim and expanding the simulation systems. I separated the program into dedicated classes so the main file only handles startup, while the different systems manage their own responsibilities.

🛠️ **Improvements**  
I added duplicate protection for truck numbers, driver IDs, and load numbers, truck mileage tracking from completed loads, custom company naming, a maintenance system with service intervals and expenses, and a banking system that tracks revenue, expenses, and company balance.

I also moved the remaining application flow into TruckzSimApp so TruckzSim.cpp is now only a few lines and is responsible for starting the application.

🌵 **Challenges**  
The biggest challenge was refactoring working code without breaking existing features. I handled this by moving one system at a time, rebuilding often, and testing each part before continuing.

Keeping Dispatch, Maintenance, mileage, and Banking connected correctly also required additional testing.

🏆 **Accomplishments**  
One of my biggest improvements this week was learning how to separate responsibilities between classes instead of allowing one large file to control the entire program.

TruckzSim.cpp was reduced from approximately 990 lines to only the main application entry point, while systems such as input, fleet management, drivers, freight, dispatch, maintenance, banking, and application flow now have clearer responsibilities.

🔮 **Next Steps**  
For Week 4, my main goal is to add save/load functionality so company progress can persist between sessions.

I also plan to begin expanding the simulation with truck fuel usage, simplified driver Hours of Service rules, and truck breakdown events if time allows.

### Week 4

## Final Project Stand-Up

⚙️ **Overview – What did you accomplish this past week?**  
This week I completed the final development work on TruckzSim. I added save/load, fuel management, driver HOS and rest controls, breakdowns and repairs, financial history, settings, system validation, truck locations, an improved company overview, safety confirmations, and driver compensation.

🛠️ **Project Wrap-Up – What did you finish, polish, or improve before submitting your final project?**  
I improved how the systems work together, strengthened input validation and navigation, added confirmation prompts, improved my use of references and pointers, tested save/load persistence, cleaned up the code, and completed full-system testing.

🌵 **Challenges – What obstacles did you face, and how did you overcome them?**  
The biggest challenge was making sure new systems did not break existing features. HOS, fuel, maintenance, repairs, finances, driver compensation, and save/load all interact. I handled this by working one issue at a time, rebuilding often, testing edge cases, and using system validation after major changes.

📚 **Biggest Takeaway – Looking back, what was the most valuable thing you learned during this course?**  
My biggest takeaway was learning how much easier a larger program becomes when it is separated into classes and systems. I also became more comfortable with GitHub, branches, Issues, commits, debugging, and testing. I now understand how to break a larger project into smaller pieces and build it over time.

🔮 **Future Development – If you continued this project after the course, what would you build or improve next?**  
I would expand TruckzSim with deeper driver systems, trailers, equipment, freight types, customers, insurance, loans, employees, maintenance and compliance systems, and eventually connect it to an Unreal Engine driving experience.