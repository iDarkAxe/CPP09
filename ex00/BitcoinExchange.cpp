#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

const size_t minimalSizeOfDataPrice = 12; // 10 for date, 1 for sep, 1 for price

BitcoinExchange::BitcoinExchange()
{
	this->replaceDuplicatesWithLast = 0;
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

BitcoinExchange::insertResultType BitcoinExchange::addPair(const std::string& date, float prix)
{
	return this->data.insert(std::make_pair(date, prix));
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
void BitcoinExchange::loadDataFromFile(std::string filename)
{
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
		if (line.size() < minimalSizeOfDataPrice || line[minimalSizeOfDataPrice - 2] != ',')
		{
			std::cout << "Error: bad input => " << line << std::endl;
			throw CSVFileWronglyFormattedException();
		}
		presumedDate = line.substr(0, 10);
		char *end;
		std::string subline = line.substr(11);
		presumedPrice = strtof(subline.c_str(), &end);
		if (end == subline.c_str()) {
			// std::cout << "Error : conversion invalid\n";
			continue;
		}
		if (this->replaceDuplicatesWithLast)
		{
			insertResultType ret = this->addPair(presumedDate, presumedPrice);
			if (!ret.second)
				ret.first->second = presumedPrice;
		}
		else
		{
			this->addPair(presumedDate, presumedPrice);
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
