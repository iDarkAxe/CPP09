#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

/* Function prototypes for additional tests */
int evaluate_args(int argc, char **argv);
void makeCalculation(const BitcoinExchange &btc, const std::string &date, float price);
bool isLineOK(const std::string &line);
void compareFiles(const BitcoinExchange &btc, std::ifstream &file);

const size_t minimalSizeOfDataPrice = 14; // 10 for date, 3 for space and sep, 1 for price
const size_t maximalSizeOfDataPrice = 24; // 10 for date, 3 for space and sep, 11 for price
const char *databaseFileName = "data.csv";

/* Mandatory main function */
int main(int argc, char **argv)
{
	if (evaluate_args(argc, argv) != 0)
		return (1);
	
	std::ifstream file(argv[1], std::ios_base::in);
	if (file.is_open() == false || file.fail())
	{
		std::cout << "Error: could not open file " << argv[1] << std::endl;
		return (0);
	}
	std::string header;
	// Read the header line and check if it is valid
	getline(file, header);
	if (header != "date | value")
	{
		std::cout << "Error: bad header => " << header << std::endl;
		return (0);
	}

	BitcoinExchange btc;
	try
	{
		btc.loadDataFromFile(databaseFileName);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return (1);
	}
	compareFiles(btc, file);
	return (0);
}

int evaluate_args(int argc, char **argv)
{
	if (argc == 2)
		return (0);
	if (argc == 1)
	{
		std::cout << "Error : could not open file" << std::endl;
		return (1);
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
	return (1);
}

bool isLineOK(const std::string &line)
{
	if (line.size() < minimalSizeOfDataPrice || line.size() > maximalSizeOfDataPrice)
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return false;
	}
	// Check date format : YYYY-MM-DD, YYYY is 4 digits, MM is 2 digits, DD is 2 digits, all positive, dash-separated, MM is between 01-12 and DD is between 01-31
	// Check separator : '|' at position 10
	// Check price format : only positive, int, float, or double
	return true;
}

void makeCalculation(const BitcoinExchange &btc, const std::string &date, float price)
{
	BitcoinExchange::const_iterator it = btc.find(date);
	if (it != btc.end())
	{
		std::cout << date << " => " << price << " = " << price * it->second << std::endl;
		// std::cout << "Date: " << it->first << ", Price: " << it->second << std::endl;
	}
	else
	{
		std::cout << "Error: date not found => " << date << std::endl;
		// std::cout << "Error: not found need to search after or before => " << date << std::endl;
	}
}

void compareFiles(const BitcoinExchange &btc, std::ifstream &file)
{
	std::string date;
	std::string price;
	float priceValue;

	for (std::string line; getline(file, line);)
	{
		if (!isLineOK(line))
			continue;
		date = line.substr(0, 10);
		price = line.substr(13);
		priceValue = atof(price.c_str());
		if (priceValue < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (priceValue > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}
		makeCalculation(btc, date, priceValue);
	}
}
