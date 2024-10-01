#include "PmergeMe.hpp"

int partition_pairs(std::deque<std::pair<int, int> >& pairs, int left, int right);

//avx_swap


void quicksort_pairs(std::deque<std::pair<int, int> >& pairs, int left, int right) 
{
    if (left < right) 
    {
        int pivotIndex = partition_pairs(pairs, left, right);
        quicksort_pairs(pairs, left, pivotIndex - 1);
        quicksort_pairs(pairs, pivotIndex + 1, right);
    }
}

int partition_pairs(std::deque<std::pair<int, int> >& pairs, int left, int right) 
{
    std::pair<int, int> pivot = pairs[right];
    int i = left - 1;

    for (int j = left; j < right; ++j) 
    {
        if (ComparePairs()(pairs[j], pivot)) 
        {
            ++i;
            std::swap(pairs[i], pairs[j]);
        }
    }
    std::swap(pairs[i + 1], pairs[right]);
    return i + 1;
}

std::deque<int>& ford_johnson_sort_deque(std::deque<int>& arr) 
{
    int n = arr.size();
    int straggler = -1;
    bool has_straggler = false;
    if ((n & 1) != 0) 
	{
        has_straggler = true;
        straggler = arr.back();
        arr.pop_back();
        n--;
    }
    std::deque<std::pair<int, int> > pairs;
    for (int i = 0; i < n; i += 2)
        pairs.push_back(std::make_pair(arr[i], arr[i + 1]));

    compare_pairs_avx_deque(pairs);
	quicksort_pairs(pairs, 0, pairs.size() - 1);
    std::deque<int> S;
    std::deque<int> pend;


    for (size_t i = 0; i < pairs.size(); ++i) 
	{
        S.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
    }

    std::deque<uint64_t> jacobsthal_sequence = generate_jacobsthal_deque(pend.size());
    std::set<int> inserted_values;

    for (size_t i = 0; i < jacobsthal_sequence.size(); ++i)
	{
        size_t idx = jacobsthal_sequence[i];
        if (idx < pend.size() && inserted_values.find(pend[idx]) == inserted_values.end()) 
		{
            insertion_deque(S, pend[idx]);
            inserted_values.insert(pend[idx]);
        }
    }

    if (has_straggler)
        if (inserted_values.find(straggler) == inserted_values.end())
            insertion_deque(S, straggler);

    for (size_t i = 0; i < pend.size(); ++i) 
	{
        if (inserted_values.find(pend[i]) == inserted_values.end()) 
		{
            insertion_deque(S, pend[i]);
            inserted_values.insert(pend[i]);
        }
    }

    arr.swap(S);
    return arr;
}

