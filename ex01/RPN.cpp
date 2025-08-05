#include "RPN.hpp"
#include <iostream>
#include <sstream>

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

/**
 * @brief print All the strings stored in queue
 * Crazy slow as it needs to unroll the queue to print it, adding it temporarly in another queue and 
 * 
 */
void RPN::printAll(void)
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

uint8_t RPN::checkArguments(std::string& item, char sep)
{
	size_t item_len = 0;

	if (item.empty() || item.find(sep) != std::string::npos || (item.size() == 1 && item[0] == '\''))
	{
		return (1);
		// continue;
	}
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

void RPN::store(std::string& input, std::string& separator)
{
	(void)input;
	(void)separator;
	size_t sep_len = separator.size();

	if (input.empty() || sep_len == 0)
		throw ArgumentEmptyException();

	// if (input.find(separator) == std::string::npos)
	// {
	// 	this->fifo.push(input);
	// 	return;
	// }

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
#include <cstdlib>
double RPN::calculate(void)
{
	std::queue<std::string> backup = this->fifo;
	std::string sign;
	double argLeft;
	double argRight;
	// double result = 0;

	std::ostringstream strs;

	size_t it = 0;
	if (this->fifo.size() < 3)
		throw NumberOfArgumentTooLowException();
	argRight = strtod(this->fifo.front().c_str(), NULL);
	this->fifo.pop();
	argLeft = strtod(this->fifo.front().c_str(), NULL);
	this->fifo.pop();

	sign = this->fifo.front();
	this->fifo.pop();

	std::cout << "sign " << sign << " argLeft " << argLeft << " argRigth" << argRight << std::endl;
	if (sign == "+")
		argLeft = (argLeft + argRight);
	else if (sign == "-")
		argLeft = (argLeft - argRight);
	else if (sign == "*")
		argLeft = (argLeft * argRight);
	else if (sign == "/")
		argLeft = (argLeft / argRight);
	else
		throw SignDontExistException();
	std::cout << "curr res: " << strs.str() << std::endl;
	printAll();
	while (this->fifo.size() != 0)
	{
		argRight = strtod(this->fifo.front().c_str(), NULL);
		this->fifo.pop();
		sign = this->fifo.front();
		this->fifo.pop();

		std::cout << "sign " << sign << " argLeft " << argLeft << " argRigth" << argRight << std::endl;
		if (sign == "+")
			argLeft = (argLeft + argRight);
		else if (sign == "-")
			argLeft = (argLeft - argRight);
		else if (sign == "*")
			argLeft = (argLeft * argRight);
		else if (sign == "/")
			argLeft = (argLeft / argRight);
		else
			throw SignDontExistException();
		std::cout << "curr res: " << strs.str() << std::endl;
		it++;
	}
	fifo = backup;
	return (argLeft);
}

/* 
while (!queue.empty()) {
    token = queue.front();
    queue.pop();
    
    if (isNumber(token)) {
        stack.push(token);
    } else if (isOperator(token)) {
        b = stack.top(); stack.pop();
        a = stack.top(); stack.pop();
        result = calculate(a, token, b);
        stack.push(result);
    }
} 
*/

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

/* 
Work Pretty good for really basic RPN.
It will work for "8 9 * 9 - 9 - 9 - 4 - 1 +" but not for "3 10 5 +" 
*/
/* 
double RPN::calculate(void)
{
	std::queue<std::string> backup = this->fifo;
	std::string sign;
	double argLeft;
	double argRight;

	std::ostringstream strs;

	size_t it = 0;
	if (this->fifo.size() < 3)
		throw NumberOfArgumentTooLowException();
	argRight = strtod(this->fifo.front().c_str(), NULL);
	this->fifo.pop();
	argLeft = strtod(this->fifo.front().c_str(), NULL);
	this->fifo.pop();

	sign = this->fifo.front();
	this->fifo.pop();

	std::cout << "sign " << sign << " argLeft " << argLeft << " argRigth" << argRight << std::endl;
	if (sign == "+")
		argLeft = (argLeft + argRight);
	else if (sign == "-")
		argLeft = (argLeft - argRight);
	else if (sign == "*")
		argLeft = (argLeft * argRight);
	else if (sign == "/")
		argLeft = (argLeft / argRight);
	else
		throw SignDontExistException();
	std::cout << "curr res: " << strs.str() << std::endl;
	printAll();
	while (this->fifo.size() != 0)
	{
		argRight = strtod(this->fifo.front().c_str(), NULL);
		this->fifo.pop();
		sign = this->fifo.front();
		this->fifo.pop();

		std::cout << "sign " << sign << " argLeft " << argLeft << " argRigth" << argRight << std::endl;
		if (sign == "+")
			argLeft = (argLeft + argRight);
		else if (sign == "-")
			argLeft = (argLeft - argRight);
		else if (sign == "*")
			argLeft = (argLeft * argRight);
		else if (sign == "/")
			argLeft = (argLeft / argRight);
		else
			throw SignDontExistException();
		std::cout << "curr res: " << strs.str() << std::endl;
		it++;
	}
	fifo = backup;
	return (argLeft);
} */
