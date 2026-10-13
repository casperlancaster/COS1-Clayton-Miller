#include "InputManager.h"

#include <iostream>

bool InputManager::TryParseInt(const std::string& input, int& value)
{
    try
    {
        std::size_t charactersProcessed = 0;
        value = std::stoi(input, &charactersProcessed);

        return charactersProcessed == input.length();
    }
    catch (...)
    {
        return false;
    }
}

bool InputManager::TryParseDouble(const std::string& input, double& value)
{
    try
    {
        std::size_t charactersProcessed = 0;
        value = std::stod(input, &charactersProcessed);

        return charactersProcessed == input.length();
    }
    catch (...)
    {
        return false;
    }
}

int InputManager::ReadInt(const std::string& prompt)
{
    while (true)
    {
        std::string input;
        int value = 0;

        std::cout << prompt;
        std::getline(std::cin, input);

        if (TryParseInt(input, value))
        {
            return value;
        }

        std::cout << "Invalid input. Please enter a whole number.\n";
    }
}

int InputManager::ReadPositiveInt(
    const std::string& prompt,
    const std::string& fieldName)
{
    while (true)
    {
        int value = ReadInt(prompt);

        if (value > 0)
        {
            return value;
        }

        std::cout << "Invalid " << fieldName
            << ". Please enter a number greater than 0.\n";
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
        std::getline(std::cin, input);

        if (TryParseDouble(input, value) && value >= 0.0)
        {
            return value;
        }

        std::cout << "Invalid " << fieldName
            << ". Please enter a number that is 0 or greater.\n";
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
        std::getline(std::cin, input);

        if (!input.empty())
        {
            return input;
        }

        std::cout << fieldName << " cannot be empty. Please try again.\n";
    }
}

double InputManager::ReadCurrency(const std::string& prompt)
{
    while (true)
    {
        std::string payInput;
        std::string cleanedPay;
        double pay = 0.0;

        std::cout << prompt;
        std::getline(std::cin, payInput);

        for (char character : payInput)
        {
            if (character != ',' && character != '$')
            {
                cleanedPay += character;
            }
        }

        if (TryParseDouble(cleanedPay, pay) && pay >= 0.0)
        {
            return pay;
        }

        std::cout << "Invalid load pay. Please enter a valid amount.\n";
    }
}
