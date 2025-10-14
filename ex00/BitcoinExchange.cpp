#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

const size_t minimalSizeOfDataPrice = 12; // 10 for date, 1 for sep, 1 for price

BitcoinExchange::BitcoinExchange()
{
	;
}

BitcoinExchange::~BitcoinExchange()
{
	;
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

void BitcoinExchange::addPair(const std::string& date, float prix)
{
	this->data.insert(std::make_pair(date, prix));
}

void BitcoinExchange::printPair(const std::string& date)
{
	std::map<std::string, float>::iterator it;

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
	std::map<std::string, float>::const_iterator it;
	std::map<std::string, float>::const_iterator end = this->data.end();

	for (it = this->data.begin(); it != end; ++it)
	{
		std::cout << it->first << " => " << it->second << " = " << "placeholder" << std::endl;
	}
}

void BitcoinExchange::loadDataFromFile(std::string filename)
{
	std::ifstream database(filename.c_str(), std::ios_base::in);

	if (database.is_open() == false || database.fail())
	{
		std::cout << "Error: could not open file" << filename << std::endl;
		throw DatabaseNotFoundException();
	}
{
	std::string header;
	// Read the header line and check if it is valid
	getline(database, header);
	if (header != "date,exchange_rate")
	{
		std::cout << "Error: bad header => " << header << std::endl;
		throw WrongHeaderException();
	}
}
	std::string line;
	std::string presumedDate;
	double presumedPrice;
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
		presumedPrice = strtod(line.substr(11).c_str(), NULL);
		this->addPair(presumedDate, presumedPrice);
	}
}

std::map<std::string, float>::const_iterator BitcoinExchange::findNearest(const std::string &date) const
{
    std::map<std::string, float>::const_iterator it = this->data.lower_bound(date);
    if (it != this->data.end() && it->first == date) {
        return it;
    }

    if (it == this->data.end()) {
        return --it;
    }

    if (it == this->data.begin()) {
        return it;
    }
    std::map<std::string, float>::const_iterator prev_it = it;
	--prev_it;
    if ((date.compare(prev_it->first)) <= (it->first.compare(date))) {
        return prev_it;
    } else {
        return it;
    }
}

std::map<std::string, float>::const_iterator BitcoinExchange::begin() const
{
	return this->data.begin();
}

std::map<std::string, float>::const_iterator BitcoinExchange::end() const
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
