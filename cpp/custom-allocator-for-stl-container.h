#pragma once

#include "mimalloc.h"
#include <stdlib.h>
#include <malloc.h>

/**
 * Custom allocator with MS's mimalloc library for STL container
 */
template <typename T, std::size_t N = 16>
class custom_allocator
{
public:
  typedef T value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  typedef T* pointer;
  typedef const T* const_pointer;

  typedef T& reference;
  typedef const T& const_reference;

public:
  inline custom_allocator() throw () { }

  template <typename T2>
  inline custom_allocator(const custom_allocator<T2, N>&) throw () { }
  
  inline ~custom_allocator() throw () { }
  
  /**
   * Get address of a reference
   * @param[in] r The reference that gets address.
   */
  inline pointer adress(reference r)
  {
    return &r;
  }

  inline const_pointer adress(const_reference r) const
  {
    return &r;
  }

  inline pointer allocate(size_type n)
  {
    return (pointer)mi_malloc_aligned(n * sizeof(value_type), N);
  }

  inline void deallocate(pointer p, size_type)
  {
    mi_free_aligned(p, N);
  }

  inline void construct(pointer p, const value_type& wert)
  {
    new (p) value_type(wert);
  }

  inline void destroy(pointer p)
  {
    p->~value_type();
  }

  inline size_type max_size() const throw ()
  {
    return size_type(-1) / sizeof(value_type);
  }

  template <typename T2>
  struct rebind
  {
    typedef custom_allocator<T2, N> other;
  };
  
  /**
   * Inequality operator invokes operator!= on each element in order of vector.
   */
  bool operator!=(const custom_allocator<T, N>& other) const 
  {
    return !(*this == other);
  }
  
  /**
   * Returns true if and only if storage allocated from *this
   * can be deallocated from other, and vice versa.
   * Always returns true for stateless allocators.
   */  
  bool operator==(const custom_allocator<T, N>& other) const 
  {
    return true;
  }
};

/**
 * Eg. Custom for std::vector
 */
typedef std::vector<float, custom_allocator<float, 32>> MyVector;
