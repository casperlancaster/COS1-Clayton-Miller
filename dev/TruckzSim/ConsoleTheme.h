#pragma once

// TruckzSim console color palette: red headings, lime menus/success,
// cyan prompts, yellow cautions, and red failures.
// Windows 10+ VT mode is enabled when available; redirected output stays plain.
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

// windows.h maps GetDateFormat/GetTimeFormat to Win32 API A/W aliases.
// TruckzSim has Company and HOSManager methods with these exact names;
// leaving the macros defined silently renames declarations and call sites
// (e.g. GetDateFormatW) in some translation units, causing LNK2019/LNK2001.
// Remove only these two Windows macros after importing the Windows APIs.
#ifdef GetDateFormat
#undef GetDateFormat
#endif
#ifdef GetTimeFormat
#undef GetTimeFormat
#endif
#else
#include <unistd.h>
#endif

namespace ConsoleTheme
{
    enum class Color
    {
        Red,
        Lime,
        Yellow,
        Cyan,
        Muted
    };

    // Header-only shared state (one copy program-wide with inline functions).
    inline bool& ColorsEnabled()
    {
        static bool enabled = false;
        return enabled;
    }

    // MSVC warns on std::getenv (C4996). Use its checked, allocating
    // _dupenv_s API on Windows, and standard getenv on other compilers.
    // The allocated Windows string is always freed before returning.
    inline std::string ReadEnvironment(const char* name)
    {
#if defined(_WIN32) && defined(_MSC_VER)
        char* value = nullptr;
        std::size_t length = 0;
        const auto error = ::_dupenv_s(&value, &length, name);
        if (error != 0)
        {
            std::free(value);
            return {};
        }
        const std::string result = value ? std::string(value) : std::string();
        std::free(value);
        return result;
#else
        const char* value = std::getenv(name);
        return value ? std::string(value) : std::string();
#endif
    }

    inline void Initialize()
    {
        ColorsEnabled() = false;

        // Respect standard NO_COLOR convention and the TruckzSim override.
        if (!ReadEnvironment("NO_COLOR").empty() ||
            !ReadEnvironment("TRUCKZSIM_NO_COLOR").empty())
        {
            return;
        }

#ifdef _WIN32
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (console != INVALID_HANDLE_VALUE && console != nullptr &&
            GetConsoleMode(console, &mode))
        {
            if ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0 ||
                SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
            {
                ColorsEnabled() = true;
            }
        }
#else
        ColorsEnabled() = isatty(STDOUT_FILENO) != 0;
#endif

        // Optional override for terminals/build tests supporting ANSI output.
        const std::string forceColor = ReadEnvironment("TRUCKZSIM_FORCE_COLOR");
        if (!forceColor.empty() && forceColor.front() != '0')
        {
            ColorsEnabled() = true;
        }
    }

    // Stream-safe coloring for dynamic numbers; keeps std::fixed/setprecision intact.
    inline const char* Begin(Color color)
    {
        if (!ColorsEnabled()) return "";
        switch (color)
        {
        case Color::Red:    return "\x1b[91m";
        case Color::Lime:   return "\x1b[38;2;166;255;35m";
        case Color::Yellow: return "\x1b[93m";
        case Color::Cyan:   return "\x1b[96m";
        case Color::Muted:  return "\x1b[90m";
        }
        return "";
    }

    inline const char* End()
    {
        return ColorsEnabled() ? "\x1b[0m" : "";
    }

    inline std::string Paint(std::string_view text, Color color)
    {
        if (!ColorsEnabled() || text.empty())
        {
            return std::string(text);
        }

        const char* code = "";
        switch (color)
        {
        case Color::Red:    code = "\x1b[91m"; break;
        case Color::Lime:   code = "\x1b[38;2;166;255;35m"; break;
        case Color::Yellow: code = "\x1b[93m"; break;
        case Color::Cyan:   code = "\x1b[96m"; break;
        case Color::Muted:  code = "\x1b[90m"; break;
        }
        return std::string(code) + std::string(text) + "\x1b[0m";
    }

    inline bool Contains(std::string_view text, std::string_view token)
    {
        return text.find(token) != std::string_view::npos;
    }

    inline std::string Lower(std::string_view text)
    {
        std::string result(text);
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return result;
    }

    inline std::string Prompt(std::string_view text)
    {
        return Paint(text, Color::Cyan);
    }

