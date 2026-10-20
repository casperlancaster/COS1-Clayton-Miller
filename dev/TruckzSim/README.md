# TruckzSim — C++ trucking-management simulation

**Full Sail University — Project & Portfolio I (COS119)**  
Student: Clayton Miller  
Project: TruckzSim — console-based trucking company / dispatch simulation

## Open, build, and run on Windows

1. Install **Visual Studio 2022 or Visual Studio 2026** with the **Desktop development with C++** workload and a Windows 10/11 SDK. The project's compiler is selected automatically: **v143** in VS 2022, **v145** in VS 2026. These are standard Microsoft C++ compiler components installed with the appropriate C++ workload; no third-party libraries are required.
2. Clone/download the GitHub repository. Keep all the `.cpp`, `.h`, solution, and project files together in this folder (`dev/TruckzSim/` in the COS119 repository).
3. Open **`TruckzSim.sln`** in Visual Studio. The `.slnx` is also provided for newer Visual Studio versions.
4. At the toolbar, choose **Debug** and **x64**.
5. Select **Build > Rebuild Solution**.
6. Run with **Ctrl+F5** (**Debug > Start Without Debugging**). Use the displayed menu to start or load a trucking company.

**Expected result:** A console window opens with the TruckzSim menu. The default color theme uses red headings, lime-green menu/success text, cyan prompts, and yellow warnings. It reverts to plain text automatically in environments without ANSI support.

### Color controls (optional)

No setup is required. If you want plain text for logging or accessibility, set `TRUCKZSIM_NO_COLOR=1` in the launching terminal. `NO_COLOR=1` also works.

### Saving/loading

The app stores companies in the `saves/` directory **relative to the program's working directory**. Visual Studio normally runs it from the project directory, so included example saves should be visible. New companies are saved there. Run with a writable working directory. If a student opens the executable directly from `x64/Debug` instead, its working directory may be different and the game will create/use a separate `saves` directory.

## Contents and dependencies

- C++20, Microsoft Visual C++ compiler / Visual Studio. Uses the C++ standard library and Windows console APIs only; no SDL, graphics engine, database, external packages, SDK downloads (beyond normal Windows C++ development tools), or external services.
- `TruckzSim.sln` (widely supported solution), `TruckzSim.slnx` (modern solution), `TruckzSim.vcxproj` and `.filters` (project metadata)
- `ConsoleTheme.h` (header-only console colors / Windows-compiler safety and macro compatibility fixes)
- `.cpp` and `.h` gameplay sources (fleet, dispatch, drivers, banking, freight, fuel, HOS, etc.)
- `saves/*.txt`: example testing data. Save format is unchanged.
- `.gitignore`: excludes generated IDE files, binaries, and local machine caches.

## For GitHub submission

Commit the source, headers, both solution formats, `.vcxproj`, `.vcxproj.filters`, `.gitignore`, this README, and intended sample saves. **Do not commit** `x64/`, `.vs/`, `*.obj`, `*.pdb`, `*.exe`, or `*.vcxproj.user`. The `.gitignore` prevents newly created artifacts from being added; if any such files are **already tracked** by Git, Git must be told to untrack them explicitly before final submission.

The professor should not need to copy patches or change the compiler target manually. **They must still have Visual Studio with the corresponding Microsoft C++ workload and Windows SDK installed.** If they have an older Visual Studio release or the C++ workload isn't installed, that prerequisite is not something a repository can replace.

## Build-check status (Oct 20, 2026)

- Source code identical to the previously provided V1B game build; only project/solution metadata and submission documentation are changed.
- GNU GCC C++20 compile and console run validated in the test environment; project XML and file references checked, no external dependencies detected.
- This environment does **not** have Microsoft Visual Studio/MSVC. Before uploading/merging for grading, perform **Build > Rebuild Solution** on Windows **after cloning the final commit to a new location** (don't compile using old local intermediate files), check **1 succeeded / 0 failed**, and test launching. This is necessary to establish the actual professor experience.
