#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#include <iostream>
#include <stdint.h>
#include <ctime>
#include <immintrin.h>
/*
 * Jacobsthal sequence is a sequence of integers defined by the recurrence relation:
 * J(0) = 0
 * J(1) = 1
 * J(n) = J(n - 1) + 2 * J(n - 2)
 *
 * This function generates the first `size` elements of the Jacobsthal sequence.
 * The function uses AVX instructions to compute the sequence faster.
 * here is a static table of the first 64 elements of the Jacobsthal sequence.
 * this table is used to compute the first elements of the sequence.
 * if the requested size is greater than 64, the function uses AVX instructions to compute the rest of the sequence.
 */

static const uint64_t jacobsthal_table[] = {
		0, 1 , 1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, \
		5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, \
		1398101, 2796203, 5592405, 11184811, 22369621, 44739243, \
		89478485, 178956971, 357913941, 715827883, 1431655765, \
		2863311531, 5726623061, 11453246123, 22906492245, 45812984491, \
		91625968981, 183251937963, 366503875925, 733007751851, 1466015503701, \
		2932031007403, 5864062014805, 11728124029611, 23456248059221, 46912496118443, \
		93824992236885, 187649984473771, 375299968947541, 750599937895083, 1501199875790165, \
		3002399751580331, 6004799503160661, 12009599006321323, 24019198012642645, 48038396025285291, \
		96076792050570581, 192153584101141163, 384307168202282325, 768614336404564651,\
		1537228672809129301, 3074457345618258603, 6148914691236517205
};

bool is_aligned_32(const void* ptr) {
    return (reinterpret_cast<uintptr_t>(ptr) & 31) == 0;
}

inline void calculate_jacobsthal_avx(std::vector<uint64_t>& jacobsthal, size_t start, size_t size) {
    if (start < 2) return;

    uint64_t* data	= &jacobsthal[start];
    bool aligned	= is_aligned_32(data);
    uint64_t prev2	= jacobsthal[start - 2];
    uint64_t prev1	= jacobsthal[start - 1];

    size_t i = start;

    if (aligned) {
		if (i + 4 < size)
			_mm_prefetch((const char*)&jacobsthal[i + 4], _MM_HINT_T0);
        for (; i + 4 <= size; i += 4) {
            __m256i prev1_vec		= _mm256_set1_epi64x(prev1);
            __m256i prev2_vec		= _mm256_set1_epi64x(prev2);
            __m256i two_prev2_vec	= _mm256_slli_epi64(prev2_vec, 1);
            __m256i jacobsthal_vec	= _mm256_add_epi64(prev1_vec, two_prev2_vec);
            _mm256_store_si256((__m256i*)&jacobsthal[i], jacobsthal_vec);

            prev2 = jacobsthal[i + 2];
            prev1 = jacobsthal[i + 3];

			#ifndef DEBUG
				std::cout << "jacobsthal[" << i << "] = " << jacobsthal[i] << std::endl;
			#endif
        }
    } else {
        for (; i + 4 <= size; i += 4) {
            __m256i prev1_vec		= _mm256_set1_epi64x(prev1);
            __m256i prev2_vec		= _mm256_set1_epi64x(prev2);
            __m256i two_prev2_vec	= _mm256_slli_epi64(prev2_vec, 1);
            __m256i jacobsthal_vec	= _mm256_add_epi64(prev1_vec, two_prev2_vec);
            _mm256_storeu_si256((__m256i*)&jacobsthal[i], jacobsthal_vec); 

            prev2 = jacobsthal[i + 2];
            prev1 = jacobsthal[i + 3];

			#ifndef DEBUG
				std::cout << "jacobsthal[" << i << "] = " << jacobsthal[i] << std::endl;
			#endif
        }
    }

    for (; i < size; ++i) {
        jacobsthal[i] = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = jacobsthal[i];
    }
}

inline void check_if_sorted(const std::vector<int>& arr) {
	for (size_t i = 1; i < arr.size(); ++i) {
		if (arr[i] < arr[i - 1]) {
			std::cerr << "Array is not sorted" << std::endl;
			return;
		}
	}
	std::cerr << "Array is sorted" << std::endl;
}

inline std::vector<uint64_t> generate_jacobsthal_AVX(size_t size) {
    size_t static_size = sizeof(jacobsthal_table) / sizeof(jacobsthal_table[0]);
    std::vector<uint64_t> jacobsthal(size);
    size_t limit = std::min(size, static_size);
    
	for (size_t i = 0; i < limit; ++i)
	{
		jacobsthal[i] = jacobsthal_table[i];
		#ifndef DEBUG
			std::cout << "jacobsthal[" << i << "] = " << jacobsthal[i] << std::endl;
		#endif
	}
    if (size > static_size)
        calculate_jacobsthal_avx(jacobsthal, static_size, size);
    return jacobsthal;
}

