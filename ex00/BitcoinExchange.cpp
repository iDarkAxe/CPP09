#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

#ifndef CUSTOM_ERROR_MESSAGE
#define CUSTOM_ERROR_MESSAGE 0
#endif

const size_t csv_minimalSizeOfDataPrice = 12; // 10 for date, 1 for sep, 1 for price
static const size_t ifile_minimalSizeOfDataPrice = 14; // 10 for date, 3 for space and sep, 1 for price
static const size_t ifile_maximalSizeOfDataPrice = 24; // 10 for date, 3 for space and sep, 11 for price

BitcoinExchange::BitcoinExchange()
{
	this->replaceDuplicatesWithLast = 0;
	this->loadDataFromFile("data.csv");
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &f)
{
	*this = f;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		return *this;
	return *this;
}

void BitcoinExchange::printPair(const std::string& date)
{
	BitcoinExchange::dataType::iterator it;

	try {
		it = this->data.find(date);
		std::cout << "Date: " << it->first << ", Price: " << it->second << std::endl;
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return;
	}
}

void BitcoinExchange::printAll(void) const
{
	BitcoinExchange::dataType::const_iterator it;
	BitcoinExchange::dataType::const_iterator end = this->data.end();

	for (it = this->data.begin(); it != end; ++it)
	{
		std::cout << it->first << " => " << it->second << " = " << "placeholder" << std::endl;
	}
}

/**
 * @brief Load the data from a filename and store each line properly in the class
 * 
 * @param[in] filename file to read 
 */
void BitcoinExchange::loadDataFromFile(const std::string& filename)
{
	this->data.clear();
	std::ifstream database(filename.c_str(), std::ios_base::in);

	if (database.is_open() == false || database.fail())
	{
		std::cout << "Error: could not open file " << filename << std::endl;
		std::cout << "File should be placed at the same subfolder level as this program" << std::endl;
		throw DatabaseNotFoundException();
	}
	{
		std::string header;
		// Read the header line and check if it is valid
		
		if (std::getline(database, header).fail() || header != "date,exchange_rate")
		{
			std::cout << "Error: bad header => " << header << std::endl;
			throw WrongHeaderException();
		}
	}
	std::string line;
	std::string presumedDate;
	float presumedPrice;
	while (std::getline(database, line))
	{
		if (line.empty() || line[0] == '#')
			continue;
		if (line.size() < csv_minimalSizeOfDataPrice || line[csv_minimalSizeOfDataPrice - 2] != ',')
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			throw CSVFileWronglyFormattedException();
		}
		presumedDate = line.substr(0, 10);
		int year, month, day;
		year = atoi(line.substr(0, 4).c_str());
		month = atoi(line.substr(5, 2).c_str());
		day = atoi(line.substr(8, 2).c_str());
		if (!isDateValid(year, month, day))
		{
			std::cerr << "Data invalid but not throwing error => " << presumedDate << std::endl;
		}
		char *end;
		std::string subline = line.substr(11);
		presumedPrice = strtof(subline.c_str(), &end);
		if (end == subline.c_str()) {
			// std::cerr << "Error : conversion invalid\n";
			continue;
		}
		if (this->replaceDuplicatesWithLast)
		{
			insertResultType ret = this->data.insert(std::make_pair(presumedDate, presumedPrice));
			if (!ret.second)
				ret.first->second = presumedPrice;
		}
		else
		{
			this->data.insert(std::make_pair(presumedDate, presumedPrice));
		}
	}
}

BitcoinExchange::dataType::const_iterator BitcoinExchange::findNearest(const std::string &date) const
{
	BitcoinExchange::dataType::const_iterator it = this->data.lower_bound(date);

	if (it != this->data.end() && it->first == date) {
		return it;
	}

	// Data is before all stored dates
	if (it == this->data.begin()) {
		return it;
	}

	// Date is between two stored dates, we choose one before
	--it;
    return it;
}

bool BitcoinExchange::isdigit_inrange(const std::string& str, size_t start, size_t end) const
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

/**
 * @brief Verify if a date is valid (exist, existed or will exist)
 * 
 * @param[in] year year to verify 
 * @param[in] month month to verify 
 * @param[in] day day to verify 
 * @return true invalid
 * @return false invalid
 */
