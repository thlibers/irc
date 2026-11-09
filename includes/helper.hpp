
#pragma once

#include <string>
#include <sstream>
#include <string>
#include <inttypes.h>

#define FR_RED "\e[31m"
#define FR_GREEN "\e[32m"
#define FR_YELLOW "\e[33m"
#define FR_BLUE "\e[34m"
#define RESET "\e[0m"

namespace helper
{
	bool	strisdigit(char *str) throw();
	bool	strisdigit(const char *str) throw();
	bool	strisdigit(const std::string &str) throw();

	/*
	 * @brief: Convert a C++ string to any value
	 * @return:
	 *     - The value of the string in a the desired type
	 */
	template <typename T>
	T		strtoany(const std::string str) throw()
	{
		std::stringstream ss(str);
		T converted;
		ss >> converted;
		return converted;
	}

	uint16_t	checkPort(const char *cstr_port);
	uint16_t	checkPort(const std::string &str);

	std::string checkPassword(char *_str);
	std::string checkPassword(std::string &_str);

	std::string	strtrim(char *c_str) throw();
	std::string	strtrim(std::string &str) throw();
}
