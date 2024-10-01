#include "PmergeMe.hpp"

void compare_pairs_avx(std::vector<std::pair<int, int> >& pairs) 
{
	size_t n = pairs.size();
    check_alignment(&pairs[0], 32);
    size_t block_size = 64;  // Tile size: number of bytes that fit into a cache line

    for (size_t block_start = 0; block_start < n; block_start += block_size) 
    {
        size_t block_end = std::min(block_start + block_size, n);
        size_t i = block_start;

        for (; i + 8 <= block_end; i += 8) 
        {
            if (i + 8 < block_end)
                _mm_prefetch((const char*)&pairs[i + 8], _MM_HINT_T0);

            __m256i first = _mm256_set_epi32(
                pairs[i + 7].first, pairs[i + 6].first, pairs[i + 5].first, pairs[i + 4].first,
                pairs[i + 3].first, pairs[i + 2].first, pairs[i + 1].first, pairs[i + 0].first);
            __m256i second = _mm256_set_epi32(
                pairs[i + 7].second, pairs[i + 6].second, pairs[i + 5].second, pairs[i + 4].second,
                pairs[i + 3].second, pairs[i + 2].second, pairs[i + 1].second, pairs[i + 0].second);
            __m256i min_values = _mm256_min_epi32(first, second);
            __m256i max_values = _mm256_max_epi32(first, second);
            int results_min[8], results_max[8];
            _mm256_storeu_si256((__m256i*)results_min, min_values);
            _mm256_storeu_si256((__m256i*)results_max, max_values);

            for (int j = 0; j < 8; ++j) 
            {
                if (i + j + 8 < block_end)
                    _mm_prefetch((const char*)&pairs[i + j + 8], _MM_HINT_T0);
                pairs[i + j].first = results_min[j];
                pairs[i + j].second = results_max[j];
            }
        }

        // Handle the remaining elements in the block
        for (; i < block_end; ++i) 
        {
            int min_value = std::min(pairs[i].first, pairs[i].second);
            int max_value = std::max(pairs[i].first, pairs[i].second);
            pairs[i].first = min_value;
            pairs[i].second = max_value;
        }
    }
}



void insertion(std::vector<int>& arr, int value) 
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


