#include "PmergeMe.hpp"
#include <xmmintrin.h>

__attribute__((always_inline, cold))
static inline void vectorized_copy(const std::vector<std::pair<int, int> >& pairs,
                     std::vector<std::pair<int, int> >& leftArray,
                     std::vector<std::pair<int, int> >& rightArray,
                     int left, int middle, int n1, int n2) 
{
    int i = 0;
    for (; i + 4 <= n1; i += 4) {
        __m256i firsts = _mm256_loadu_si256((__m256i*)&pairs[left + i].first);
        __m256i seconds = _mm256_loadu_si256((__m256i*)&pairs[left + i].second);

        _mm256_storeu_si256((__m256i*)&leftArray[i].first, firsts);
        _mm256_storeu_si256((__m256i*)&leftArray[i].second, seconds);
    }

    for (; i < n1; ++i) {
        leftArray[i] = pairs[left + i];
    }

    int j = 0;
    for (; j + 4 <= n2; j += 4) {
        __m256i firsts = _mm256_loadu_si256((__m256i*)&pairs[middle + 1 + j].first);
        __m256i seconds = _mm256_loadu_si256((__m256i*)&pairs[middle + 1 + j].second);

        _mm256_storeu_si256((__m256i*)&rightArray[j].first, firsts);
        _mm256_storeu_si256((__m256i*)&rightArray[j].second, seconds);
    }

    for (; j < n2; ++j) {
        rightArray[j] = pairs[middle + 1 + j];
    }
}

__attribute__((flatten))
void merge_pairs(std::vector<std::pair<int, int> >& pairs, int left, int middle, int right) 
{
    int n1 = middle - left + 1;
    int n2 = right - middle;

    std::vector<std::pair<int, int> > leftArray(n1);
    std::vector<std::pair<int, int> > rightArray(n2);
	leftArray.reserve(n1);
	rightArray.reserve(n2);

    vectorized_copy(pairs, leftArray, rightArray, left, middle, n1, n2);

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) 
    {
        if (ComparePairs()(leftArray[i], rightArray[j])) 
        {
            pairs[k] = leftArray[i];
            ++i;
        } 
        else 
        {
            pairs[k] = rightArray[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) 
    {
        pairs[k] = leftArray[i];
        ++i;
        ++k;
    }

    while (j < n2) 
    {
        pairs[k] = rightArray[j];
        ++j;
        ++k;
    }
}

void sort_pairs(std::vector<std::pair<int, int> >& pairs, int left, int right) 
{
    if (__builtin_expect(left < right, 1)) 
    {
		int middle = left + ((right - left) >> 1);

        sort_pairs(pairs, left, middle);
        sort_pairs(pairs, middle + 1, right);

        merge_pairs(pairs, left, middle, right);
    }
}

std::vector<int> ford_johnson_sort(std::vector<std::pair<int, int> >& pairs, int straggler, bool has_straggler) 
{
    compare_pairs_avx(pairs);
    sort_pairs(pairs, 0, pairs.size() - 1);

    size_t size = pairs.size();
    std::vector<int> S;
    std::vector<int> pend;
    S.reserve(size);
    pend.reserve(size);

    for (size_t i = 0; i < size; ++i) 
    {
        S.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
    }

    std::vector<uint64_t> jacobsthal_sequence = generate_jacobsthal_AVX(pend.size());
    std::vector<bool> inserted(pend.size(), false);

    for (size_t i = 0; i < jacobsthal_sequence.size(); ++i)
    {
        size_t idx = jacobsthal_sequence[i];
        if (idx < pend.size() && !inserted[idx]) 
        {
            insertion(S, pend[idx]);
            inserted[idx] = true;
        }
    }

    if (has_straggler)
    {
        insertion(S, straggler);
    }

    for (size_t i = 0; i < pend.size(); ++i) 
    {
        if (!inserted[i]) 
        {
            insertion(S, pend[i]);
            inserted[i] = true;
        }
    }

    return S;
}
