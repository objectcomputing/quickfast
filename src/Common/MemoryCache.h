// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MEMORY_CACHE
#define MEMORY_CACHE

#include <Common/AtomicOps.h>
#define DISABLE_CACHE

namespace QuickFAST
{
  namespace Cache
  {
    /// An entry for the memory cache.  Placeholder the buffer for the actual object.
    struct CacheEntry
    {
    public:
      CacheEntry(size_t size)
        : size_(size)
        , next_(0)
      {
      }
      size_t size_;
      CacheEntry * next_;
    };

    struct Synchronized
    {
      static bool store(CacheEntry * & root, CacheEntry * ifeq, CacheEntry * value)
      {
        void * volatile * target = reinterpret_cast<void * volatile *>(&root);
        return CASPtr(target, ifeq, value);
      }
    };
    struct Unsynchronized
    {
      static bool store(CacheEntry * & root, CacheEntry * ifeq, CacheEntry * value)
      {
        root = value;
        return true;
      }
    };
  }

  /// @brief A cache of buffers of size_ or fewer bytes
  ///
  /// Also manages buffers of non-standard size.
  template<typename SYNC = Cache::Synchronized>
  class MemoryCache
  {
    typedef Cache::CacheEntry CacheEntry;
  private:
    MemoryCache(const MemoryCache<SYNC> & rhs);
    MemoryCache();

  public:
    explicit MemoryCache(size_t size, size_t preallocate/* = 0*/)
    : size_(size)
    , root_(0)
    , allocs_(0)
    , hits_(0)
    , misses_(0)
    , frees_(0)
    , grows_(0)
    , oddSizes_(0)
    , largest_(0)
    {
#ifndef DISABLE_CACHE
      // no synchronization is needed during construction.
      for(size_t nEntry = 0; nEntry < preallocate; ++nEntry)
      {
        ++allocs_;
        CacheEntry * entry = new (new char[size_ + sizeof(CacheEntry)]) CacheEntry(size_);
//        std::cout << (void *) this << " Allocates cache buffer[" << size_ << "] @" << (void *) entry << std::endl;
        free(entry + 1);
      }
#endif
    }

    ~MemoryCache()
    {
      // No synchronization is needed during destruction.
      while(root_ != 0)
      {
        CacheEntry * entry = root_;
//        std::cout << (void *) this << " destructor frees cache buffer[" << size_ << "] @" << (void *) entry << std::endl;

        root_ = entry->next_;
        delete [] reinterpret_cast<char *>(entry);
      }
    }

    void free(void * buffer)
    {
      free(reinterpret_cast<CacheEntry *>(buffer));
    }

    void free(const void * buffer)
    {
      free(const_cast<free(CacheEntry *>(
        reinterpret_cast<const free(CacheEntry *>(buffer)))));
    }

    void free(CacheEntry * entry)
    {
      ++frees_;
      entry -= 1;
      if(
#ifdef DISABLE_CACHE
        true ||
#endif
        entry->size_ != size_)
      {
//        std::cout << (void *) this << " free odd cache buffer[" << size_ << " != " << entry->size_ << "] @" << (void *) entry << std::endl;
        delete [] reinterpret_cast<char *>(entry);
      }
      else
      {
        bool stored = false;
        while(!stored)
        {
          entry->next_ = root_;
          stored = SYNC::store(root_, entry->next_, entry);
        }
      }
    }

    void * allocate(size_t needed)
    {
      ++allocs_;
      CacheEntry * entry = 0;
      if(needed <= size_)
      {
        while (entry == 0 && root_ != 0)
        {
          CacheEntry * next = root_;
          if(SYNC::store(root_, next, next->next_))
          {
            entry = next;
            ++hits_;
          }
        }
        if(entry == 0)
        {
          ++misses_;
          entry = new (new char[size_ + sizeof(CacheEntry)]) CacheEntry(size_);
//          std::cout << (void *) this << " Grow cache buffer[" << size_ << "] @" << (void *) entry << std::endl;
        }
      }
      else
      {
        ++oddSizes_;
        largest_ = std::max(largest_, needed);
        entry = new (new char[needed + sizeof(CacheEntry)]) CacheEntry(needed);
//        std::cout << (void *) this << " Odd cache buffer[" << size_ << "] @" << (void *) entry << std::endl;
      }
      return reinterpret_cast<void *> (entry + 1);
    }

  public:
    size_t getStats(
      size_t & allocs,
      size_t & hits,
      size_t & frees,
      size_t & grows,
      size_t & oddSizes,
      size_t & largest
      )const
    {
      allocs = allocs_;
      hits = hits_;
      frees = frees_;
      grows = grows_;
      oddSizes = oddSizes_;
      largest = largest_;
    }

    size_t getAndClearStats(
      size_t & allocs,
      size_t & hits,
      size_t & frees,
      size_t & grows,
      size_t & oddSizes,
      size_t & largest
      )const
    {
      allocs = allocs_;
      hits = hits_;
      hits = hits_;
      frees = frees_;
      grows = grows_;
      oddSizes = oddSizes_;
      largest = largest_;

      allocs_ = 0;
      hits_ = 0;
      misses_ = 0;
      frees_ = 0;
      grows_ = 0;
      oddSizes_ = 0;
      largest_ = 0;
    }

  private:
    size_t size_;
    CacheEntry * root_;

    //////////////
    // Statistics
    size_t allocs_;
    size_t hits_;
    size_t misses_;
    size_t frees_;
    size_t grows_;
    size_t oddSizes_;
    size_t largest_;
  };
}
#endif // MEMORY_CACHE
