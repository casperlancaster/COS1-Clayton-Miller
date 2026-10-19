#pragma once

#include <string>
#include <vector>

class Company;

class SaveLoadManager
{
public:
    static bool HasSavedCompanies();

    static std::vector<std::string> GetSavedCompanies();

    static bool SaveCompany(const Company& company);

    static bool LoadCompany(
        Company& company,
        const std::string& companyName
    );

private:
    static std::string CreateSaveFileName(
        const std::string& companyName
    );
};