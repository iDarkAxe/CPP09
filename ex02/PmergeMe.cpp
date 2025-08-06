#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <set>
#include <cstdlib>

PmergeMe::PmergeMe()
{
	// std::cout << "Default Constructor of PmergeMe" << std::endl;
}

PmergeMe::PmergeMe(size_t numberOfElement)
{
	// std::cout << "Constructor with size of PmergeMe" << std::endl;
	this->vect.reserve(numberOfElement);
}

PmergeMe::~PmergeMe()
{
	;
}

PmergeMe::PmergeMe(const PmergeMe &f)
{
	*this = f;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
		return *this;
	return *this;
}

void PmergeMe::printAllVect(void)
{
	for(std::vector<unsigned int>::iterator it = this->vect.begin(); it != vect.end(); it++)
	{
		std::cout << "it " << *it << std::endl;
	}
}

void PmergeMe::printAllList(void)
{
	for(std::list<unsigned int>::iterator it = this->lst.begin(); it != lst.end(); it++)
	{
		std::cout << "it " << *it << std::endl;
	}
}

/**
 * @brief Store in the two container at once
 * 
 * @param input input
 * @param separator list of characters to separate the inputs
 */
void PmergeMe::storeInVect(const char *array[])
{
	if (!array)
		throw ArgumentEmptyException();
	std::set<unsigned int> duplicate_test;
	std::string item;
	char *pointer = NULL;
	unsigned int value;

	for (size_t i = 0; array[i]; i++)
	{
		item = array[i];
		if (item.find_first_not_of("0123456789+") != std::string::npos)
			throw ArgumentInvalidException();
		value = strtoul(array[i], &pointer, 10);
		if (&pointer == &array[i] || pointer == NULL || (*pointer != 0 && *pointer != ' ' ) || value < 0)
		{
			if (DEBUG_LEVEL >= DEBUG)
			{
				std::cout << "Input numbers should only be positive integers" << std::endl;
				std::cout << "Faulty argument is '" << array[i] << "'" << std::endl;
			}
			throw ArgumentInvalidException();
		}
		if (!duplicate_test.insert(value).second)
		{
			if (DEBUG_LEVEL >= INFO)
			{
				std::cout << "Faulty number is '" << value << "'" << std::endl;
			}
			if (THROW_ERROR_IF_DUPLICATE)
				throw DuplicateException();
		}
		vect.push_back(value);
	}
	std::cout << "Vector capacity now: " << vect.capacity() << std::endl;
	std::cout << "Vector size now: " << vect.size() << std::endl;
}

void PmergeMe::storeInListFromVect(void)
{
	lst.assign(vect.begin(), vect.end());
}

void PmergeMe::sort_FJMI(void)
{
	std::cout << "Trying to sort" << std::endl;
}

const char* PmergeMe::ArgumentEmptyException::what() const throw()
{
	return "At least one argument is empty";
}

const char* PmergeMe::ArgumentInvalidException::what() const throw()
{
	return "This argument is invalid";
}

const char* PmergeMe::DuplicateException::what() const throw()
{
	return "There is a duplicate";
}