void compare_pairs_avx(std::vector<std::pair<int, int> >& pairs) {
    size_t n = pairs.size();
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
		if (i + 8 < n) 
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
		if (i + 8 < n)
			_mm_prefetch((const char*)&pairs[i + 8], _MM_HINT_T0);
        for (int j = 0; j < 8; ++j) {
            pairs[i + j].first = results_min[j];
            pairs[i + j].second = results_max[j];
        }
    }
	if (i + 8 < n) {
		_mm_prefetch((const char*)&pairs[i + 8], _MM_HINT_T0);
	}
    for (; i < n; ++i) {
        int min_value = std::min(pairs[i].first, pairs[i].second);
        int max_value = std::max(pairs[i].first, pairs[i].second);
        pairs[i].first = min_value;
        pairs[i].second		= max_value;
		pairs[i + 1].first	= min_value;
		pairs[i + 1].second	= max_value;
		pairs[i + 2].first	= min_value;
		pairs[i + 2].second	= max_value;
		pairs[i + 3].first	= min_value;
		pairs[i + 3].second	= max_value;
		pairs[i + 4].first	= min_value;
		pairs[i + 4].second	= max_value;
		pairs[i + 5].first	= min_value;
		pairs[i + 5].second	= max_value;
		pairs[i + 6].first	= min_value;
		pairs[i + 6].second	= max_value;
		pairs[i + 7].first	= min_value;
		pairs[i + 7].second	= max_value;

    }
}

struct ComparePairs {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
        return a.second < b.second;
    }
};

inline void insertion(std::vector<int>& arr, int value) {
    int left = 0;
    int right = arr.size() - 1;
    int pos = arr.size();
    while (left <= right) {
        int mid = left + ((right - left) >> 1);
        if (arr[mid] < value) {
            left = mid + 1;
        } else {
            pos = mid;
            right = mid - 1;
        }
    }
    arr.insert(arr.begin() + pos, value);
}


std::vector<int>& ford_johnson_sort(std::vector<int>& arr) {
    int n = arr.size();
    int straggler = -1;
    bool has_straggler = false;
    if ((n & 1) != 0) {
        has_straggler = true;
        straggler = arr.back();
        arr.pop_back();
        n--;
    }
    std::vector<std::pair<int, int> > pairs;
    for (int i = 0; i < n; i += 2) {
        pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
    }
    compare_pairs_avx(pairs);
    std::sort(pairs.begin(), pairs.end(), ComparePairs());
    std::vector<int> S;
    std::vector<int> pend;
    S.reserve(pairs.size());
    pend.reserve(pairs.size());
    for (size_t i = 0; i < pairs.size(); ++i) {
        S.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
    }
    std::vector<uint64_t> jacobsthal_sequence = generate_jacobsthal_AVX(pend.size());
    std::set<int> inserted_values;
    for (size_t i = 0; i < jacobsthal_sequence.size(); ++i) {
        size_t idx = jacobsthal_sequence[i];
        if (idx < pend.size() && inserted_values.find(pend[idx]) == inserted_values.end()) {
            insertion(S, pend[idx]);
            inserted_values.insert(pend[idx]);
        }
    }
    if (has_straggler) {
        if (inserted_values.find(straggler) == inserted_values.end()) {
            insertion(S, straggler);
        }
    }
    for (size_t i = 0; i < pend.size(); ++i) {
        if (inserted_values.find(pend[i]) == inserted_values.end()) {
            insertion(S, pend[i]);
            inserted_values.insert(pend[i]);
        }
    }
    arr.swap(S);
    return arr;
}

int main(int argc, char* argv[]) {
    std::vector<int> data;
    std::vector<int> data2;

	std::ios::sync_with_stdio(false);
	if (argc <= 2) {
		std::cout << "Usage: " << argv[0] << " <number1> <number2> ... <numberN>" << std::endl;
		std::cout << " " << std::endl;
		std::cout << "Example: " << argv[0] << "`shuf -i 1-N -n N | tr " "`" << std::endl; 
		return 0;
	}
    for (int i = 1; i < argc; ++i) {
        std::string argument(argv[i]);

		if (argument.empty()) {
			std::cout << "Invalid argument: " << argument << std::endl;
			return 0;
		}
        if (!argument.empty()) { 
            data.push_back(std::atoi(argument.c_str()));
        }
    }

    if (data.empty()) {
        std::cout << "Nothing to sort" << std::endl;
        return 0;
    }

    data2.insert(data2.begin(), data.begin(), data.end());

    std::cout << "Before :";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << " " << data[i];
    }
    std::cout << "\n"; 

    std::clock_t start = std::clock();
    ford_johnson_sort(data);
    std::clock_t end = std::clock();
    double time1 = (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC;  
    start = std::clock();
    ford_johnson_sort(data2);
    end = std::clock();
    double time2 = (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC; 

    std::cout << "After  :";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << " " << data[i]; 
    }
    std::cout << "\n";
	check_if_sorted(data);
	check_if_sorted(data2);
    std::cout << "Time for first sort (vector): " << time1 << " us" << std::endl;
    std::cout << "Time for second sort (deque): " << time2 << " us" << std::endl;

    return 0;
}
