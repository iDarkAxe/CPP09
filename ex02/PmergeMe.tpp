#ifndef PMERGEME_TPP
#define PMERGEME_TPP

#include "PmergeMe.hpp"

template <typename T>
void PmergeMe::printAll(const T &container, const char *separator)
{
	for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
	{
		std::cout << *it << separator;
	}
	std::cout << std::endl;
}

template <typename T>
void PmergeMe::printShort(const T &container, const char *separator)
{
	size_t numberOfElementsToPrint = max_short_args;
	for (typename T::const_iterator it = container.begin(); it != container.end() && numberOfElementsToPrint > 0; ++it)
	{
		std::cout << *it << separator;
		--numberOfElementsToPrint;
	}
	if (container.size() > max_short_args)
		std::cout << "[...]";
	std::cout << std::endl;
}

/**
 * @brief Generate the Jacobsthal sequence up to a given number
 * see : https://en.wikipedia.org/wiki/Jacobsthal_number
 *
 * @param[in,out] n The upper limit for the sequence
 * @return T The generated Jacobsthal sequence
 */
template <typename Container>
Container PmergeMe::generateJacobsthalSequence(size_t n)
{
	Container jacobsthal;
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

/**
 * @brief Insert a value into a sorted container using binary search
 *
 * @param[in,out] temp_container The container to insert into
 * @param[in] value The value to insert
 * @param[in] comparison_count number of comparison
 */
template <typename Container>
void PmergeMe::binaryInsertContainer(Container &temp_container, unsigned int value, size_t limit, size_t &comparison_count)
{
	typename Container::iterator left = temp_container.begin();
	typename Container::iterator right = temp_container.begin();
	std::advance(right, std::min(limit, temp_container.size()));

	while (left != right)
	{
		typename Container::iterator mid = left;
		typename Container::difference_type dist = std::distance(left, right);
		std::advance(mid, dist / 2);

		comparison_count++;
		if (*mid < value)
			left = ++mid;
		else
			right = mid;
	}

	temp_container.insert(left, value);
}

#endif // PMERGEME_TPP
