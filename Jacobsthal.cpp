#include "PmergeMe.hpp"

__attribute__((always_inline))
inline void calculate_jacobsthal_avx(std::vector<uint64_t>& jacobsthal, size_t start, size_t size) {
    if (__builtin_expect(size < 2, 0))
		return;

    uint64_t prev2 = jacobsthal[start - 2];
    uint64_t prev1 = jacobsthal[start - 1];
    size_t i = start;
	jacobsthal.reserve(size);
    for (; i + 4 <= size; i += 4) {
        __m256i prev1_vec = _mm256_set1_epi64x(prev1);
        __m256i prev2_vec = _mm256_set1_epi64x(prev2);
        __m256i two_prev2_vec = _mm256_slli_epi64(prev2_vec, 1); 
        __m256i jacobsthal_vec = _mm256_add_epi64(prev1_vec, two_prev2_vec);
        _mm256_storeu_si256((__m256i*)&jacobsthal[i], jacobsthal_vec);

        prev2 = jacobsthal[i + 2];
        prev1 = jacobsthal[i + 3];
    }

    for (; i < size; ++i) {
        jacobsthal[i] = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = jacobsthal[i];
    }
}

std::vector<uint64_t> generate_jacobsthal_AVX(size_t size) {
    size_t static_size = sizeof(jacobsthal_table) / sizeof(jacobsthal_table[0]);
    std::vector<uint64_t> jacobsthal(size);
	jacobsthal.reserve(size);
    size_t limit = std::min(size, static_size);

    for (size_t i = 0; i < limit; ++i)
        jacobsthal[i] = jacobsthal_table[i];

    if (__builtin_expect(size > static_size, 0))
        calculate_jacobsthal_avx(jacobsthal, static_size, size);

    return jacobsthal;
}
