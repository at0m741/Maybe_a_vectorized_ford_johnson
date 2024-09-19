#include "PmergeMe.hpp"

int main(int argc, char* argv[]) 
{
    std::vector<int> data;
    std::deque<int> data2;

	if (argc <= 2) 
	{
		std::cout << "Usage: " << argv[0] << " <number1> <number2> ... <numberN>" << std::endl;
		std::cout << " " << std::endl;
		std::cout << "Example: " << argv[0] << "`shuf -i 1-N -n N | tr " "`" << std::endl; 
		return 0;
	}

    for (int i = 1; i < argc; ++i) 
	{
        std::string argument(argv[i]);
		if (argument.empty()) 
		{
			std::cout << "Invalid argument: " << argument << std::endl;
			return 0;
		}

        if (!argument.empty())
            data.push_back(std::atoi(argument.c_str()));
    }

    if (data.empty()) 
	{
        std::cout << "Nothing to sort" << std::endl;
        return 0;
    }

    data2.insert(data2.begin(), data.begin(), data.end());

    std::cout << "Before :";
    for (size_t i = 0; i < data.size(); ++i)
        std::cout << " " << data[i];
    std::cout << "\n"; 

	std::clock_t start = std::clock();
    ford_johnson_sort(data);
    std::clock_t end = std::clock();
    double time1 = (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC;  
	start = std::clock();
    ford_johnson_sort_deque(data2);
    end = std::clock();
    double time2 = (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC; 

    std::cout << "After  :";
    for (size_t i = 0; i < data.size(); ++i)
        std::cout << " " << data[i];

    std::cout << "\n";
	check_if_sorted(data);
	check_if_sorted_deque(data2);
    std::cout << "Time for first sort (vector): " << time1 << " us" << std::endl;
    std::cout << "Time for second sort (deque): " << time2 << " us" << std::endl;

    return 0;
}
