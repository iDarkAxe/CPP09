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
	{
		this->vect = other.vect;
		this->lst = other.lst;
		return *this;
	}
	return *this;
}

std::vector<unsigned int>& PmergeMe::getVector(void)
{
	return this->vect;
}

std::list<unsigned int>& PmergeMe::getList(void)
{
	return this->lst;
}

void PmergeMe::clear(void)
{
	this->vect.clear();
	this->lst.clear();
}

void PmergeMe::printAllVect(void) const
{
	size_t index = 0;
	for (std::vector<unsigned int>::const_iterator it = this->vect.begin(); it != vect.end(); it++)
	{
		std::cout << "it[" << index << "] = " << *it << std::endl;
		index++;
	}
}

void PmergeMe::printAllList(void) const
{
	size_t index = 0;
	for (std::list<unsigned int>::const_iterator it = this->lst.begin(); it != lst.end(); it++)
	{
		std::cout << "it[" << index << "] = " << *it << std::endl;
		index++;
	}
}

/**
 * @brief Store in the two container at once
 * 
 * @param array array of strings
 */
void PmergeMe::storeInVect(const char *array[])
{
	if (!array)
		throw ArgumentEmptyException();
	std::set<unsigned int> duplicate_test;
	std::string item;
	char *pointer = NULL;
	unsigned int value;
	
	size_t i;
	for (i = 0; array[i]; i++)
	{
		item = array[i];
		if (item.empty())
			throw ArgumentEmptyException();
		if (item.find_first_not_of("0123456789+") != std::string::npos)
			throw ArgumentInvalidException();
		value = strtoul(array[i], &pointer, 10);
		if (&pointer == &array[i] || pointer == NULL || (*pointer != 0 && *pointer != ' ' ))
		{
			if (DEBUG_LEVEL >= DEBUG)
			{
				std::cout << "Input numbers should only be positive integers" << std::endl;
				std::cout << "Faulty argument is '" << array[i] << "'" << std::endl;
			}
			throw ArgumentInvalidException();
		}
		if (!duplicate_test.insert(static_cast<unsigned int>(value)).second)
		{
			if (DEBUG_LEVEL >= INFO)
			{
				std::cout << "Faulty number is '" << value << "'" << std::endl;
			}
			if (THROW_ERROR_IF_DUPLICATE)
				throw DuplicateException();
		}
		vect.push_back(static_cast<unsigned int>(value));
	}
	numberOfElements = i;
	std::cout << "Before : ";
	printAll(vect);
	std::cout << "After :  ";
	printAll(duplicate_test);
}

std::vector<size_t> PmergeMe::generateJacobsthalSequence(size_t n) 
{
	std::vector<size_t> jacobsthal;
	if (n == 0)
		return jacobsthal;
	jacobsthal.push_back(1);
	jacobsthal.push_back(1);
	if (n == 1)
		return jacobsthal;
	jacobsthal.push_back(3);
	size_t prev1 = 1;
	size_t prev2 = 3;
	size_t next;
	while (true)
	{
		next = prev2 + 2 * prev1;
		if (next > n)
			break;
		jacobsthal.push_back(next);
		prev1 = prev2;
		prev2 = next;
	}
	return jacobsthal;
}

void PmergeMe::storeInListFromVect(void)
{
	lst.assign(vect.begin(), vect.end());
}

void PmergeMe::sort_FJMI_vect(void)
{
	size_t comparison_count = 0;

	//= Phase 1 =//
	// Group the elements into pairs and leave the last if odd number of elements.
	unsigned int oddElement = 0;
	bool hasOddElement = false;

	if (this->numberOfElements % 2 == 1)
	{
		hasOddElement = true;
		oddElement = this->vect.back();
		this->vect.pop_back();
	}
	std::vector<std::pair<unsigned int, unsigned int> > pairs;
	size_t pairCount = vect.size() / 2;
	for (size_t i = 0; i < pairCount; ++i)
	{
		unsigned int a = vect[2 * i];
		unsigned int b = vect[2 * i + 1];
		if (a > b) 
			std::swap(a, b);
		comparison_count++; // Counting comparison for performance analysis
		pairs.push_back(std::make_pair(a, b)); // a <= b
	}
	//= Phase 2 =//
	// Recursively sort the larger elements of each pair in ascending order
	// Phase 3 : Insertion =//
	// Insert the smaller elements into the sorted sequence using the Jacobsthal sequence
	// Phase 4 : Final Insertion =//
	// Insert the smaller elements into the sorted sequence using binary search
	this->comparison_count.push_back(comparison_count);
}

void PmergeMe::sort_FJMI_lst(void)
{
	size_t comparison_count = 0;

	this->comparison_count.push_back(comparison_count);
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
