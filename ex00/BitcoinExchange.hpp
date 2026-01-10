#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>

class BitcoinExchange
{
public:
	typedef std::map<std::string, float> dataType;
	typedef std::pair<BitcoinExchange::dataType::iterator, bool> insertResultType;
private:
	dataType data;

public:
	bool replaceDuplicatesWithLast;

private:
	BitcoinExchange(const BitcoinExchange &f);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	insertResultType addPair(const std::string &date, float prix);
	void printPair(const std::string &date);
	void printAll() const;

public:
	BitcoinExchange();
	~BitcoinExchange();
	
	void loadDataFromFile(std::string filename);
	void enableReplaceDuplicatesWithLast();
	void disableReplaceDuplicatesWithLast();

	dataType::const_iterator findNearest(const std::string &date) const;
	dataType::const_iterator begin() const;
	dataType::const_iterator end() const;

	class DatabaseNotFoundException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class WrongHeaderException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class CSVFileWronglyFormattedException : public std::exception
	{
		public:
			const char *what() const throw();
	};
};

#endif
