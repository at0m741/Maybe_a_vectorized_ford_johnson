
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <deque>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <list of numbers>" << std::endl;
        return 1;
    }

    std::vector<int> unsorted_list;
    for (int i = 1; i < argc; ++i) {
        unsorted_list.push_back(std::atoi(argv[i]));
    }
	
	std::deque<int> unsorted_list2;
	for (int i = 1; i < argc; ++i) {
		unsorted_list2.push_back(std::atoi(argv[i]));
	}

    std::cout << "Sorting the list...\n";

    std::clock_t start = std::clock();
    std::sort(unsorted_list.begin(), unsorted_list.end());
    std::clock_t end = std::clock();

    double time1 = (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC; 



    std::cout << "Sorting (vector) std::sort completed in " << time1 << " us.\n";

	std::cout << "Sorting the list...\n";

	std::clock_t start2 = std::clock();
	std::sort(unsorted_list2.begin(), unsorted_list2.end());
	std::clock_t end2 = std::clock();
	
	double time2 = (double)(end2 - start2) * 1000000.0 / CLOCKS_PER_SEC;

	std::cout << "Sorting (deque) std::sort completed in " << time2 << " us.\n";
    return 0;
}
