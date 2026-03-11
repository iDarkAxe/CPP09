#include "PmergeMe.hpp"
#include <iostream>

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
	this->show_short_args = other.show_short_args;
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

void PmergeMe::storeInListFromVect(void)
{
	lst.assign(vect.begin(), vect.end());
}

void PmergeMe::sort_FJMI_vect(void)
{
	sort_FJMI(this->vect);
}

void PmergeMe::sort_FJMI_lst(void)
{
	sort_FJMI(this->lst);
}

size_t PmergeMe::size_vect(void) const
{
	return this->vect.size();
}

size_t PmergeMe::size_lst(void) const
{
	return this->lst.size();
}
