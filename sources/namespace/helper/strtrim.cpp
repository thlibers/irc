
#include "includes/namespace/helper.hpp"


/*
 * @brief: Function will trim every whitespace on a string and return the
 *     timmed string.
 * @return:
 *     - The string without whitespace at the begining and the end
 */
std::string	helper::strtrim(char *str) throw()
{
	std::string cpy_str;
	std::size_t	size;
	std::size_t first = 0;
	std::size_t last;

	if (!str)
		return ("");

	cpy_str = str;
	size = cpy_str.size();
	last = size;

	while (first < last && std::isspace(static_cast<uint8_t>(cpy_str[first])))
		++first;
	while (last > first && std::isspace(static_cast<uint8_t>(cpy_str[last - 1])))
		--last;

	if (last != size)
		cpy_str.erase(last, size);
	if (first != 0)
		cpy_str.erase(0, first);
	return (cpy_str);
}

std::string	helper::strtrim(std::string &str) throw()
{
	std::string	cpy_str = str;
	std::size_t first = 0;
	std::size_t last;
	std::size_t	size;

	if (cpy_str.empty())
		return ("");

	size = cpy_str.size();
	last = size;

	while (first < last && std::isspace(static_cast<uint8_t>(cpy_str[first])))
		++first;
	while (last > first && std::isspace(static_cast<uint8_t>(cpy_str[last - 1])))
		--last;

	if (last != size)
		cpy_str.erase(last, size);
	if (first != 0)
		cpy_str.erase(0, first);
	return (cpy_str);
}

std::string	helper::strtrim(std::string &str, std::string set) throw()
{
	std::string	cpy_str = str;
	std::size_t first = 0;
	std::size_t last;
	std::size_t	size;

	if (cpy_str.empty())
		return ("");

	size = cpy_str.size();
	last = size;

	cpy_str.find_last_of(set);
	while (first < last && helper::isSet(static_cast<uint8_t>(cpy_str[first]), set))
		++first;
	while (last > first && helper::isSet(static_cast<uint8_t>(cpy_str[last - 1]), set))
		--last;

	if (last != size)
		cpy_str.erase(last, size);
	if (first != 0)
		cpy_str.erase(0, first);
	return (cpy_str);
}
