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

void unalign(void *ptr);
void check_and_print_alignment(void* ptr, std::size_t alignment);
void* check_alignment(void* ptr, std::size_t alignment);
bool is_aligned_32(const void* ptr);
void calculate_jacobsthal_avx(std::vector<uint64_t>& jacobsthal, size_t start, size_t size); 
void check_if_sorted(const std::vector<int>& arr);
std::vector<uint64_t> generate_jacobsthal_AVX(size_t size);
void compare_pairs_avx(std::vector<std::pair<int, int> >& pairs);
void insertion(std::vector<int>& arr, int value);
std::vector<int> ford_johnson_sort(std::vector<std::pair<int, int> >& pairs, int straggler, bool has_straggler) ;


void insertion_deque(std::deque<int>& arr, int value);
void compare_pairs_avx_deque(std::deque<std::pair<int, int> >& pairs);
std::deque<int>& ford_johnson_sort_deque(std::deque<int>& arr); 
void calculate_jacobsthal_deque(std::deque<uint64_t>& jacobsthal, size_t start, size_t size); 
std::deque<uint64_t> generate_jacobsthal_deque(size_t size); 
void check_if_sorted_deque(const std::deque<int>& arr);
