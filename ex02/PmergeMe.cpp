#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>

namespace Color {
	const bool use_color = 1;
	// Text Reset;
	const char *Color_Off="\033[0m";
	// Regular Colors
	const char *Red="\033[0;31m";
}

size_t PmergeMe::max_short_args = 4;

PmergeMe::PmergeMe() : levelOfRecursion(0), numberOfElements(0)
{
	// std::cout << "Default Constructor of PmergeMe" << std::endl;
	this->show_short_args = true;
}

PmergeMe::PmergeMe(size_t startingSize) : levelOfRecursion(0), numberOfElements(0)
{
	// std::cout << "Constructor with size of PmergeMe" << std::endl;
	this->vect.reserve(startingSize);
	this->show_short_args = true;
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &f)
{
	*this = f;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this == &other)
		return *this;
	this->vect = other.vect;
	this->lst = other.lst;
	this->levelOfRecursion = other.levelOfRecursion;
	this->numberOfElements = other.numberOfElements;
	this->comparison_count_vect = other.comparison_count_vect;
	return *this;
}

void PmergeMe::clear(void)
{
	this->vect.clear();
	this->lst.clear();
	this->comparison_count_vect.clear();
	this->numberOfElements = 0;
}

/**
 * @brief Print all elements in the vector
 */
void PmergeMe::printAllVect(void) const
{
	printContainer(this->vect);
}

/**
 * @brief Print some elements in the vector
 */
void PmergeMe::printShortVect(void) const
{
	printShort(this->vect);
}

/**
 * @brief Print all elements in the list
 */
void PmergeMe::printAllList(void) const
{
	printContainer(this->lst);
}

/**
 * @brief Store in the vector container
 *
 * @param[in] array array of strings
 */
void PmergeMe::storeInVect(const char *array[])
{
	storeInLoop(this->vect, array);
}

/**
 * @brief Store in the list container
 *
 * @param[in] array array of strings
 */
void PmergeMe::storeInList(const char *array[])
{
	storeInLoop(this->lst, array);
}

void PmergeMe::storeValues(const char *array[])
{
	storeInVect(array);
	storeInListFromVect();
}

void PmergeMe::storeInListFromVect(void)
{
	lst.assign(vect.begin(), vect.end());
}

void PmergeMe::storeInVectFromList(void)
{
	vect.assign(lst.begin(), lst.end());
}

void PmergeMe::sort_FJMI_vect(void)
{
	size_t comparison_count = 0;

	const size_t start_size = this->vect.size();
	if (start_size <= 1)
		return;

	// Phase 1: recursive pairing
	size_t maxGroupSize = splitIntoPairsRecursive<typeVect>(this->vect, 2, comparison_count);
	(void)maxGroupSize;

	if (start_size != this->vect.size())
	{
		// if (DEBUG_LEVEL >= INFO)
		std::cerr << "Error: Vector size changed during sorting! Before: " << start_size << ", After: " << this->vect.size() << std::endl;
	}
	if (!verifyOrder(this->vect))
	{
		// if (DEBUG_LEVEL >= INFO)
		std::cerr << "Vector is not sorted correctly!" << std::endl;
	}
	this->comparison_count_vect.push_back(comparison_count);
}

const char *PmergeMe::ArgumentEmptyException::what() const throw()
{
	return "At least one argument is empty";
}

const char *PmergeMe::ArgumentInvalidException::what() const throw()
{
	return "This argument is invalid";
}

const char *PmergeMe::DuplicateException::what() const throw()
{
	return "There is a duplicate";
}
