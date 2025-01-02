#include "PmergeMe.hpp"
#include <xmmintrin.h>

__attribute__((always_inline, hot))
static inline void copy_pairs(const std::vector<std::pair<int, int> >& pairs,
                              std::vector<std::pair<int, int> >& leftArray,
                              std::vector<std::pair<int, int> >& rightArray,
                              int left, int middle, int n1, int n2) {
    int i = 0;
    for (; i + 3 < n1; i += 4) {
        _mm_prefetch((const char*)&pairs[left + i + 4], _MM_HINT_T0);
        _mm_prefetch((const char*)&leftArray[i + 4], _MM_HINT_T0);

        __m256i data = load_256(&pairs[left + i]);
        store_256(&leftArray[i], data);
    }
    for (; i < n1; ++i) {
        leftArray[i] = pairs[left + i];
    }

    int j = 0;
    for (; j + 3 < n2; j += 4) {
        _mm_prefetch((const char*)&pairs[middle + 1 + j + 4], _MM_HINT_T0);
        _mm_prefetch((const char*)&rightArray[j + 4], _MM_HINT_T0);

        __m256i data = load_256(&pairs[middle + 1 + j]);
        store_256(&rightArray[j], data);
    }
    for (; j < n2; ++j) {
        rightArray[j] = pairs[middle + 1 + j];
    }
}

__attribute__((always_inline, hot))
static inline void merge_pairs(std::vector<std::pair<int, int> >& pairs, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    std::vector<std::pair<int, int> > leftArray(n1);
    std::vector<std::pair<int, int> > rightArray(n2);
	leftArray.resize(n1);
	rightArray.resize(n2);
	leftArray.reserve(n1);
	rightArray.reserve(n2);

    copy_pairs(pairs, leftArray, rightArray, left, middle, n1, n2);
    int i = 0, j = 0, k = left;

	while (i + 3 < n1 && j + 3 < n2) {
		if (ComparePairs()(leftArray[i], rightArray[j]))
			pairs[k++] = leftArray[i++];
		else 
			pairs[k++] = rightArray[j++];

		if (ComparePairs()(leftArray[i], rightArray[j]))
			pairs[k++] = leftArray[i++];
		else
			pairs[k++] = rightArray[j++];

		if (ComparePairs()(leftArray[i], rightArray[j])) 
			pairs[k++] = leftArray[i++];
		else 
			pairs[k++] = rightArray[j++];

		if (ComparePairs()(leftArray[i], rightArray[j])) 
			pairs[k++] = leftArray[i++];
		else
			pairs[k++] = rightArray[j++];
	}

	while (i < n1 && j < n2) {
		if (ComparePairs()(leftArray[i], rightArray[j])) 
			pairs[k++] = leftArray[i++];
		else
			pairs[k++] = rightArray[j++];
	}

	while (i + 3 < n1) {
		pairs[k++] = leftArray[i++];
		pairs[k++] = leftArray[i++];
		pairs[k++] = leftArray[i++];
		pairs[k++] = leftArray[i++];
	}

	while (i < n1) 
		pairs[k++] = leftArray[i++];

	while (j + 3 < n2) {
		pairs[k++] = rightArray[j++];
		pairs[k++] = rightArray[j++];
		pairs[k++] = rightArray[j++];
		pairs[k++] = rightArray[j++];
	}

	while (j < n2) 
		pairs[k++] = rightArray[j++];

}

void sort_pairs(std::vector<std::pair<int, int> >& pairs, int left, int right) {
    const int threshold = 32;
    std::vector<std::pair<int, int> > leftBuffer((right - left + 1) / 2 + 1);
    std::vector<std::pair<int, int> > rightBuffer((right - left + 1) / 2 + 1);
	leftBuffer.reserve((right - left + 1) / 2 + 1);
	rightBuffer.reserve((right - left + 1) / 2 + 1);

    if (right - left <= threshold) {
        std::sort(pairs.begin() + left, pairs.begin() + right + 1, ComparePairs());
        return;
    }

    int middle = left + ((right - left) >> 1);
    sort_pairs(pairs, left, middle);
    sort_pairs(pairs, middle + 1, right);
    merge_pairs(pairs, left, middle, right);
}

std::vector<int> ford_johnson_sort(std::vector<std::pair<int, int> >& pairs, int straggler, bool has_straggler) {
    compare_pairs_avx(pairs);
    sort_pairs(pairs, 0, pairs.size() - 1);

    size_t size = pairs.size();
    std::vector<int> S;
    std::vector<int> pend;
    S.reserve(size);
    pend.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        S.push_back(pairs[i].second);
        pend.push_back(pairs[i].first);
    }

    std::vector<uint64_t> jacobsthal_sequence = generate_jacobsthal_AVX(pend.size());
    std::vector<bool> inserted(pend.size(), false);

    for (size_t i = 0; i < jacobsthal_sequence.size(); ++i){
        size_t idx = jacobsthal_sequence[i];
        if (idx < pend.size() && !inserted[idx]) {
            insertion(S, pend[idx]);
            inserted[idx] = true;
        }
    }

    if (has_straggler)
        insertion(S, straggler);

    for (size_t i = 0; i < pend.size(); ++i) {
        if (!inserted[i]) {
            insertion(S, pend[i]);
            inserted[i] = true;
        }
    }

    return S;
}
