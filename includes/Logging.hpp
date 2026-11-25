
#pragma once

#include <string>
#include <sstream>
#include <cstddef>

class Logging
{
	public:
		typedef enum e_logLevel
		{
			INFORMATION = 0x0,
			OK = 0x1,
			WARNING,
			ERROR,
		} t_logLevel;

		/*
		 * @brief: Temporary stream that accumulates the data pushed with
		 *         operator<< and prints the complete colored line in his
		 *         destructor (so at the end of the statement)
		 * @note:
		 *     - Usage: Logging::info() << "New client fd: " << fd;
		 *     - The buffer is behind a pointer because a std::ostringstream
		 *       cannot be copied, a Stream returned by value needs an
		 *       accessible copy constructor
		 */
		class Stream
		{
			public:
				~Stream(void);
				Stream	&operator<<(const char *message);
				Stream	&operator<<(const std::string &message);
				Stream	&operator<<(int number);
				Stream	&operator<<(size_t number);
				Stream	&operator<<(char c);
			private:
				friend class Logging;
				Stream(t_logLevel level);
				/*
				 * Has to take a const reference (unlike the copy constructors
				 * of the rest of the codebase): Stream is returned by value,
				 * so the copy must be able to bind to the returned temporary
				 * even when the compiler elides the call
				 */
				Stream(const Stream &cpy);
				mutable std::ostringstream	*_buffer;
				t_logLevel					_level;
		};

		static Stream	info(void);
		static Stream	ok(void);
		static Stream	warn(void);
		static Stream	error(void);

		static void	write(const std::string &message, t_logLevel level);
		static void	write(const char *message, t_logLevel level);
};
