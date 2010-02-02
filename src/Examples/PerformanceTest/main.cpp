// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#define PROFILER_ENABLE
#include <Common/Profiler.h>
#include <PerformanceTest/PerformanceTest.h>

using namespace QuickFAST;
using namespace Examples;

class B
{
public:
  B()
    : counter_(0)
    , refcount_(0)
  {
  }

  ~B()
  {
  }

  void touch()
  {
    ++counter_;
  }

  void freeB()const
  {
    delete this;
  }

  volatile int counter_;

  mutable unsigned long refcount_;
};
typedef boost::shared_ptr<B> BPtr;

typedef boost::intrusive_ptr<B> BIPtr;

inline
void intrusive_ptr_add_ref(const B * ptr)
{
  ++ptr->refcount_;
}
inline
void intrusive_ptr_release(const B * ptr)
{
  if(--ptr->refcount_ == 0)
  {
    ptr->freeB();
  }
}


void test1()
{
  PROFILE_POINT("SmCopy");
  BPtr b(new B);
  for(size_t i = 0; i < 100000; ++i)
  {
    BPtr b1;
    b1 = b;
  }
}

void test2()
{
  PROFILE_POINT("InCopy");
  BIPtr b(new B);
  for(size_t i = 0; i < 100000; ++i)
  {
    BIPtr b1;
    b1 = b;
  }
}

void test3()
{
  PROFILE_POINT("RwCopy");
  B * b = new B;
  for(size_t i = 0; i < 100000; ++i)
  {
    B * b1;
    b1 = b;
  }
  delete b;
}

void test11()
{
  PROFILE_POINT("SmUse ");
  BPtr b(new B);
  for(size_t i = 0; i < 100000; ++i)
  {
    b->touch();
  }
}

void test12()
{
  PROFILE_POINT("InUse ");
  BIPtr b = new B;
  for(size_t i = 0; i < 100000; ++i)
  {
    b->touch();
  }
}

void test13()
{
  PROFILE_POINT("RwUse ");
  B * b = new B;
  for(size_t i = 0; i < 100000; ++i)
  {
    b->touch();
  }
  delete b;
}

int main(int argc, char* argv[])
{
  if(argc <= 1)
  {
    for(size_t i = 0; i < 
#if 0
        1000; // runs about 6 seconds
#elif 10
      10000; // runs about a minute
#else
      100000; // runs about ten minutes
#endif
      ++i)
    {
      test1();
      test2();
      test3();
      test11();
      test12();
      test13();
    }
    ProfileAccumulator::print(std::cout);
    return 0;
  }
  int result = -1;
  PerformanceTest application;
  if(application.init(argc, argv))
  {
    result = application.run();
    application.fini();
  }
  return result;
}