bool BitcoinExchange::isDateValid(int year, int month, int day) const
{
	if (month < 1 || month > 12 || day < 1 || day > 31 || year < 0) 
		return false;
	if (year <= 2009 && month == 1 && day < 2) // création du btc, pas de date antérieure
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

/**
 * @brief Verify if a line is OK
 * A line is ok if it is properly formatted
 * A line is invalid if the date is invalid or if the priceValue is to high
 * 
 * @param[in] line line to read 
 * @param[out] priceValue value rode in the line
 * @return true line OK
 * @return false line invalid
 */
bool BitcoinExchange::isLineOK(const std::string &line, float &priceValue)
{
	size_t line_len;

	line_len = line.size();
	if (line_len >= 1 && line[0] == '#')
		return false;
	if (line_len < ifile_minimalSizeOfDataPrice || line_len > ifile_maximalSizeOfDataPrice)
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cerr << "Error: line is too small or too big => " << line << std::endl;
		else
			std::cerr << "Error: bad input => " << line << std::endl;
		return false;
	}
	if (line[10] != ' ' || line[11] != '|' || line[12] != ' ')
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cerr << "Error: separator badly placed or missing spaces => " << line << std::endl;
		else
			std::cerr << "Error: bad input => " << line << std::endl;
		return false;
	}
	if (!isdigit_inrange(line, 0, 4) ||	!isdigit_inrange(line, 5, 7) ||	!isdigit_inrange(line, 8, 10))
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cerr << "Error: Date format is not YYYY-MM-DD => " << line << std::endl;
		else
			std::cerr << "Error: bad input => " << line << std::endl;
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
			std::cerr << "Error: Date is not valid => " << line << std::endl;
		else
			std::cerr << "Error: bad input => " << line << std::endl;
		return false;
	}
}
	if (line[4] != '-' || line[7] != '-')
	{
		if (CUSTOM_ERROR_MESSAGE)
			std::cerr << "Error: bad date format => " << line << std::endl;
		else
			std::cerr << "Error: bad input => " << line << std::endl;
		return false;
	}
{
	char *end;
	std::string subline = line.substr(13);
	priceValue = strtof(subline.c_str(), &end);
	if (end == subline.c_str()) {
		std::cerr << "Error : conversion invalid\n";
		return false;
	}
}
	if (priceValue < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	if (priceValue > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}

/**
 * @brief Make the calculation between a date and a BitcoinExchance class
 * 
 * @param[in] btc element to compare
 * @param[in] date date
 * @param[in] price price found
 */
void BitcoinExchange::makeCalculation(const BitcoinExchange &btc, const std::string &date, float price)
{
	BitcoinExchange::dataType::const_iterator it = btc.findNearest(date);
	if (it != btc.end())
	{
		std::cout << date << " => " << price << " = " << price * it->second << std::endl;
	}
	else
	{
		std::cerr << "Error: date not found => " << date << std::endl;
	}
}

void BitcoinExchange::compareFiles(std::ifstream &file)
{
	if (data.empty())
		throw DatabaseNotFoundException();
	if (!file.is_open())
		throw IFileNotOpenException();

	std::string date;
	std::string price;
	float priceValue;

	for (std::string line; getline(file, line);)
	{
		if (!isLineOK(line, priceValue))
			continue;
		date = line.substr(0, 10);
		makeCalculation(*this, date, priceValue);
	}
}

void BitcoinExchange::enableReplaceDuplicatesWithLast()
{
	this->replaceDuplicatesWithLast = 1;
}

void BitcoinExchange::disableReplaceDuplicatesWithLast()
{
	this->replaceDuplicatesWithLast = 0;
}

BitcoinExchange::dataType::const_iterator BitcoinExchange::begin() const
{
	return this->data.begin();
}

BitcoinExchange::dataType::const_iterator BitcoinExchange::end() const
{
	return this->data.end();
}


const char * BitcoinExchange::DatabaseNotFoundException::what() const throw()
{
	return "Database file not found";
}

const char * BitcoinExchange::WrongHeaderException::what() const throw()
{
	return "Header is wrong";
}

const char * BitcoinExchange::CSVFileWronglyFormattedException::what() const throw()
{
	return "CSV file is wrongly formatted";
}

const char * BitcoinExchange::IFileNotOpenException::what() const throw()
{
	return "Input file not open or incorrect permissions";
}
