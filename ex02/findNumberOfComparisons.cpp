#include <cmath>
#include <iostream>

// https://dev.to/emuminov/human-explanation-and-step-by-step-visualisation-of-the-ford-johnson-algorithm-5g91
/**
 * @brief Formula to calculate the maximum number of comparisons needed to sort n elements with the Ford-Johnson algorithm.
 * 
 * @param[in] n number of elements to sort
 * @return int maximum number of comparisons needed to sort n elements with the Ford-Johnson algorithm
 */
int F(int n)
{
    int sum = 0;
    for (int k = 1; k <= n; ++k) {
        double value = (3.0 / 4.0) * k;
        sum += static_cast<int>(ceil(log2(value)));
    }
    return sum;
}

int main(int argc, char *argv[])
{
	int n = 21;
	if (argc != 1)
		n = atoi(argv[1]);
	int result = F(n);
	std::cout << "F(" << n << ") = " << result << std::endl;
	std::cout << "Le nombre de comparaisons max nécessaires pour trier " << n << " éléments avec l'algorithme de Ford-Johnson est : " << result << std::endl;
	return 0;
}
