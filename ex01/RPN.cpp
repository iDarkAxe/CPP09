#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>

RPN::RPN()
{
	;
}

RPN::~RPN()
{
	;
}

RPN::RPN(const RPN &f)
{
	*this = f;
}

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
		return *this;
	return *this;
}

template <>
std::string RPN::getAndPop<std::string>(void)
{
	if (fifo.empty())
	{
		if (MAKE_UNAUTORIZED_ACCESS_THROW)
			throw UseOfEmptyContainerException();
		return "";
	}
	std::string value = fifo.front();
	fifo.pop();
	return value;
}

template <>
double RPN::getAndPop<double>(void)
{
	if (result.empty())
	{
		if (MAKE_UNAUTORIZED_ACCESS_THROW)
			throw UseOfEmptyContainerException();
		return 0;
	}
	double value = result.top();
	result.pop();
	return value;
}

/**
 * @brief print All the strings stored in queue
 * Crazy slow as it needs to unroll the queue to print it, adding it temporarly in another queue and 
 * 
 */
void RPN::printAllFifo(void)
{
	std::queue<std::string> backup = this->fifo;

	size_t it = 0;
	while (this->fifo.size() != 0)
	{
		std::cout << "it[" << it << "]: '" << this->fifo.front() << "'" << std::endl;
		this->fifo.pop();
		it++;
	}
	fifo = backup;
}

/**
 * @brief print All the strings stored in queue
 * Crazy slow as it needs to unroll the queue to print it, adding it temporarly in another queue and 
 * 
 */
void RPN::printAllResult(void)
{
	std::stack<double> backup = this->result;

	size_t it = 0;
	while (this->result.size() != 0)
	{
		std::cout << "it[" << it << "]: '" << this->result.top() << "'" << std::endl;
		this->result.pop();
		it++;
	}
	this->result = backup;
}


uint8_t RPN::checkArguments(std::string& item, char sep)
{
	size_t item_len = 0;

	if (item.empty() || item.find(sep) != std::string::npos || (item.size() == 1 && item[0] == '\''))
		return (1);
	item_len = item.size();
	if (!ENABLE_MULTIPLE_DIGITS && item_len > 1)
	{
		if (ENABLE_DEBUG)
			std::cout << "Faulty argument is '" << item << "'\n";
		throw ArgumentTooLongException();
	}
	if (ENABLE_MULTIPLE_DIGITS && item_len >= 3 && item[0] == '\'' && item[item_len - 1] == '\'')
		item = item.substr(1, item_len - 2);
	if (!ENABLE_MULTIPLE_DIGITS)
	{
		if (!isdigit(item[0]) && item[0] != '+' && item[0] != '-'&& item[0] != '/'&& item[0] != '*')
		{
			if (ENABLE_DEBUG)
				std::cout << "Faulty argument is '" << item << "'\n";
			throw ArgumentUnexpectedException();
		}
	}
	return (0);
}

/**
 * @brief Store at the end of the fifo the input
 * 
 * @param input input
 * @param separator list of characters to separate the inputs
 */
void RPN::store(std::string& input, std::string& separator)
{
	(void)input;
	(void)separator;
	size_t sep_len = separator.size();

	if (input.empty() || sep_len == 0)
		throw ArgumentEmptyException();

    std::stringstream ss (input);
    std::string item;
	for (size_t i = 0; i < sep_len; i++)
	{
		while (getline(ss, item, separator[i]))
		{
			if (checkArguments(item, separator[i]) != 0)
				continue;
			this->fifo.push(item);
		}
	}
}

#if ENABLE_DECIMALS == 1
bool RPN::isNumber(std::string& token)
{
	for (size_t i = 0; token[i]; i++)
	{
		if (!isdigit(token[i]) && token[i] != '.' && token[i] != 'f')
			return (false);
	}
	return (true);
}
#else
bool RPN::isNumber(std::string& token)
{
	for (size_t i = 0; token[i]; i++)
	{
		if (!isdigit(token[i]))
			return (false);
	}
	return (true);
}
#endif

bool RPN::isOperator(std::string& token)
{
	if (token == "+" || token == "-" || token == "*" || token == "/")
		return (true);
	return (false);
}

double RPN::makeSimpleCalc(double argLeft, std::string& sign, double argRight)
{
	if (sign == "+")
		return (argLeft + argRight);
	if (sign == "-")
		return (argLeft - argRight);
	if (sign == "*")
		return (argLeft * argRight);
	if (sign == "/")
		return (argLeft / argRight);
	else
		throw;
}

/**
 * @brief Calculate what's in the queue
 * this->result still handles the currentResult at the end, 
 * don't forget to trash or next calculate will use it
 * 
 * @return double 
 */
double RPN::calculate(void)
{
	std::queue<std::string> backup = this->fifo;
	std::string token;
	double argLeft, argRight;
	double currentResult;

	while (!this->fifo.empty()) 
	{
		token = getAndPop<std::string>();
		if (isNumber(token))
			this->result.push(strtod(token.c_str(), NULL));
		else if (isOperator(token))
		{
			argRight = getAndPop<double>();
			argLeft = getAndPop<double>();
			currentResult = makeSimpleCalc(argLeft, token, argRight);
			this->result.push(currentResult);
		}
	}
	if (result.size() != 1)
	{
		if (ENABLE_DEBUG)
		{
			std::cout << "Fifo: " << std::endl;
			printAllFifo();
			std::cout << "Result: " << std::endl;
			printAllResult();
		}
		throw ContainerNotCompletelyEmptiedException();
	}
	fifo = backup;
	return (currentResult);
}

void RPN::trashFifo(void)
{
	fifo = std::queue<std::string>();
}

void RPN::trashResult(void)
{
	result = std::stack<double>();
}

const char* RPN::ArgumentEmptyException::what() const throw()
{
	return "At least one argument is empty";
}

const char* RPN::ArgumentTooLongException::what() const throw()
{
	return "Arguments should be one character long";
}

const char* RPN::ArgumentUnexpectedException::what() const throw()
{
	return "This argument is not registered currently";
}

const char* RPN::SignDontExistException::what() const throw()
{
	return "This sign doesn't exist";
}

const char* RPN::NumberOfArgumentTooLowException::what() const throw()
{
	return "It needs at least 3 argument to work";
}

const char* RPN::UseOfEmptyContainerException::what() const throw()
{
	return "This container is already empty !";
}

const char* RPN::ContainerNotCompletelyEmptiedException::what() const throw()
{
	return "This container still has content ! (maybe needs an operator ?)";
}
