#include "includes/namespace/helper.hpp"
#include <cstring>

/*
 * @brief: Function that allocated and copy a string in a second one without
 *     using malloc like the real function (use new instead)
 * @return:
 *     - The copy of the string to copy
 * @throw:
 *     - std::bad_alloc if a memory allocation failed
 */
char	*helper::strdup(char *tocpy)
{
	char		*str = NULL;
	std::size_t	sz;

	if (std::strlen(tocpy) == 0)
	{
		sz = 0;
		str = new char[sz + 1];
		str[sz] = '\0';
	}
	else
	{
		sz = std::strlen(tocpy);
		str = new char[sz + 1];
		std::memcpy(str, tocpy, sz + 1);
	}

	return (str);
}
char	*helper::strdup(const char *tocpy)
{
	char		*str = NULL;
	std::size_t	sz;

	if (std::strlen(tocpy) == 0)
	{
		sz = 0;
		str = new char[sz + 1];
		str[sz] = '\0';
	}
	else
	{
		sz = std::strlen(tocpy);
		str = new char[sz + 1];
		std::memcpy(str, tocpy, sz + 1);
	}

	return (str);
}
