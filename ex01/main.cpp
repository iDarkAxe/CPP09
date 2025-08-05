#include "RPN.hpp"
#include <iostream>

#ifndef DEBUG
#define DEBUG 1
#endif

/* Mandatory main function */
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		if (DEBUG)
		{
			std::cout << "Usage: " << argv[0] << " \"[RPN NOTATION]\"\n" << "Simple Reverse Polish Notation Calculator\n\n" << "[RPN NOTATION]:\n0-9\t\tAll digits (numbers below 10)\n+, -, *, /\tSupport addition, soustraction, multiplication and division.\n\n" << "Example:\n" << argv[0] << " \"7 8 * 9 / 3 * 1 2 - +\"\n-> 17.66"  << '\n';
		}
		else
			std::cout << "Error\n";
		return (0);
	}

	RPN rpn;
	try 
	{
		rpn.store(argv[1], " ");
		double result = rpn.calculate();
		std::cout << result << std::endl;
	}
	catch(const std::exception& e)
	{
		if (DEBUG)
			std::cout << e.what() << '\n';
		else
			std::cout << "Error\n";
		std::cout << std::endl;
		return (1);
	}
}
