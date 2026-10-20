# TruckzSim – Black / Red / Lime Console Theme (October 20, 2026)

## WHAT CHANGED
The C++ game now supports a consistent terminal color theme across the existing startup, main menu, banking, dispatch, fleet, freight, driver, fuel, maintenance, settings, and validation screens.

- **Red:** game/section headings, exit options, serious errors and blocked actions.
- **Lime green:** numbered menu selections, successful actions, revenue, positive account balances, and available/completed statuses.
- **Yellow:** warnings, maintenance notices, expenses, and active/assigned statuses.
- **Cyan:** input prompts.
- **Muted gray:** dividing lines.
- **Normal terminal text:** explanations and most changing data.

The color engine is in **ConsoleTheme.h**, registered under the Visual Studio project as a header file. It enables Windows 10+ VT console support when available, and can also work in Linux/macOS terminals. No external color library is required.

All existing game logic remains in place. Saved-company data and serialization were not rewritten. New color calls are used only for console display.

## HOW TO RUN ON WINDOWS
1. **Back up your current TruckzSim project folder** before replacing anything.
2. Unzip this package into a **new folder**. Don't extract directly over your existing working project.
3. Open **TruckzSim.slnx** in Visual Studio (or open **TruckzSim.vcxproj** directly).
4. Select your usual **Debug / x64** configuration, then **Build > Build Solution**.
5. Run with **Ctrl+F5** (Start Without Debugging) to see the colored console.

The new file **ConsoleTheme.h** must be present in the same folder as the .cpp files. The `.vcxproj` and `.filters` have been updated to include it.

## TERMINAL COLOR CONTROLS
Color is **enabled automatically** when your terminal supports ANSI colors. When program output is redirected to a file, it stays plain by default so logs and automated tests stay readable. If VT activation is unavailable, the program falls back to normal uncolored text.

To switch colors OFF for a Command Prompt session:
```bat
set TRUCKZSIM_NO_COLOR=1
```
Then launch the game from that same session. `NO_COLOR=1` also works.

To force ANSI colors in a supporting terminal (optional):
```bat
set TRUCKZSIM_FORCE_COLOR=1
```
Only use force-color with a terminal that supports ANSI escapes. It is not needed for normal Windows Terminal or supported console hosts.

## DEVELOPMENT NOTES
- No added `.cpp` files or third-party dependencies; **ConsoleTheme.h** contains the color utility.
- Preserves menu numbering, prompt text, numeric formatting, save/load behavior, and game calculations.
- Handles dynamic truck/driver/load status text and financial totals with semantic colors.
- Save files under `saves/` are included unchanged.
- Old `x64/Debug` object files, executables, and Visual Studio generated caches were **excluded** intentionally: Visual Studio rebuilds them.

## VERIFICATION
- Clean C++20 GCC compile, with `-Wall -Wextra -pedantic` (Linux test environment).
- 13 menu/navigation scenarios matched the original build byte-for-byte with colors disabled.
- ANSI codes stripped from the colored output match original messages exactly.
- Color opt-out (`NO_COLOR`) tested.
- Existing saved-company files verified unchanged via hash.
- Visual Studio `.vcxproj` and `.filters` XML parsed successfully.

**Note:** The project has not been built using MSVC inside Windows here. Please compile in Visual Studio on your PC as the final platform verification.

## OCTOBER 20, 2026 — V1A VISUAL STUDIO BUILD FIX
- Replaced the three direct `std::getenv` accesses in `ConsoleTheme.h` with a shared environment lookup helper.
- On MSVC/Windows this uses `_dupenv_s` and properly frees the allocated string, avoiding Visual Studio C4996.
- Other compilers retain standard `std::getenv` behavior.
- No changes to dispatch logic, saved data, menu numbering, or color palette.
- After extracting, rebuild the solution in **Debug | x64**.
- A pre-existing `C4267` warning in `TruckzSimApp.cpp` may still appear; it is not the build-stopping `C4996` error.
