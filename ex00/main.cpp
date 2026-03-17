#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>

/* Function prototypes for additional tests */
static bool evaluate_args(int argc, char **argv);
static bool prepare_file(std::ifstream &file, const char* filename);

/* Mandatory main function */
int main(int argc, char **argv)
{
	if (evaluate_args(argc, argv) != 0)
		return (1);
	
	std::string ifileName = argv[1];
	if (ifileName.empty())
		return (1);
	std::ifstream file(ifileName.c_str(), std::ios_base::in);
	if (!file.is_open() || prepare_file(file, argv[1]) != 0)
		return (1);
	try
	{
		BitcoinExchange btc;
		// btc.enableReplaceDuplicatesWithLast();
		btc.compareFiles(file);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return (1);
	}
	return (0);
}

/**
 * @brief Evaluate if the args are valid
 * 
 * @param[in] argc number of argument 
 * @param[in] argv list of strings containing the arguments
 * @return true invalid
 * @return false invalid
 */
bool evaluate_args(int argc, char **argv)
{
	if (argc == 2)
		return (false);
	if (argc == 1)
	{
		std::cout << "Error : could not open file." << std::endl;
		return (true);
	}
	std::cout << "Error\nToo many arguments\n";
	std::cout << "\"" << argv[0] << " ";
	for (int i = 1; i < argc; i++)
	{
		std::cout << argv[i];
		if (i != argc - 1)
			std::cout << " ";
	}
	std::cout << "\" is invalid" << std::endl;
	std::cout << "Usage:\n"  << argv[0] << " FILE\nFILE file representing a database of date/prices of bitcoin" << std::endl;
	return (true);
}

bool prepare_file(std::ifstream &file, const char* filename)
{
	if (file.is_open() == false || file.fail())
	{
		std::cout << "Error: could not open file " << filename << std::endl;
		return (true);
	}
	std::string header;
	// Read the header line and check if it is valid
	getline(file, header);
	if (header != "date | value")
	{
		std::cout << "Error: bad header => " << header << std::endl;
		return (true);
	}
	return (false);
}
