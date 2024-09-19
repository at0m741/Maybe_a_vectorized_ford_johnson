#include "PmergeMe.hpp"

void compare_pairs_avx_deque(std::deque<std::pair<int, int> >& pairs) 
{
    size_t n = pairs.size();

    std::vector<std::pair<int, int> > temp(pairs.begin(), pairs.end());

    size_t i = 0;
    
    for (; i + 8 <= n; i += 8) 
    {
        if (i + 8 < n) 
            _mm_prefetch((const char*)&temp[i + 8], _MM_HINT_T0);

        __m256i first = _mm256_set_epi32(
            temp[i + 7].first, temp[i + 6].first, temp[i + 5].first, temp[i + 4].first,
            temp[i + 3].first, temp[i + 2].first, temp[i + 1].first, temp[i + 0].first);
        __m256i second = _mm256_set_epi32(
            temp[i + 7].second, temp[i + 6].second, temp[i + 5].second, temp[i + 4].second,
            temp[i + 3].second, temp[i + 2].second, temp[i + 1].second, temp[i + 0].second);
        __m256i min_values = _mm256_min_epi32(first, second);
        __m256i max_values = _mm256_max_epi32(first, second);
        int results_min[8], results_max[8];
        _mm256_storeu_si256((__m256i*)results_min, min_values);
        _mm256_storeu_si256((__m256i*)results_max, max_values);

        for (int j = 0; j < 8; ++j) 
        {
            if (i + j + 8 < n)
                _mm_prefetch((const char*)&temp[i + j + 8], _MM_HINT_T0);
            temp[i + j].first = results_min[j];
            temp[i + j].second = results_max[j];
        }
    }

    for (; i < n; ++i) 
    {
        int min_value = std::min(temp[i].first, temp[i].second);
        int max_value = std::max(temp[i].first, temp[i].second);
        temp[i].first = min_value;
        temp[i].second = max_value;
    }

    for (size_t j = 0; j < n; ++j) 
        pairs[j] = temp[j];
}


void insertion_deque(std::deque<int>& arr, int value) 
{
    int left = 0;
    int right = arr.size() - 1;
    int pos = arr.size();

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] < value) 
            left = mid + 1;
        else 
        {
            pos = mid;
            right = mid - 1;
        }
    }

    arr.insert(arr.begin() + pos, value); 
}
