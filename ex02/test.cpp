
#include <vector>
#include <iostream>

int main(void)
{
    std::vector<int> vect;
    vect.reserve(300000);
    for (int i = 0; i < 300000; ++i) 
	{
        vect.push_back(i); // Aucune allocation supplémentaire en théorie
    }
	for (int i = 0; i < 300000; ++i) 
	{
        std::cout << vect[i] << "\n"; // Aucune allocation supplémentaire en théorie
    }
	std::cout << std ::endl;
}
   