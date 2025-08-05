#ifndef RPN_HPP
#define RPN_HPP

#include <queue>
#include <stack>
#include <string>
#include <stdint.h>

#ifndef ENABLE_MULTIPLE_DIGITS
#define ENABLE_MULTIPLE_DIGITS 1
#endif

#if ENABLE_MULTIPLE_DIGITS == 1
# ifndef ENABLE_DECIMALS
# define ENABLE_DECIMALS 1
# endif
#endif

#ifndef MAKE_UNAUTORIZED_ACCESS_THROW
#define MAKE_UNAUTORIZED_ACCESS_THROW 1
#endif

#ifndef ENABLE_DEBUG
#define ENABLE_DEBUG 0
#endif

class RPN
{
private:
	std::queue<std::string>	fifo;
	std::stack<double> result;

	uint8_t checkArguments(std::string &item, char sep);
	bool isNumber(std::string& token);
	bool isOperator(std::string& token);
	double makeSimpleCalc(double argLeft, std::string& sign, double argRight);


public:
	RPN();
	~RPN();
	RPN(const RPN &f);
	RPN &operator=(const RPN &other);

	template <typename T>
	T getAndPop(void);

	void printAllFifo(void);
	void printAllResult(void);
	void store(std::string &input, std::string &separator);
	double calculate(void);
	void trashFifo(void);
	void trashResult(void);

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
	class UseOfEmptyContainerException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class ContainerNotCompletelyEmptiedException : public std::exception
	{
		public:
			const char *what() const throw();
	};
};

#endif
