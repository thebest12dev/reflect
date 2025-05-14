#include "TypeDefinitions.h"
#include <Console.h>
#include <Windows.h>
#include <cassert>
#include <cstring> // For memset
#include <iostream>
#include <new>
#include <string>
#include <vector>

namespace reflect {
REFLECT_API void initializeHeapPool(size_t size);
REFLECT_API void cleanupHeapPool();

class HeapPool {
protected:
  std::size_t poolSize = 64 * 1024 * 1024; // 64MB pool
  char *pool = nullptr;
  char *loc = nullptr;
  size_t usedMemory = 0; // Track used memory
  // Free list node structure
  struct FreeBlock {
    FreeBlock *next; // Pointer to the next free block
  };

  FreeBlock *freeList = nullptr;
  REFLECT_API ~HeapPool();
  REFLECT_API HeapPool(size_t poolSize);
  friend void initializeHeapPool(size_t size);
  friend void cleanupHeapPool();

public:
  REFLECT_API void *allocate(std::size_t size);

  REFLECT_API void deallocate(void *ptr, std::size_t size);
};
REFLECT_API HeapPool *getHeapPool();
} // namespace reflect

// Overriding new/delete operators
