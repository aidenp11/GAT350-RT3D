#include "StringUtils.h"
#include <string>
#include <algorithm>

namespace lady
{
	std::string ToUpper(const std::string& s)
	{
		std::string result = s;
		for (auto& c : result) c = std::toupper(c);

		return result;
	}
	std::string ToLower(const std::string& s)
	{
		std::string result = s;
		for (auto& c : result) c = std::tolower(c);

		return result;

	}
	bool IsEqualIgnoreCase(const std::string& s1, const std::string& s2)
	{
		if (s1.size() != s2.size()) return false;

		return ToUpper(s1) == ToUpper(s2);
	}
	std::string CreateUnique(const std::string& s)
	{
		return std::string();
	}
}