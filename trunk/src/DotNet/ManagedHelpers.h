// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#pragma once

private ref class NullPointerException: System::Exception
{
};


#ifdef BOOST_SHARED_PTR_HPP_INCLUDED
/// <summary>
// This is used to prevent a boost::shared_ptr from freeing its
// pointer upon going out of scope.  This is usefull when a shared_ptr
// must be passed to a function or method from a .NET entity.
// Managed classes can not have unmanaged entities as members.
// Pointers to unmanaged entities are the exception.
//
// Pass an instance of the null_deleter in the constructor of the shared_ptr
/// </summary>
template<typename T>
class null_deleter
{
public:
    void operator()(T* p) {}
};

#endif //BOOST_SHARED_PTR_HPP_INCLUDED


template<typename TPtrType>
struct IDataAccess
{
    typedef TPtrType* TPointer;
    virtual void GetPtr(TPointer*) = 0;
};

struct IRefCounter
{
    virtual void AddRef() = 0;
    virtual void Release() = 0;
};

template<typename TPtrType>
class RefCounter:
  public IDataAccess<TPtrType>,
  public IRefCounter
{
public:
    typedef TPtrType* TPointer;
    typedef RefCounter<TPtrType> TRefCounter;

    RefCounter(TPtrType* ptr): m_refCount(1), m_ptr(ptr)
    {
        if (ptr == 0)
        {
            throw gcnew System::Exception("Null initialization pointer.");
        }
    }

    ~RefCounter()
    {
        if(m_ptr != 0)
        {
            delete m_ptr;
        }
    }

    void AddRef()
    {
        ++m_refCount;
    }

    void Release()
    {
        if(--m_refCount < 1)
        {
            delete this;
        }
    }

    void GetPtr(TPtrType** pPtr)
    {
        *pPtr = m_ptr;
    }

private:
    unsigned long m_refCount;
    TPtrType* m_ptr;
};


/// <summary>
/// This is a ref counted smart pointer somewhat like a boost::shared_ptr
/// It is used to hold an unmanaged pointer in a managed class
/// The unmanaged pointer is freed when the NativePtr goes out of
/// scope.
/// </summary>
template <class TUnmanaged>
private ref class NativePtr sealed
{
public:

#ifdef BOOST_SHARED_PTR_HPP_INCLUDED
  typedef boost::shared_ptr<TUnmanaged> TUnmanagedPtr;
#endif //BOOST_SHARED_PTR_HPP_INCLUDED

  NativePtr():m_pRefCounter(NULL)
  {
  }

  NativePtr(TUnmanaged* ptr):m_pRefCounter(new RefCounter<TUnmanaged>(ptr))
  {
  }

  NativePtr(NativePtr% nativePtr)
  {
    m_pRefCounter = nativePtr.m_pRefCounter;
    if(m_pRefCounter != NULL)
    {
      m_pRefCounter->AddRef();
    }
  }

  NativePtr(IDataAccess<TUnmanaged>* refCounter):
    m_pRefCounter(dynamic_cast<IRefCounter*>(refCounter))
  {

  }

  ~NativePtr()
  {
    this->!NativePtr();
  }

  !NativePtr()
  {
    DeleteReference();
  }

  /// <summary>
  ///   Returns the underlying unmanaged pointer.
  ///   The caller of this method should take care not to use
  ///   the returned pointer after this NativePtr has gone out of
  ///   scope.  The returned pointer is no longer guaranteed to be
  ///   valid once this happens.
  ///
  ///   Be careful of the this syntax:
  ///
  ///    someVar->NativePtr->Ptr();  //Very bad!!!
  ///
  ///   Depending on the implementation the NativePtr property depicted above may
  ///   produce a temporary NativePtr object the goes out of scope immediately following
  ///   the line.  The returned pointer could become invalid as soon as the garbage collector
  ///   runs.
  ///
  ///   NativePtr<SomeClass> spMyClass;
  ///   spMyClass->Ptr();
  ///
  ///   Much better because the returned pointer will be valid until spMyClass goes
  ///   out of scope.
  /// </summary>
  TUnmanaged* Ptr()
  {
    if(m_pRefCounter != NULL)
    {
      return GetPointer(m_pRefCounter);
    }
    else
    {
      return 0;
    }
  }

  bool IsNull()
  {
    return m_pRefCounter == NULL;
  }

  /// <summary>
  ///   Takes ownership of the supplied unmanaged pointer.
  ///   The NativePtr must not already be bound to another resource.
  /// </summary>
  void Bind(TUnmanaged* ptr)
  {
    if(m_pRefCounter != NULL)
    {
      throw gcnew System::InvalidOperationException("Already bound. Can not rebind before Release.");
    }
    m_pRefCounter = new RefCounter<TUnmanaged>(ptr);
  }

  /// <summary>
  ///  Relese the unmanaged resource.
  /// </summary>
  void Release()
  {
    if(m_pRefCounter != NULL)
    {
      m_pRefCounter->Release();
      m_pRefCounter = NULL;
    }
  }

#ifdef BOOST_SHARED_PTR_HPP_INCLUDED
  /// <summary>
  ///  Casts this managed NativePtr to an unmanaged
  ///  boost::shared_ptr.  The lifetime of the encapsulated
  ///  pointer is maintained by this NativePtr.
  /// </summary>
  operator TUnmanagedPtr()
  {
    null_deleter<TUnmanaged> deleter;
    return TUnmanagedPtr(GetPointer(m_pRefCounter), deleter);
  }
#endif //BOOST_SHARED_PTR_HPP_INCLUDED


  /// <summary>
  ///  Returns true if this NativePtr is holding a null pointer.
  /// </summary>

  NativePtr% operator =(NativePtr% rhs)
  {
    if(m_pRefCounter != NULL)
    {
      m_pRefCounter->Release();
    }

    m_pRefCounter = rhs.m_pRefCounter;
    m_pRefCounter->AddRef();
    return *this;
  }

  static TUnmanaged* operator ->(NativePtr% lhs)
  {
    if(lhs.m_pRefCounter != NULL)
    {
      return GetPointer(m_pRefCounter);
    }
    else
    {
        throw gcnew NullPointerException();
    }
  }

  static TUnmanaged& operator *(NativePtr% rhs)
  {
    if(rhs.m_pRefCounter == NULL)
    {
      throw gcnew NullPointerException();
    }
    return *GetPointer(rhs.m_pRefCounter);
  }


private:

  IRefCounter* m_pRefCounter;

  TUnmanaged* GetPointer(IRefCounter* refCounter)
  {
    TUnmanaged* retPtr;
    dynamic_cast<IDataAccess<TUnmanaged>* >(refCounter)->Ptr(&retPtr);
    return retPtr;
  }

  void DeleteReference()
  {
    if(m_pRefCounter != NULL)
    {
        try
        {
            m_pRefCounter->Release();
        } catch(System::Exception^ /*e*/) {
            //Todo:  do some logging here.
        } catch(...) {
            //Todo: do some logging here.
        } finally {
            m_pRefCounter = NULL;
        }
    }
  }
};
