#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>

class BitcoinExchange : public std::map<std::string, float>
{
private:

public:
	BitcoinExchange();
	~BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &f);
	BitcoinExchange &operator=(const BitcoinExchange &other);

	void addPair(const std::string &date, float prix);
	void printPair(const std::string &date);
	void printAll() const;
	void loadDataFromFile(std::string filename);

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
