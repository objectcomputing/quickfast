// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

template <typename T>
ref class BoostPtrHolder
{
public:
  typedef typename T::element_type* TPointer;
  typedef typename T::element_type TValue;
  typedef BoostPtrHolder<T> TMyType;

  BoostPtrHolder()
    : ptr_(new T)
    , cptr_(0)
  {}

  explicit BoostPtrHolder(const T& rhs)
    : ptr_(new T(rhs))
    , cptr_(rhs.get())
  {
    //System::GC::AddMemoryPressure(sizeof(TValue));
  }

  template <typename COMPATABLE_BOOST_PTR>
  void Assign(COMPATABLE_BOOST_PTR rhs)
  {
    (*ptr_) = rhs;
    cptr_ = ptr_->get();
  }

  ~BoostPtrHolder()
  {
    Release();
  }

  !BoostPtrHolder()
  {
    Release();
  }

  // Note not a reference.  You can't change it this way.
  T GetBoostPtr()
  {
    return *ptr_;
  }

  T& operator=(T& rhs)
  {
    (*ptr_) = rhs;
    cptr_ = rhs.get();
  }

  TValue& GetRef()
  {
    return *cptr_;
  }

  TValue * operator->()
  {
    return cptr_;
  }

  void Release()
  {
    cptr_ = 0;
    if(ptr_ != 0)
    {
      delete ptr_;
      //System::GC::RemoveMemoryPressure(sizeof(TValue));
      ptr_ = 0;
    }
  }

private:
  T* ptr_;
  TValue * cptr_;
};
