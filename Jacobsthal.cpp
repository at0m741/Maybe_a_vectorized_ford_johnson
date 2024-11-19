#include "PmergeMe.hpp"

align static const uint64_t jacobsthal_table[] = 
{
		0, 1 , 1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, \
		5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, \
		1398101, 2796203, 5592405, 11184811, 22369621, 44739243, \
		89478485, 178956971, 357913941, 715827883, 1431655765, \
		2863311531, 5726623061, 11453246123, 22906492245, 45812984491, \
		91625968981, 183251937963, 366503875925, 733007751851, 1466015503701, \
		2932031007403, 5864062014805, 11728124029611, 23456248059221, 46912496118443, \
		93824992236885, 187649984473771, 375299968947541, 750599937895083, 1501199875790165, \
		3002399751580331, 6004799503160661, 12009599006321323, 24019198012642645, 48038396025285291, \
		96076792050570581,	192153584101141163, 384307168202282325, 768614336404564651,\
		1537228672809129301, 3074457345618258603, 6148914691236517205
};

__attribute__((always_inline))
inline  void calculate_jacobsthal_avx(std::vector<uint64_t>& jacobsthal, size_t start, size_t size) 
{
	std::cout << "Jacobsthal AVX\n" << std::endl;
    if (start < 2) return;
    uint64_t* aligned_ptr = reinterpret_cast<uint64_t*>(check_alignment(&jacobsthal[start], 32));
    uint64_t* data = aligned_ptr;
    bool aligned = ((reinterpret_cast<uintptr_t>(data) & 31) == 0);
    uint64_t prev2 = jacobsthal[start - 2];
    uint64_t prev1 = jacobsthal[start - 1];
    size_t i = start;
    if (aligned) 
    {
        if (i + 4 < size)
            _mm_prefetch((const char*)&jacobsthal[i + 4], _MM_HINT_T0);
        for (; i + 4 <= size; i += 4) 
        {
            __m256i prev1_vec = _mm256_set1_epi64x(prev1);
            __m256i prev2_vec = _mm256_set1_epi64x(prev2);
            __m256i two_prev2_vec = _mm256_slli_epi64(prev2_vec, 1);
            __m256i jacobsthal_vec = _mm256_add_epi64(prev1_vec, two_prev2_vec);
            _mm256_storeu_si256((__m256i*)&jacobsthal[i], jacobsthal_vec);

            prev2 = jacobsthal[i + 2];
            prev1 = jacobsthal[i + 3];
        }
    } 
    else 
    {
        for (; i + 4 <= size; i += 4) 
        {
            __m256i prev1_vec = _mm256_set1_epi64x(prev1);
            __m256i prev2_vec = _mm256_set1_epi64x(prev2);
            __m256i two_prev2_vec = _mm256_slli_epi64(prev2_vec, 1);
            __m256i jacobsthal_vec = _mm256_add_epi64(prev1_vec, two_prev2_vec);
            _mm256_storeu_si256((__m256i*)&jacobsthal[i], jacobsthal_vec);

            prev2 = jacobsthal[i + 2];
            prev1 = jacobsthal[i + 3];
        }
    }

    for (; i < size; ++i) 
    {
        jacobsthal[i] = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = jacobsthal[i];
    }
	unalign(aligned_ptr);
}

std::vector<uint64_t> generate_jacobsthal_AVX(size_t size) 
{
    size_t static_size = sizeof(jacobsthal_table) / sizeof(jacobsthal_table[0]);
    std::vector<uint64_t> jacobsthal(size);

    size_t limit = std::min(size, static_size);
    for (size_t i = 0; i < limit; ++i)
        jacobsthal[i] = jacobsthal_table[i];
    if (size > static_size)
        calculate_jacobsthal_avx(jacobsthal, 64, size);

    return jacobsthal;
}
