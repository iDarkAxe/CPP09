#include "RPN.hpp"
#include <iostream>

/* Tester prototype */
int tester(void);

/* Mandatory main function */
int main(int argc, char **argv)
{
	// return (tester());
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
	std::string argv_str = argv[1];
	std::string delimitor = " ";
	RPN rpn;
	try 
	{
		if (argv_str.empty())
			throw RPN::ArgumentEmptyException();
		rpn.store(argv_str, delimitor);
		double result = rpn.calculate();
		std::cout << result << std::endl;
		rpn.trashResult();
	}
	catch(const std::exception& e)
	{
		if (ENABLE_DEBUG)
			std::cout << "Error: " << e.what() << std::endl;
		else
			std::cout << "Error" << std::endl;
		return (1);
	}
}

int tester(void)
{
	std::vector<std::string> test_cases;
	test_cases.push_back("5 3 +");          				 // = 8
	test_cases.push_back("10 2 -");         				 // = 8
	test_cases.push_back("4 7 *");          				 // = 28
	test_cases.push_back("15 3 /");         				 // = 5

	// Tests avec nombres négatifs/décimaux
	test_cases.push_back("2.5 1.5 +");      				 // = 4
	test_cases.push_back("10 3 -");          				// = 7
	test_cases.push_back("0 5 -");          				 // = -5

	// Tests à 3 opérandes
	test_cases.push_back("3 10 5 + *");      				// = 45
	test_cases.push_back("15 7 1 1 + - /");  				// = 3
	test_cases.push_back("1 2 + 4 * 5 +");   				// = 17

	// Tests plus complexes
	test_cases.push_back("8 9 * 9 - 9 - 9 - 4 - 1 +");     	// = 42
	test_cases.push_back("2 3 4 * +");                       // = 14
	test_cases.push_back("100 25 / 2 *");                    // = 8
	test_cases.push_back("5 1 2 + 4 * + 3 -");              // = 14
	test_cases.push_back("3 4 + 2 * 1 -");                   // = 13

	std::string argv_str;
	std::string delimitor = " ";
	RPN rpn;
	try 
	{
		while (!test_cases.empty())
		{
			argv_str = test_cases.back();
			test_cases.pop_back();
			if (argv_str.empty())
				throw RPN::ArgumentEmptyException();
			std::cout << "testing with " << argv_str << ":\n\t";
			rpn.store(argv_str, delimitor);
			double result = rpn.calculate();
			std::cout << result << std::endl;
			rpn.trashResult();
			rpn.trashFifo();
		}
	}
	catch(const std::exception& e)
	{
		if (ENABLE_DEBUG)
			std::cout << e.what() << std::endl;
		else
			std::cout << "Error" << std::endl;
		return (1);
	}
	return (0);
}
