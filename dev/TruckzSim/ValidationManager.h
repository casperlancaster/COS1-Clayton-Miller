#pragma once

#include <string>
#include <vector>

class Company;

class ValidationManager
{
public:
    static bool ValidateCompany(
        const Company& company,
        std::vector<std::string>& issues
    );

    static bool DisplayReport(
        const Company& company
    );
};