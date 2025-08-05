#include "RPN.hpp"
#include <iostream>

/* Mandatory main function */
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		if (ENABLE_DEBUG)
		{
			std::cout << "Usage: " << argv[0] << " \"[RPN NOTATION]\"\n" << "Simple Reverse Polish Notation Calculator\n\n" << "[RPN NOTATION]:\n0-9\t\tAll digits (numbers below 10)\n+, -, *, /\tSupport addition, soustraction, multiplication and division.\n\n" << "Example:\n" << argv[0] << " \"7 8 * 9 / 3 * 1 2 - +\"\n-> 17.66"  << '\n';
		}
		else
			std::cout << "Error\n";
		return (0);
	}
	std::string arv_str = argv[1];
	std::string delimitor = " ";
	RPN rpn;
	try 
	{
		if (arv_str.empty())
			throw RPN::ArgumentEmptyException();
		rpn.store(arv_str, delimitor);
		rpn.printAll();
		double result = rpn.calculate();
		std::cout << result << std::endl;
	}
	catch(const std::exception& e)
	{
		if (ENABLE_DEBUG)
			std::cout << e.what() << std::endl;
		else
			std::cout << "Error" << std::endl;
		return (1);
	}
}
