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
		#if MAKE_UNAUTORIZED_ACCESS_THROW == 1
			throw UseOfEmptyContainerException();
		#endif
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
		#if MAKE_UNAUTORIZED_ACCESS_THROW == 1
			throw UseOfEmptyContainerException();
		#endif
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
void RPN::printAllFifo(void) const
{
	std::queue<std::string> backup = this->fifo;

	size_t it = 0;
	while (backup.size() != 0)
	{
		std::cout << "it[" << it << "]: '" << backup.front() << "'" << std::endl;
		backup.pop();
		it++;
	}
}

/**
 * @brief print All the strings stored in queue
 * Crazy slow as it needs to unroll the queue to print it, adding it temporarly in another queue and 
 * 
 */
void RPN::printAllResult(void) const
{
	std::stack<double> backup = this->result;

	size_t it = 0;
	while (backup.size() != 0)
	{
		std::cout << "it[" << it << "]: '" << backup.top() << "'" << std::endl;
		backup.pop();
		it++;
	}
}

/**
 * @brief Verify the Argument given in `item` and throw if it is invalid
 * 
 * @param[in,out] item argument to check (may be modified if between quotes and ENABLE_MULTIPLE_DIGITS)
 * @return uint8_t 
 */
uint8_t RPN::checkArguments(std::string& item) const
{
	if (item.empty())
		return (1);

	size_t len = item.size();

	if (len == 1)
	{
		if (isdigit(item[0]) || item == "+" || item == "-" || item == "*" || item == "/")
			return (0);
		else
		{
			if (ENABLE_DEBUG)
				std::cout << "Faulty argument is '" << item << "'\n";
			throw ArgumentUnexpectedException();
		}
	}

	if (!ENABLE_MULTIPLE_DIGITS && len > 1)
	{
		if (ENABLE_DEBUG)
			std::cout << "Faulty argument is '" << item << "'\n";
		throw ArgumentTooLongException();
	}

	// ENABLE_MULTIPLE_DIGITS == 1 as we are here
	// Gestion des nombres entre quotes
	if (len >= 3 && item[0] == '\'' && item[len - 1] == '\'')
	{
		item = item.substr(1, len - 2);
		len = item.size();
	}
	
	// Validation du token
	if (len > 1 && !isdigit(item[0]) && item[0] != '+' && item[0] != '-' && item[0] != '*' && item[0] != '/' && item != ".f")
	{
		if (ENABLE_DEBUG)
				std::cout << "Faulty argument is '" << item << "'\n";
		throw ArgumentUnexpectedException();
	}
	for (size_t i = 1; i < len; ++i)
	{
		if (!isdigit(item[i]) && item != "+" && item != "-" && item != "*" && item != "/" && item[i] != '.' && item[i] != 'f')
		{
			if (ENABLE_DEBUG)
				std::cout << "Faulty argument is '" << item << "'\n";
			throw ArgumentUnexpectedException();
		}
	}

	// Specific decimal checks
	size_t dot_pos = item.find_first_of('.');
	size_t f_pos = item.find_first_of('f');

	if (dot_pos == std::string::npos && f_pos == std::string::npos)
		return (0);
	if (dot_pos != item.find_last_of('.') || f_pos != item.find_last_of('f')) // plus d'un '.' ou 'f'
	{
		if (ENABLE_DEBUG)
			std::cout << "Faulty argument is '" << item << "'\n";
		throw ArgumentUnexpectedException();
	}

	// s'il y a un 'f' il doit être à la fin
	if (f_pos != std::string::npos && f_pos != len - 1)
	{
		if (ENABLE_DEBUG)
			std::cout << "Faulty argument is '" << item << "'\n";
		throw ArgumentUnexpectedException();
	}
	return (0);
}

/**
 * @brief Store at the end of the fifo the input
 * 
 * @param input input
 * @param separator list of characters to separate the inputs
 */
void RPN::store(const std::string& input, const std::string& separator)
{
	if (input.empty() || separator.empty())
		throw ArgumentEmptyException();

	size_t pos = 0;
	size_t sep_len = separator.length();

	while (true)
	{
		size_t next = input.find(separator, pos);
		std::string token;

		if (next == std::string::npos)
			token = input.substr(pos);
		else
			token = input.substr(pos, next - pos);


		if (checkArguments(token) == 0)
			fifo.push(token);

		if (next == std::string::npos)
			break;

		pos = next + sep_len;
	}
}

#if ENABLE_DECIMALS == 1
bool RPN::isNumber(std::string& token) const
{
	if (token.empty())
		return (false);
	size_t i;
	if (token.size() > 1 && (token[0] == '+' || token[0] == '-'))
		i = 1; // skip sign
	else
		i = 0;
	for (; token[i]; i++)
	{
		if (!isdigit(token[i]) && token[i] != '.' && token[i] != 'f')
			return (false);
	}
	return (true);
}
#else
bool RPN::isNumber(std::string& token) const
{
	for (size_t i = 0; token[i]; i++)
	{
		if (!isdigit(token[i]))
			return (false);
	}
	return (true);
}
#endif

bool RPN::isOperator(std::string& token) const
{
	if (token == "+" || token == "-" || token == "*" || token == "/")
		return (true);
	return (false);
}

double RPN::makeSimpleCalc(double argLeft, std::string& sign, double argRight) const
{
	if (sign.size() != 1)
		throw RPN::SignDontExistException();
	switch (sign[0])
	{
		case '+':
			return (argLeft + argRight);
		case '-':
			return (argLeft - argRight);
		case '*':
			return (argLeft * argRight);
		case '/':
			if (argRight == 0)
				throw RPN::IllegalDivisionException();
			return (argLeft / argRight);
		default:
			throw RPN::SignDontExistException();
	}
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
	if (this->fifo.empty())
		throw UseOfEmptyContainerException();
	if (this->fifo.size() < 3)
		throw NumberOfArgumentTooLowException();
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
	if (!fifo.empty())
		fifo = std::queue<std::string>();
}

void RPN::trashResult(void)
{
	if (!result.empty())
		result = std::stack<double>();
}

void RPN::clear(void)
{
	this->trashFifo();
	this->trashResult();
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

const char* RPN::IllegalDivisionException::what() const throw()
{
	return "Division by zero is not allowed";
}
