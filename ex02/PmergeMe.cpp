#include "PmergeMe.hpp"

namespace Color {
	const bool use_color = true;
	// Text Reset;
	const char *Color_Off="\033[0m";
	// Regular Colors
	const char *Red="\033[0;31m";
}

size_t PmergeMe::max_short_args = 4;
bool PmergeMe::show_short_args = true;

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &other) {(void)other;}
PmergeMe &PmergeMe::operator=(const PmergeMe &other) {(void)other;return *this;}

/**
 * @brief Generate the Jacobsthal sequence up to a given number
 * see : https://en.wikipedia.org/wiki/Jacobsthal_number
 *
 * @param[in,out] n The upper limit for the sequence
 * @return T The generated Jacobsthal sequence
 */
PmergeMe::typeVect PmergeMe::generateJacobsthalSequence(size_t n)
{
	typeVect jacobsthal;

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
		jacobsthal.push_back(static_cast<typeElement>(next));
		prev1 = prev2;
		prev2 = next;
	}
	return jacobsthal;
}

PmergeMe::typeVect PmergeMe::buildJacobsthalOrder(size_t size)
{
    typeVect order;
    if (size == 0)
		return order;

    typeVect jacob = generateJacobsthalSequence(size);
    size_t last_pos = 1;
    
    for (size_t i = 2; i < jacob.size(); ++i)
    {
        size_t current_jacob = jacob[i];
        size_t val = current_jacob;
        while (val > last_pos)
        {
            if (val <= size)
                order.push_back(static_cast<typeElement>(val));
            val--;
        }
        last_pos = current_jacob;
    }
    size_t val = size;
    while (val > last_pos)
    {
        order.push_back(static_cast<typeElement>(val));
        val--;
    }
    return order;
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

const char *PmergeMe::ContainerChangedSizeException::what() const throw()
{
	return "The container size has changed";
}

const char *PmergeMe::ContainerNotSortedException::what() const throw()
{
	return "The container is not sorted";
}
