#ifndef RPN_HPP
#define RPN_HPP

#include <queue>
#include <string>
#include <stdint.h>

#ifndef ENABLE_MULTIPLE_DIGITS
#define ENABLE_MULTIPLE_DIGITS 0
#endif

#ifndef ENABLE_DEBUG
#define ENABLE_DEBUG 1
#endif

class RPN
{
private:
	std::queue<std::string> fifo;

	uint8_t checkArguments(std::string &item, char sep);

public:
	RPN();
	~RPN();
	RPN(const RPN &f);
	RPN &operator=(const RPN &other);

	void printAll(void);
	void store(std::string &input, std::string &separator);
	double calculate(void);

	class ArgumentEmptyException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class ArgumentTooLongException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class ArgumentUnexpectedException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class SignDontExistException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class NumberOfArgumentTooLowException : public std::exception
	{
		public:
			const char *what() const throw();
	};
};

#endif
