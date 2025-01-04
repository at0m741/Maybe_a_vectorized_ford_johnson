#include "PmergeMe.hpp"
#include <cstring>


void compare_pairs_avx(std::vector<std::pair<int, int> >& pairs) {
	size_t n = pairs.size();
    size_t block_size = 256 / sizeof(std::pair<int, int>);

    for (size_t block_start = 0; block_start < n; block_start += block_size) {
        size_t block_end = std::min(block_start + block_size, n);
        size_t i = block_start;

		for (; i + 8 <= block_end; i += 8) {
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
            
			for (int j = 0; j < 8; ++j) {
                pairs[i + j].first = results_min[j];
                pairs[i + j].second = results_max[j];
            }
        }

        for (; i < block_end; ++i) {
            int min_value = std::min(pairs[i].first, pairs[i].second);
            int max_value = std::max(pairs[i].first, pairs[i].second);
            pairs[i].first = min_value;
            pairs[i].second = max_value;
        }
    }
}

__attribute__((target("avx2"), hot))
void insertion(std::vector<int>& arr, int value)
{
	arr.resize(arr.size());
    if (arr.empty()) {
        arr.push_back(value);
        return;
    }
    if (value >= arr.back()) {
        arr.push_back(value);
        return;
    }
    if (value <= arr.front()) {
        arr.push_back(0);  
        __builtin_memmove(&arr[1], &arr[0], (arr.size()-1)*sizeof(int));
        arr[0] = value;
        return;
    }

    size_t bound = 1;
    while (bound < arr.size() && arr[bound] < value) {
        bound <<= 1;  
    }
    if (bound > arr.size() - 1) {
        bound = arr.size() - 1;
    }
    
    size_t left  = bound >> 1; 
    size_t right = bound;
    size_t pos   = arr.size(); 
    while (left <= right) {
        size_t mid = left + ((right - left) >> 1);
        if (arr[mid] >= value) {
            pos   = mid;
            right = (mid == 0 ? 0 : mid - 1);
        } else {
            left  = mid + 1;
        }
    }
    arr.push_back(0); 
    const size_t len = arr.size() - pos - 1;
	_mm_prefetch(&arr[pos], _MM_HINT_T0);
    if (len > 0) {
        __builtin_memmove(&arr[pos + 1], &arr[pos], len * sizeof(int));
    }
    arr[pos] = value;
}
