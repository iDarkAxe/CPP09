#ifndef RPN_HPP
#define RPN_HPP

#include <queue>
#include <stack>
#include <string>
#include <stdint.h>

#ifndef ENABLE_MULTIPLE_DIGITS
#define ENABLE_MULTIPLE_DIGITS 0
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
	T getAndPop(void);  // Déclaration générique pour les deux façons

	void printAllFifo(void);
	void printAllResult(void);
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
	class UseOfEmptyContainerException : public std::exception
	{
		public:
			const char *what() const throw();
	};
};

#endif
