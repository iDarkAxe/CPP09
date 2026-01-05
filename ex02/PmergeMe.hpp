#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <list>
#include <set>
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
#define DEBUG_LEVEL NOTHING
#endif

#ifndef THROW_ERROR_IF_DUPLICATE
#define THROW_ERROR_IF_DUPLICATE 1
#endif

class PmergeMe
{
//= Variables =//
private:
	typedef std::vector<unsigned int> typeVect;
	typedef std::list<unsigned int> typeList;
	typeVect vect;
	typeList lst;

public:
	typedef std::set<unsigned int> typeSet;
	std::vector<size_t> comparison_count_vect;
	size_t numberOfElements;
	bool show_short_args;
	static size_t max_short_args;
	typeSet sorted_set;

//= Methods =//
private:
	void sort_FJMI_vect_recursive(typeVect &temp_vec, size_t &comparison_count);
	void sort_FJMI_lst_recursive(typeList &temp_lst, size_t &comparison_count);

public:
	// Orthodox Canonical Form : Constructors / Destructors / Operators
	PmergeMe();
	PmergeMe(size_t numberOfElement);
	~PmergeMe();
	PmergeMe(const PmergeMe &f);
	PmergeMe &operator=(const PmergeMe &other);

	typeVect &getVector(void);
	typeList &getList(void);
	void clear(void);
	void printAllVect(void) const;
	void printAllList(void) const;
	template <typename T>
	void storeInLoop(T &container, const char *array[]);
	void storeInVect(const char *array[]);
	void storeInList(const char *array[]);
	void storeValues(const char *array[]);
	void storeInListFromVect(void);
	void storeInVectFromList(void);
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
	static void printAll(const T &container, const char *separator = " ")
	{
		for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
		{
			std::cout << *it << separator;
		}
		std::cout << std::endl;
	}

	template <typename T>
	static void printShort(const T &container, const char *separator = " ")
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
	 * @param[in] comparison_count number of comparison
	 */
	template <typename Container>
	void binaryInsertContainer(Container &temp_container, unsigned int value, size_t limit, size_t &comparison_count)
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

	/**
	 * @brief Insert a value into a sorted container using binary search
	 *
	 * @param[in,out] temp_lst The list to insert into
	 * @param[in] value The value to insert
	 * @param[in] end The end index of the sorted portion of the lst
	 */
	void binaryInsertContainer(typeList &temp_lst, unsigned int value, size_t end)
	{
		size_t left = 0;
		size_t right = end;
		typeList::iterator it;
		while (left < right)
		{
			size_t mid = left + (right - left) / 2;
			it = temp_lst.begin();
			std::advance(it, mid);
			if (*it < value)
				left = mid + 1;
			else
				right = mid;
		}
		typeList::iterator insert_it = temp_lst.begin();
		std::advance(insert_it, left);
		temp_lst.insert(insert_it, value);
	}
};

#endif
