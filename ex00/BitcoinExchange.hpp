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
	bool replaceDuplicatesWithLast;

private:
	dataType data;

private:
	BitcoinExchange(const BitcoinExchange &f);
	BitcoinExchange &operator=(const BitcoinExchange &other);

	void printPair(const std::string &date);
	void printAll() const;
	void makeCalculation(const BitcoinExchange &btc, const std::string &date, float price);
	bool isLineOK(const std::string &line, float &priceValue);
	bool isdigit_inrange(const std::string& str, size_t start, size_t end) const;
	bool isDateValid(int year, int month, int day) const;

	dataType::const_iterator findNearest(const std::string &date) const;
	dataType::const_iterator begin() const;
	dataType::const_iterator end() const;

public:
	BitcoinExchange();
	~BitcoinExchange();
	
	void loadDataFromFile(const std::string& filename);
	void compareFiles(std::ifstream &file);
	void enableReplaceDuplicatesWithLast();
	void disableReplaceDuplicatesWithLast();

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
	class IFileNotOpenException : public std::exception
	{
		public:
			const char *what() const throw();
	};
};

#endif
