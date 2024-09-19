#include "PmergeMe.hpp"


std::vector<int>& ford_johnson_sort(std::vector<int>& arr) 
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
    std::vector<std::pair<int, int> > pairs;
    for (int i = 0; i < n; i += 2)
        pairs.push_back(std::make_pair(arr[i], arr[i + 1]));

    compare_pairs_avx(pairs);
    std::sort(pairs.begin(), pairs.end(), ComparePairs());
    std::vector<int> S;
    std::vector<int> pend;
    S.reserve(pairs.size());
    pend.reserve(pairs.size());

    for (size_t i = 0; i < pairs.size(); ++i) 
	{
        S.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
    }

    std::vector<uint64_t> jacobsthal_sequence = generate_jacobsthal_AVX(pend.size());
    std::set<int> inserted_values;

    for (size_t i = 0; i < jacobsthal_sequence.size(); ++i)
	{
        size_t idx = jacobsthal_sequence[i];
        if (idx < pend.size() && inserted_values.find(pend[idx]) == inserted_values.end()) 
		{
            insertion(S, pend[idx]);
            inserted_values.insert(pend[idx]);
        }
    }

    if (has_straggler)
        if (inserted_values.find(straggler) == inserted_values.end())
            insertion(S, straggler);

    for (size_t i = 0; i < pend.size(); ++i) 
	{
        if (inserted_values.find(pend[i]) == inserted_values.end()) 
		{
            insertion(S, pend[i]);
            inserted_values.insert(pend[i]);
        }
    }

    arr.swap(S);
    return arr;
}

