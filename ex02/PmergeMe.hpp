#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <list>
#include <string>
#include <stdint.h>
#include <iostream>

enum debug_level
{
	NOTHING = 0,
	INFO = 1,
	DEBUG = 2
};

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG
#endif

#ifndef THROW_ERROR_IF_DUPLICATE
#define THROW_ERROR_IF_DUPLICATE 1
#endif

class PmergeMe
{
//= Variables =//
private:
	std::vector<unsigned int> vect;
	std::list<unsigned int> lst;

public:
	std::vector<size_t> comparison_count;
	size_t	numberOfElements;
	bool show_short_args;
	size_t max_short_args;

//= Methods =//
private:
	size_t sort_FJMI_vect_recursive(std::vector<unsigned int> &temp_vec);
	// size_t sort_FJMI_lst_recursive(std::list<unsigned int> &temp_lst);

public:
	// Orthodox Canonical Form : Constructors / Destructors / Operators
	PmergeMe();
	PmergeMe(size_t numberOfElement);
	~PmergeMe();
	PmergeMe(const PmergeMe &f);
	PmergeMe &operator=(const PmergeMe &other);

	std::vector<unsigned int>& getVector(void);
	std::list<unsigned int>& getList(void);
	void clear(void);
	void printAllVect(void) const;
	void printAllList(void) const;
	void storeInVect(const char *array[]);
	void storeInListFromVect(void);
	void sort_FJMI_lst(void);
	void sort_FJMI_vect(void);

//= Exceptions =//
	class ArgumentEmptyException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class ArgumentInvalidException : public std::exception
	{
		public:
			const char *what() const throw();
	};
	class DuplicateException : public std::exception
	{
		public:
			const char *what() const throw();
	};

public:
//= Templates =//
	template <typename T>
	static void printAll(const T& container, const char *separator = " ")
	{
		for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
		{
			std::cout << *it << separator;
		}
		std::cout << std::endl;
	}

	template <typename T>
	void printShort(const T& container, const char *separator = " ")
	{
		size_t numberOfElementsToPrint = this->max_short_args;
		for (typename T::const_iterator it = container.begin(); it != container.end() && numberOfElementsToPrint > 0; ++it)
		{
			std::cout << *it << separator;
			--numberOfElementsToPrint;
		}
		std::cout << "[...]";
		std::cout << std::endl;
	}

private:
	/**
	 * @brief Generate the Jacobsthal sequence up to a given number
	 * see : https://en.wikipedia.org/wiki/Jacobsthal_number
	 * 
	 * @param[in,out] n The upper limit for the sequence
	 * @return T The generated Jacobsthal sequence
	 */
	template <typename Container>
	Container generateJacobsthalSequence(size_t n) 
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
	 * @param[in] end The end index of the sorted portion of the container
	 */
	template <typename Container>
	void binaryInsertContainer(Container& temp_container, unsigned int value, size_t end) 
	{
		size_t left = 0;
		size_t right = end;
		while (left < right) {
			size_t mid = left + (right - left) / 2;
			if (temp_container[mid] < value) {
				left = mid + 1;
			} else {
				right = mid;
			}
		}
		temp_container.insert(temp_container.begin() + left, value);
	}

};

#endif
