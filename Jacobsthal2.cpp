#include "PmergeMe.hpp"

void calculate_jacobsthal_deque(std::deque<uint64_t>& jacobsthal, size_t start, size_t size) {
    if (start < 2) return;

    uint64_t prev2 = jacobsthal[start - 2];
    uint64_t prev1 = jacobsthal[start - 1];
    size_t i = start;

    for (; i < size; ++i) {
        jacobsthal[i] = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = jacobsthal[i];
    }
}

std::deque<uint64_t> generate_jacobsthal_deque(size_t size) {
    size_t static_size = sizeof(jacobsthal_table) / sizeof(jacobsthal_table[0]);
    std::deque<uint64_t> jacobsthal(size);

    size_t limit = std::min(size, static_size);
    for (size_t i = 0; i < limit; ++i)
        jacobsthal[i] = jacobsthal_table[i];

    if (size > static_size)
        calculate_jacobsthal_deque(jacobsthal, static_size, size);

    return jacobsthal;
}
