#include "InputManager.h"

#include <cctype>
#include <cmath>
#include <iostream>

std::string InputManager::Trim(
    const std::string& input)
{
    std::size_t start = 0;

    while (start < input.size() &&
        std::isspace(
            static_cast<unsigned char>(
                input[start])))
    {
        start++;
    }

    std::size_t end =
        input.size();

    while (end > start &&
        std::isspace(
            static_cast<unsigned char>(
                input[end - 1])))
    {
        end--;
    }

    return input.substr(
        start,
        end - start
    );
}

bool InputManager::TryParseInt(
    const std::string& input,
    int& value)
{
    std::string cleanedInput =
        Trim(input);

    if (cleanedInput.empty())
    {
        return false;
    }

    try
    {
        std::size_t charactersProcessed = 0;

        value =
            std::stoi(
                cleanedInput,
                &charactersProcessed
            );

        return charactersProcessed ==
            cleanedInput.length();
    }
    catch (...)
    {
        return false;
    }
}

bool InputManager::TryParseDouble(
    const std::string& input,
    double& value)
{
    std::string cleanedInput =
        Trim(input);

    if (cleanedInput.empty())
    {
        return false;
    }

    try
    {
        std::size_t charactersProcessed = 0;

        value =
            std::stod(
                cleanedInput,
                &charactersProcessed
            );

        return charactersProcessed ==
            cleanedInput.length() &&
            std::isfinite(value);
    }
    catch (...)
    {
        return false;
    }
}

int InputManager::ReadInt(
    const std::string& prompt)
{
    while (true)
    {
        std::string input;
        int value = 0;

        std::cout << prompt;

        std::getline(
            std::cin,
            input
        );

        if (TryParseInt(
            input,
            value))
        {
            return value;
        }

        std::cout
            << "Invalid input. "
            << "Please enter a whole number.\n";
    }
}

int InputManager::ReadMenuChoice(
    const std::string& prompt,
    int minimumChoice,
    int maximumChoice)
{
    while (true)
    {
        int choice =
            ReadInt(
                prompt
            );

        if (choice >= minimumChoice &&
            choice <= maximumChoice)
        {
            return choice;
        }

        std::cout
            << "Invalid selection. "
            << "Please enter a number from "
            << minimumChoice
            << " to "
            << maximumChoice
            << ".\n";
    }
}

int InputManager::ReadPositiveInt(
    const std::string& prompt,
    const std::string& fieldName)
{
    while (true)
    {
        int value =
            ReadInt(
                prompt
            );

        if (value > 0)
        {
            return value;
        }

        std::cout
            << "Invalid "
            << fieldName
            << ". Please enter a number "
            << "greater than 0.\n";
    }
}

double InputManager::ReadNonNegativeDouble(
    const std::string& prompt,
    const std::string& fieldName)
{
    while (true)
    {
        std::string input;
        double value = 0.0;

        std::cout << prompt;

        std::getline(
            std::cin,
            input
        );

        if (TryParseDouble(
            input,
            value) &&
            value >= 0.0)
        {
            return value;
        }

        std::cout
            << "Invalid "
            << fieldName
            << ". Please enter a finite number "
            << "that is 0 or greater.\n";
    }
}

double InputManager::ReadPositiveDouble(
    const std::string& prompt,
    const std::string& fieldName)
{
    while (true)
    {
        std::string input;
        double value = 0.0;

        std::cout << prompt;

        std::getline(
            std::cin,
            input
        );

        if (TryParseDouble(
            input,
            value) &&
            value > 0.0)
        {
            return value;
        }

        std::cout
            << "Invalid "
            << fieldName
            << ". Please enter a finite number "
            << "greater than 0.\n";
    }
}

std::string InputManager::ReadRequiredText(
    const std::string& prompt,
    const std::string& fieldName)
{
    while (true)
    {
        std::string input;

        std::cout << prompt;

        std::getline(
            std::cin,
            input
        );

        input =
            Trim(
                input
            );

        if (!input.empty())
        {
            return input;
        }

        std::cout
            << fieldName
            << " cannot be empty. "
            << "Please try again.\n";
    }
}

double InputManager::ReadCurrency(
    const std::string& prompt)
{
    while (true)
    {
        std::string payInput;
        std::string cleanedPay;
        double pay = 0.0;

        std::cout << prompt;

        std::getline(
            std::cin,
            payInput
        );

        payInput =
            Trim(
                payInput
            );

        for (char character :
        payInput)
        {
            if (character != ',' &&
                character != '$')
            {
                cleanedPay +=
                    character;
            }
        }

        if (TryParseDouble(
            cleanedPay,
            pay) &&
            pay > 0.0)
        {
            return pay;
        }

        std::cout
            << "Invalid load pay. "
            << "Please enter an amount greater than 0.\n";
    }
}