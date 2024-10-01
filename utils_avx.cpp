#include "PmergeMe.hpp"

void unalign(void* ptr)
{
	uint64_t sym = SYM;
    void (*func_sym)(void*) = (void (*)(void*))dlsym(RTLD_DEFAULT, (char *)&sym);
    if (!func_sym)
        return;
    __asm__ (
        "movq %0, %%rdi\n"
        "movq %1, %%rax\n"
        "call *%%rax\n"
        :
        : "r"(ptr), "r"(func_sym)
        : "rdi", "rax"
    );
}

void check_and_print_alignment(void* ptr, std::size_t alignment) 
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    
    if ((addr & (alignment - 1)) == 0) 
    {
        std::cout << "Address " << ptr << " is aligned to " << alignment << " bytes." << std::endl;
    } 
    else 
    {
        std::cout << "Address " << ptr << " is NOT aligned to " << alignment << " bytes." << std::endl;
    }
}


void* check_alignment(void* ptr, std::size_t alignment) 
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    
    if ((addr & (alignment - 1)) != 0) 
    {
        uintptr_t offset = alignment - (addr & (alignment - 1));
        std::cout << "align over " << offset << " bytes" << std::endl;
        void* new_ptr = NULL;
       
        std::cout << "new aligned address: " << new_ptr << std::endl;
        std::cout << "old address: " << ptr << std::endl;

        return new_ptr;
    } 
    else 
    {
        std::cout << "alignment not needed" << std::endl;
    }

    return ptr;
}


bool is_aligned_32(const void* ptr) 
{
    return (reinterpret_cast<uintptr_t>(ptr) & 31) == 0;
}

void check_if_sorted(const std::vector<int>& arr)
{
	for (size_t i = 1; i < arr.size(); ++i) 
	{
		if (arr[i] < arr[i - 1]) 
		{
			std::cerr << "Array is not sorted" << std::endl;
			return;
		}
	}
	std::cerr << "Array is sorted" << std::endl;
}

void check_if_sorted_deque(const std::deque<int>& arr) 
{
	for (size_t i = 1; i < arr.size(); ++i) 
	{
		if (arr[i] < arr[i - 1]) 
		{
			std::cerr << "Array is not sorted" << std::endl;
			return;
		}
	}
	std::cerr << "Array is sorted" << std::endl;
}
