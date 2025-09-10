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
#define DEBUG_LEVEL INFO
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

//= Methods =//
private:
	std::vector<size_t> generateJacobsthalSequence(size_t n);

public:
	PmergeMe();
	PmergeMe(size_t numberOfElement);
	~PmergeMe();
	PmergeMe(const PmergeMe &f);
	PmergeMe &operator=(const PmergeMe &other);

	template <typename T>
	static void printAll(const T& container, const char *separator = " ")
	{
		for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
		{
			std::cout << *it << separator;
		}
		std::cout << std::endl;
	}
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
};

#endif
