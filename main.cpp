#include "PmergeMe.hpp"

int main(int argc, char* argv[]) 
{
    std::vector<int> data;
    std::deque<int> data2;

    if (argc <= 2) 
    {
        std::cout << "Usage: " << argv[0] << " <number1> <number2> ... <numberN>" << std::endl;
        std::cout << " " << std::endl;
        std::cout << "Example: " << argv[0] << " `shuf -i 1-N -n N | tr \"\\n\" \" \"`" << std::endl; 
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
        data.push_back(std::atoi(argument.c_str()));
    }

    if (data.empty()) 
    {
        std::cout << "Nothing to sort" << std::endl;
        return 0;
    }

    data2.insert(data2.begin(), data.begin(), data.end());

    int n = data.size();
    int straggler = -1;
    bool has_straggler = false;

    if (n % 2 != 0) 
    {
        has_straggler = true;
        straggler = data.back();
        data.pop_back();
        n--;
    }

    std::vector<std::pair<int, int> > pairs;
    pairs.reserve(n / 2);
    for (int i = 0; i < n; i += 2)
        pairs.push_back(std::make_pair(data[i], data[i + 1]));

    std::cout << "Before :";
    for (size_t i = 0; i < data2.size(); ++i)
        std::cout << " " << data2[i];
    std::cout << "\n"; 

    std::clock_t start = std::clock();
    std::vector<int> sorted_data = ford_johnson_sort(pairs, straggler, has_straggler);
    std::clock_t end = std::clock();
    double time1 = (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC;  

    start = std::clock();
    ford_johnson_sort_deque(data2);
    end = std::clock();
    double time2 = (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC; 

    std::cout << "After  :";
    for (size_t i = 0; i < sorted_data.size(); ++i)
        std::cout << " " << sorted_data[i];
    std::cout << "\n";

    check_if_sorted(sorted_data);
    check_if_sorted_deque(data2);

    std::cout << "Time for first sort (vector): " << time1 << " us" << std::endl;
    std::cout << "Time for second sort (deque): " << time2 << " us" << std::endl;

    return 0;
}
