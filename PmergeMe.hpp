#pragma once

#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#include <iostream>
#include <stdint.h>
#include <ctime>
#include <deque>
#include <immintrin.h>
#include <stdlib.h>
#include <dlfcn.h>

#define align	__attribute__((aligned(32)))
#define SYM		0x0000000065657266

struct ComparePairs 
{
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const 
	{
        return a.second < b.second;
    }
} align;

__attribute__((always_inline))
inline __m256i load_256(const void* ptr) {
    if (!((uint64_t)ptr & 31)) {
        return _mm256_load_si256((__m256i*)ptr);
    } else {
        return _mm256_loadu_si256((__m256i*)ptr);
    }
}

__attribute__((always_inline))
inline void store_256(void* ptr, __m256i data) {
    if (!((uint64_t)ptr & 31)) {
        _mm256_store_si256((__m256i*)ptr, data);
    } else {
        _mm256_storeu_si256((__m256i*)ptr, data);
    }
}

void* check_alignment(void* ptr, std::size_t alignment);
bool is_aligned_32(const void* ptr);
void calculate_jacobsthal_avx(std::vector<uint64_t>& jacobsthal, size_t start, size_t size); 
void check_if_sorted(const std::vector<int>& arr);
std::vector<uint64_t> generate_jacobsthal_AVX(size_t size);
void compare_pairs_avx(std::vector<std::pair<int, int> >& pairs);
void insertion(std::vector<int>& arr, int value);
std::vector<int> ford_johnson_sort(std::vector<std::pair<int, int> >& pairs, int straggler, bool has_straggler) ;
std::deque<int> ford_johnson_sort_deque(std::deque<std::pair<int, int> >& pairs, int straggler, bool has_straggler); 

void insertion_deque(std::deque<int>& arr, int value);
void compare_pairs_avx_deque(std::deque<std::pair<int, int> >& pairs);
void calculate_jacobsthal_deque(std::deque<uint64_t>& jacobsthal, size_t start, size_t size); 
std::deque<uint64_t> generate_jacobsthal_deque(size_t size); 
void check_if_sorted_deque(const std::deque<int>& arr);
