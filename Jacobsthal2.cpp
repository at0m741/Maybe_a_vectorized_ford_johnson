#include "PmergeMe.hpp"

align static const uint64_t jacobsthal_table[] = 
{
    0, 1 , 1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 
    5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 
    1398101, 2796203, 5592405, 11184811, 22369621, 44739243, 
    89478485, 178956971, 357913941, 715827883, 1431655765, 
    2863311531, 5726623061, 11453246123, 22906492245, 45812984491, 
    91625968981, 183251937963, 366503875925, 733007751851, 1466015503701, 
    2932031007403, 5864062014805, 11728124029611, 23456248059221, 46912496118443, 
    93824992236885, 187649984473771, 375299968947541, 750599937895083, 1501199875790165, 
    3002399751580331, 6004799503160661, 12009599006321323, 24019198012642645, 48038396025285291, 
    96076792050570581, 192153584101141163, 384307168202282325, 768614336404564651,
    1537228672809129301, 3074457345618258603, 6148914691236517205
};

void calculate_jacobsthal_deque(std::deque<uint64_t>& jacobsthal, size_t start, size_t size) 
{
    if (start < 2) return;

    uint64_t prev2 = jacobsthal[start - 2];
    uint64_t prev1 = jacobsthal[start - 1];
    size_t i = start;

    for (; i < size; ++i) 
    {
        jacobsthal[i] = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = jacobsthal[i];
    }
}

std::deque<uint64_t> generate_jacobsthal_deque(size_t size) 
{
    size_t static_size = sizeof(jacobsthal_table) / sizeof(jacobsthal_table[0]);
    std::deque<uint64_t> jacobsthal(size);

    size_t limit = std::min(size, static_size);
    for (size_t i = 0; i < limit; ++i)
        jacobsthal[i] = jacobsthal_table[i];

    if (size > static_size)
        calculate_jacobsthal_deque(jacobsthal, static_size, size);

    return jacobsthal;
}
