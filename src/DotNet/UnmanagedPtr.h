// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

/// Non ref counted smart pointer.
template<typename T>
ref class UnmanagedPtr
{
public:
  UnmanagedPtr():ptr_(0), size_(0){}

  UnmanagedPtr(T* ptr):ptr_(ptr), size_(sizeof(*ptr_))
  {
    //System::GC::AddMemoryPressure(size_);
  }

  T& GetRef()
  {
    return *ptr_;
  }

  T* operator->()
  {
    return ptr_;
  }

  !UnmanagedPtr() { Release(); }
  ~UnmanagedPtr() { Release(); }

  void Release()
  {
    if(ptr_ != 0)
    {
      delete ptr_;
      //System::GC::RemoveMemoryPressure(size_);
      ptr_ = 0;
      size_ = 0;
    }
  }

private:
  T* ptr_;
  long long size_;

};
