#pragma once
#include <string>
namespace lady
{
	std::string ToUpper(const std::string& s);
	std::string ToLower(const std::string& s);
	bool IsEqualIgnoreCase(const std::string& s1, const std::string& s2);
	std::string CreateUnique(const std::string& s);
}