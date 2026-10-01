#include "DispatchManager.h"

DispatchManager::DispatchManager()
{
	company = nullptr;
}

DispatchManager::DispatchManager(Company* newCompany)
{
	company = newCompany;
}

Company* DispatchManager::GetCompany() const
{
	return company;
}

void DispatchManager::SetCompany(Company* newCompany)
{
	company = newCompany;
}

bool DispatchManager::HasCompany() const
{
	return company != nullptr;
}