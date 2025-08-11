#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <list>
#include <string>
#include <stdint.h>

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
private:
	std::vector<unsigned int> vect;
	std::list<unsigned int> lst;

public:
	PmergeMe();
	PmergeMe(size_t numberOfElement);
	~PmergeMe();
	PmergeMe(const PmergeMe &f);
	PmergeMe &operator=(const PmergeMe &other);

	size_t	numberOfElements;

	template <typename T>
	T getAndPop(void);

	void printAllVect(void);
	void printAllList(void);
	void storeInVect(const char *array[]);
	void storeInListFromVect(void);
	void sort_FJMI_lst(void);
	void sort_FJMI_vect(void);

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
