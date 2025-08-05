#include "RPN.hpp"

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

void RPN::store(std::string input, std::string separator)
{
	(void)input;
	(void)separator;
}

double RPN::calculate(void)
{
	double result = 0;

	return (result);
}
