#include "PmergeMe.hpp"

void check_if_sorted(const std::vector<int>& arr) {
	for (size_t i = 1; i < arr.size(); ++i) {
		if (arr[i] < arr[i - 1]) {
			std::cerr << "Array is not sorted" << std::endl;
			return;
		}
	}
	std::cerr << "Array is sorted" << std::endl;
}

void check_if_sorted_deque(const std::deque<int>& arr) {
	for (size_t i = 1; i < arr.size(); ++i) {
		if (arr[i] < arr[i - 1]) {
			std::cerr << "Array is not sorted" << std::endl;
			return;
		}
	}
	std::cerr << "Array is sorted" << std::endl;
}