    inline std::string Status(std::string_view status)
    {
        const std::string lower = Lower(status);
        if (lower == "available" || lower == "completed" || lower == "active" ||
            lower == "ready" || lower == "rested" || lower == "paid")
        {
            return Paint(status, Color::Lime);
        }
        if (lower == "breakdown" || lower == "out of service" || lower == "failed" ||
            lower == "inactive")
        {
            return Paint(status, Color::Red);
        }
        if (lower == "maintenance" || lower == "dispatched" || lower == "assigned" ||
            lower == "off duty" || lower == "on duty" || lower == "resting")
        {
            return Paint(status, Color::Yellow);
        }
        return std::string(status);
    }

    inline std::string FormatLiteral(std::string_view text)
    {
        // This is called ONLY on literal console output, not saved game data.
        // Without terminal color, the exact existing message is preserved.
        if (!ColorsEnabled() || text.empty())
        {
            return std::string(text);
        }

        std::string_view trimmed = text;
        while (!trimmed.empty() && (trimmed.front() == '\n' ||
            trimmed.front() == '\r' || trimmed.front() == ' ' || trimmed.front() == '\t'))
        {
            trimmed.remove_prefix(1);
        }
        if (trimmed.empty()) return std::string(text);

        // "1. Fleet Management" -> lime number, default-colored name.
        std::size_t digitCount = 0;
        while (digitCount < trimmed.size() &&
            std::isdigit(static_cast<unsigned char>(trimmed[digitCount])))
        {
            ++digitCount;
        }
        if (digitCount > 0 && digitCount < 3 && digitCount + 1 < trimmed.size() &&
            (trimmed[digitCount] == '.' || trimmed[digitCount] == ')') &&
            trimmed[digitCount + 1] == ' ')
        {
            const std::size_t prefix = text.size() - trimmed.size();
            const std::size_t end = prefix + digitCount + 1;
            return std::string(text.substr(0, prefix)) +
                Paint(text.substr(prefix, end - prefix),
                    trimmed.front() == '0' ? Color::Red : Color::Lime) +
                std::string(text.substr(end));
        }

        const std::string lower = Lower(trimmed);
        const bool heading = Contains(trimmed, "====") ||
            (Contains(trimmed, "----") && trimmed.size() > 25);
        if (heading)
        {
            bool hasLetters = false;
            for (unsigned char c : trimmed)
            {
                if (std::isalpha(c)) { hasLetters = true; break; }
            }
            return Paint(text, hasLetters ? Color::Red : Color::Muted);
        }

        // Short all-caps subheadings used throughout summary screens.
        std::size_t letters = 0;
        bool uppercase = true;
        for (unsigned char c : trimmed)
        {
            if (std::isalpha(c))
            {
                ++letters;
                if (!std::isupper(c)) uppercase = false;
            }
        }
        if (uppercase && letters >= 3 && letters <= 32 && trimmed.size() <= 40 &&
            !Contains(trimmed, ":") && !Contains(trimmed, "$") &&
            !Contains(trimmed, "#"))
        {
            return Paint(text, Color::Red);
        }

        // Failures have priority over success words (e.g. "could not be loaded").
        if (Contains(lower, "invalid") || Contains(lower, "error") ||
            Contains(lower, "failed") || Contains(lower, "blocked") ||
            Contains(lower, "could not") || Contains(lower, "cannot") ||
            Contains(lower, "not allowed") || Contains(lower, "negative balance") ||
            Contains(lower, "does not exist") || Contains(lower, "not found") ||
            Contains(lower, "unavailable"))
        {
            return Paint(text, Color::Red);
        }
        if (Contains(lower, "warning") || Contains(lower, "caution") ||
            Contains(lower, "low fuel") || Contains(lower, "insufficient") ||
            Contains(lower, "maintenance due") || Contains(lower, "overdue") ||
            Contains(lower, "risk:") || Contains(lower, "due soon"))
        {
            return Paint(text, Color::Yellow);
        }
        if (Contains(lower, "successfully") || Contains(lower, "success!") ||
            Contains(lower, "completed!") || Contains(lower, "positive balance") ||
            Contains(lower, "status: company account is positive") ||
            Contains(lower, "validation passed"))
        {
            return Paint(text, Color::Lime);
        }
        if (Contains(lower, "select an option") || Contains(lower, "enter selection") ||
            Contains(lower, "select a ") || Contains(lower, "enter your ") ||
            Contains(lower, "choose an option"))
        {
            return Prompt(text);
        }
        if (Contains(lower, "revenue:") || Contains(lower, "income:") ||
            Contains(lower, "profit:") || Contains(lower, "balance:") ||
            Contains(lower, "payment received"))
        {
            return Paint(text, Color::Lime);
        }
        if (Contains(lower, "expenses:") || Contains(lower, "repair cost:") ||
            Contains(lower, "total fuel cost:") || Contains(lower, "total cost:"))
        {
            return Paint(text, Color::Yellow);
        }
        return std::string(text);
    }
}
