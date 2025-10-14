#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>

#ifndef CUSTOM_ERROR_MESSAGE
#define CUSTOM_ERROR_MESSAGE 0
#endif

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

bool isdigit_inrange(std::string str, size_t start, size_t end)
{
	if (str.empty() || start >= str.size() || end > str.size() || start >= end)
		return false;
	for (size_t i = start; i < end; ++i)
	{
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

bool isDateValid(int year, int month, int day)
{
	if (year < 2009 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;
	if (year == 2009 && month == 1 && day < 2)
		return false;
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		return false;
	if (month == 2)
	{
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) // Leap year
		{
			if (day <= 29)
				return true;
			else
				return false;
		}
		else if (day > 28)
			return false;
	}
	return true;
}

bool isLineOK(const std::string &line, float &priceValue)
{
	size_t line_len;

	line_len = line.size();
	if (line_len >= 1 && line[0] == '#')
		return false;
	if (line_len < minimalSizeOfDataPrice || line_len > maximalSizeOfDataPrice)
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cout << "Error: line is too small or too big => " << line << std::endl;
		else
			std::cout << "Error: bad input => " << line << std::endl;
		return false;
	}
	if (line[10] != ' ' || line[11] != '|' || line[12] != ' ')
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cout << "Error: separator badly placed or missing spaces => " << line << std::endl;
		else
			std::cout << "Error: bad input => " << line << std::endl;
		return false;
	}
	if (!isdigit_inrange(line, 0, 4) ||	!isdigit_inrange(line, 5, 7) ||	!isdigit_inrange(line, 8, 10))
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cout << "Error: Date format is not YYYY-MM-DD => " << line << std::endl;
		else
			std::cout << "Error: bad input => " << line << std::endl;
		return false;
	}
{
	int year, month, day;
	year = atoi(line.substr(0, 4).c_str());
	month = atoi(line.substr(5, 2).c_str());
	day = atoi(line.substr(8, 2).c_str());
	if (!isDateValid(year, month, day))
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cout << "Error: Date is not valid => " << line << std::endl;
		else
			std::cout << "Error: bad input => " << line << std::endl;
		return false;
	}
}
	if (line[4] != '-' || line[7] != '-')
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cout << "Error: bad date format => " << line << std::endl;
		else
			std::cout << "Error: bad input => " << line << std::endl;
		return false;
	}
	priceValue = atof(line.substr(13).c_str());
	if (priceValue < 0)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return false;
	}
	if (priceValue > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}

void makeCalculation(const BitcoinExchange &btc, const std::string &date, float price)
{
	std::map<std::string, float>::const_iterator it = btc.findNearest(date);
	if (it != btc.end())
	{
		std::cout << date << " => " << price << " = " << price * it->second << std::endl;
	}
	else
	{
		std::cout << "Error: date not found => " << date << std::endl;
	}
}

void compareFiles(const BitcoinExchange &btc, std::ifstream &file)
{
	std::string date;
	std::string price;
	float priceValue;

	for (std::string line; getline(file, line);)
	{
		if (!isLineOK(line, priceValue))
			continue;
		date = line.substr(0, 10);
		makeCalculation(btc, date, priceValue);
	}
}
