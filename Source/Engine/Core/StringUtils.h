#pragma once
#include <string>
namespace lady
{
	class StringUtils
	{
	public:
		std::string ToUpper(std::string& s);
		std::string ToLower(std::string& s);
		bool IsEqualIgnoreCase(const std::string& s1, const std::string& s2);
		std::string CreateUnique(const std::string& s);
	};
}