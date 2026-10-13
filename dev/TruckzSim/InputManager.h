#pragma once

#include <string>

class InputManager
{
public:
    static int ReadInt(const std::string& prompt);
    static int ReadPositiveInt(const std::string& prompt, const std::string& fieldName);
    static double ReadNonNegativeDouble(const std::string& prompt, const std::string& fieldName);
    static double ReadPositiveDouble(const std::string& prompt, const std::string& fieldName);
    static std::string ReadRequiredText(const std::string& prompt, const std::string& fieldName);
    static double ReadCurrency(const std::string& prompt);

private:
    static bool TryParseInt(const std::string& input, int& value);
    static bool TryParseDouble(const std::string& input, double& value);
};
