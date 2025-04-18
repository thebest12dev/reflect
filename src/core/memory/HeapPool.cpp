#include "HeapPool.h"
#include "Console.h"
#include <new>
namespace {
CinnamonToast::HeapPool *heapPool = nullptr; // Pointer to a HeapPool instance
}
namespace CinnamonToast {

// Function to initialize heapPool
void initializeHeapPool(size_t size) {
  if (heapPool) {
    return; // Already initialized
  }
  // Allocate memory for HeapPool using std::malloc
  void *poolPtr = std::malloc(sizeof(HeapPool));
  // Construct the HeapPool object using placement new
  heapPool = new (poolPtr) HeapPool(size);
};
// Function to get the heapPool instance
HeapPool *getHeapPool() { return heapPool; }
// Function to clean up heapPool
void cleanupHeapPool() {
  if (heapPool) {
    heapPool->~HeapPool(); // Explicitly call the destructor
    std::free(heapPool);   // Free the allocated memory
  }
}
HeapPool::HeapPool(size_t size) : poolSize(size) {
  pool = (char *)VirtualAlloc(nullptr,    // Let the system choose the address
                              poolSize,   // Size of the memory to allocate
                              MEM_COMMIT, // Allocate and commit memory
                              PAGE_READWRITE // Read and write access
  );

  if (!pool) {
    throw std::bad_alloc(); // Handle allocation failure
  }

  // Optionally zero out the memory
  std::memset(pool, 0, poolSize);
  loc = pool;

  ctoastDebug("allocated " + std::to_string(poolSize / 1024 / 1024) +
              "mb of memory for heap pool!");
}

HeapPool::~HeapPool() {
  if (pool) {
    VirtualFree(pool, 0, MEM_RELEASE); // Free the memory when done
  }
}

void *HeapPool::allocate(std::size_t size) {
  // Round size to nearest multiple of alignment (e.g., 8 bytes)
  size = (size + 7) & ~7;

  // First, check if there is space in the free list
  if (freeList) {
    // Reuse a block from the free list
    FreeBlock *block = freeList;
    freeList = block->next;
    return block;
  }

  // If no free blocks, we allocate from the pool
  if (loc + size > pool + poolSize) {
    throw std::bad_alloc(); // Out of memory, handle error
  }

  void *ptr = loc;
  loc += size;
  usedMemory += size; // Increase used memory
  
  return ptr;
}

void HeapPool::deallocate(void *ptr, std::size_t size) {
  // Round size to nearest multiple of alignment (e.g., 8 bytes)
  size = (size + 7) & ~7;

  // Add the block back to the free list
  FreeBlock *block = reinterpret_cast<FreeBlock *>(ptr);
  block->next = freeList;
  freeList = block;
  usedMemory -= size; // Increase used memory
};
} // namespace CinnamonToast
// Overriding new/delete operators
// void *operator new(std::size_t size) {
//  if (!heapPool) {
//    CinnamonToast::initializeHeapPool();
//  }
//  void *ptr = heapPool->allocate(size);
//  if (!ptr) {
//    throw std::bad_alloc(); // Handle allocation failure
//  }
//  std::cout << "Allocated memory at " << ptr << "\n";
//  return ptr;
//}
//
// void operator delete(void *ptr) noexcept {
//  if (!heapPool) {
//    CinnamonToast::initializeHeapPool();
//  }
//  heapPool->deallocate(ptr, sizeof(ptr));
//  std::cout << "Deallocated memory at " << ptr << "\n";
//}
//
// void *operator new[](std::size_t size) {
//  if (!heapPool) {
//    CinnamonToast::initializeHeapPool();
//  }
//  void *ptr = heapPool->allocate(size);
//  std::cout << "Allocated memory at " << ptr << "\n";
//  return ptr;
//}
//
// void operator delete[](void *ptr) noexcept {
//  if (!heapPool) {
//    CinnamonToast::initializeHeapPool();
//  }
//  heapPool->deallocate(ptr, sizeof(ptr));
//  std::cout << "Deallocated memory at " << ptr << "\n";
//}
