
#include "includes/Logging.hpp"

/*  */
#include <iostream>
#include <new>

/*
 * ==================
 * = LOGGING STREAM =
 * ==================
 */

/*
 * @name: Stream
 * @brief: Open a new buffer for the given log level
 */
Logging::Stream::Stream(t_logLevel level) :
	_buffer(new(std::nothrow) std::ostringstream),
	_level(level)
{ ; }

/*
 * @name: Stream (copy)
 * @brief: Steal the buffer of cpy instead of copying it, a
 *         std::ostringstream cannot be copied
 */
Logging::Stream::Stream(const Stream &cpy) :
	_buffer(cpy._buffer),
	_level(cpy._level)
{
	cpy._buffer = NULL;
}

/*
 * @name: ~Stream
 * @brief: Print the accumulated line with the color of his log level
 * @note: Does nothing if the buffer was stolen by a copy
 */
Logging::Stream::~Stream(void)
{
	if (!this->_buffer)
		return ;

	if (this->_level == Logging::INFORMATION)
		std::cout << "\x1b[K\r\e[0;34m[?] : " << this->_buffer->str() << "\e[0m" << std::endl;
	else if (this->_level == Logging::OK)
		std::cout << "\x1b[K\r\e[0;32m[+] : " << this->_buffer->str() << "\e[0m" << std::endl;
	else if (this->_level == Logging::WARNING)
		std::cerr << "\x1b[K\r\e[0;33m[⚠] : " << this->_buffer->str() << "\e[0m" << std::endl;
	else if (this->_level == Logging::ERROR)
		std::cerr << "\x1b[K\r\e[0;31m[!] : " << this->_buffer->str() << "\e[0m" << std::endl;
	else
		std::cerr << this->_buffer->str() << std::endl;
	delete this->_buffer;
}
/*
 * @name: operator<<
 * @brief: Append data to the accumulated line
 */
Logging::Stream	&Logging::Stream::operator<<(const char *message)
{
	if (this->_buffer)
		*this->_buffer << message;
	return (*this);
}
Logging::Stream	&Logging::Stream::operator<<(const std::string &message)
{
	if (this->_buffer)
		*this->_buffer << message;
	return (*this);
}
Logging::Stream	&Logging::Stream::operator<<(int number)
{
	if (this->_buffer)
		*this->_buffer << number;
	return (*this);
}
Logging::Stream	&Logging::Stream::operator<<(size_t number)
{
	if (this->_buffer)
		*this->_buffer << number;
	return (*this);
}
Logging::Stream	&Logging::Stream::operator<<(char c)
{
	if (this->_buffer)
		*this->_buffer << c;
	return (*this);
}

/*
 * ======================
 * = LOGGING ENTRY POINT =
 * ======================
 */

Logging::Stream	Logging::info(void)
{ return (Logging::Stream(Logging::INFORMATION)); }
Logging::Stream	Logging::ok(void)
{ return (Logging::Stream(Logging::OK)); }
Logging::Stream	Logging::warn(void)
{ return (Logging::Stream(Logging::WARNING)); }
Logging::Stream	Logging::error(void)
{ return (Logging::Stream(Logging::ERROR)); }

/*
 * @name: write
 * @brief: Log an already built message, the color is choosen with the
 *         log level
 * @param: message, the message to print on the screen
 * @param: level, the log level of the message
 */
void	Logging::write(const std::string &message, t_logLevel level)
{
	Logging::Stream(level) << message;
}
void	Logging::write(const char *message, t_logLevel level)
{
	Logging::Stream(level) << message;
}
