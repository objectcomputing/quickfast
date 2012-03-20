// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <Communication/SingleServerBufferQueue.h>
#include <Common/StringBuffer.h>
#include <Common/WorkingBuffer.h>
#include <Common/Exceptions.h>
#include <Common/Decimal.h>

using namespace QuickFAST;
BOOST_AUTO_TEST_CASE(TestLinkedBuffer)
{
  // construct a root buffer
  Communication::LinkedBuffer root;
  BOOST_CHECK(root.capacity() == 0);
  BOOST_CHECK(root.used() == 0);
  BOOST_CHECK(root.link() == 0);
  BOOST_CHECK(root.get() == 0);

  Communication::LinkedBuffer buffer1(100);
  BOOST_CHECK(buffer1.capacity() == 100);
  BOOST_CHECK(buffer1.used() == 0);
  BOOST_CHECK(buffer1.link() == 0);
  BOOST_CHECK(buffer1.get() != 0);

  root.link(&buffer1);
  BOOST_CHECK(root.link() == &buffer1);
  BOOST_CHECK(buffer1.link() == 0);

  for(size_t i = 0; i < buffer1.capacity(); ++i)
  {
    buffer1[i] = 1;
  }
  buffer1.setUsed(20);
  BOOST_CHECK(buffer1.used() == 20);

  Communication::LinkedBuffer external;
  unsigned char * hello = (unsigned char *)"hello";
  external.setExternal(hello, sizeof(hello));
  BOOST_CHECK_EQUAL(0, std::strcmp((char *)hello, (char *)external.get()));
  // note part of the test is being sure that hello does not get deleted[] when external goes out of scope.
  // we count on the debug memory allocater to detect that case.
}

BOOST_AUTO_TEST_CASE(TestBufferCollection)
{
  Communication::BufferCollection collection;
  BOOST_CHECK(collection.pop() == 0);

  Communication::LinkedBuffer buffer1(20);
  buffer1[0] = 1;
  buffer1[1] = 1;
  buffer1.setUsed(2);

  Communication::LinkedBuffer buffer2(20);
  buffer2[0] = 2;
  buffer2[1] = 1;
  buffer2.setUsed(2);

  Communication::LinkedBuffer buffer3(20);
  buffer2[0] = 3;
  buffer3[1] = 1;
  buffer2.setUsed(2);

  Communication::LinkedBuffer buffer4(20);
  buffer4[0] = 4;
  buffer4[1] = 1;
  buffer4.setUsed(2);

  collection.push(&buffer1);

  BOOST_CHECK(collection.pop() == &buffer1);
  BOOST_CHECK(collection.pop() == 0);

  collection.push(&buffer1);
  collection.push(&buffer2);
  collection.push(&buffer3);
  collection.push(&buffer4);

  // notice we don't care what order we get them back
  // but we do want to see all of them exactly once each.
  Communication::LinkedBuffer * p = collection.pop();
  BOOST_CHECK((*p)[1] == 1);
  (*p)[1] = 2;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 1);
  (*p)[1] = 2;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 1);
  (*p)[1] = 2;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 1);
  (*p)[1] = 2;

  p = collection.pop();
  BOOST_CHECK(p == 0);

  BOOST_CHECK(buffer1[1] == 2);
  BOOST_CHECK(buffer2[1] == 2);
  BOOST_CHECK(buffer3[1] == 2);
  BOOST_CHECK(buffer4[1] == 2);

  Communication::BufferCollection collection2;
  collection2.push(&buffer4);
  collection2.push(&buffer3);
  collection2.push(&buffer2);
  collection2.push(&buffer1);
  collection.push(collection2);
  BOOST_CHECK(collection2.pop() == 0);

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 2);
  (*p)[1] = 3;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 2);
  (*p)[1] = 3;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 2);
  (*p)[1] = 3;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 2);
  (*p)[1] = 3;

  p = collection.pop();
  BOOST_CHECK(p == 0);

  BOOST_CHECK(buffer1[1] == 3);
  BOOST_CHECK(buffer2[1] == 3);
  BOOST_CHECK(buffer3[1] == 3);
  BOOST_CHECK(buffer4[1] == 3);

  collection.push(&buffer1);
  collection.push(&buffer2);
  collection2.push(&buffer3);
  collection2.push(&buffer4);
  collection.push(collection2);
  collection2.push(collection);
  collection.push(collection2);

  BOOST_CHECK(collection2.pop() == 0);

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 3);
  (*p)[1] = 4;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 3);
  (*p)[1] = 4;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 3);
  (*p)[1] = 4;

  p = collection.pop();
  BOOST_CHECK((*p)[1] == 3);
  (*p)[1] = 4;

  p = collection.pop();
  BOOST_CHECK(p == 0);

  BOOST_CHECK(buffer1[1] == 4);
  BOOST_CHECK(buffer2[1] == 4);
  BOOST_CHECK(buffer3[1] == 4);
  BOOST_CHECK(buffer4[1] == 4);
}

BOOST_AUTO_TEST_CASE(TestBufferQueue)
{
  Communication::BufferQueue queue1;
  Communication::BufferQueue queue2;
  Communication::LinkedBuffer buffer1(20);
  buffer1[0] = 1;
  buffer1[1] = 1;
  buffer1.setUsed(2);

  Communication::LinkedBuffer buffer2(20);
  buffer2[0] = 2;
  buffer2[1] = 1;
  buffer2.setUsed(2);

  Communication::LinkedBuffer buffer3(20);
  buffer2[0] = 3;
  buffer3[1] = 1;
  buffer2.setUsed(2);

  Communication::LinkedBuffer buffer4(20);
  buffer4[0] = 4;
  buffer4[1] = 1;
  buffer4.setUsed(2);

  // keep the first test simple
  // four buffers in, four buffers out
  // and make sure it works at least twice
  for(size_t loop = 0; loop < 2; ++loop)
  {
    BOOST_CHECK( queue1.push(&buffer1));
    BOOST_CHECK(!queue1.push(&buffer2));
    BOOST_CHECK( queue1.pop() == &buffer1);
    BOOST_CHECK( queue1.pop() == &buffer2);
    BOOST_CHECK( queue1.pop() == 0);
  }

  // simple test of pushing a queue
  for(size_t loop = 0; loop < 2; ++loop)
  {
    BOOST_CHECK( queue1.push(&buffer1));
    BOOST_CHECK(!queue1.push(&buffer2));
    BOOST_CHECK( queue2.push(queue1));
    BOOST_CHECK( queue1.pop() == 0);
    BOOST_CHECK( queue2.pop() == &buffer1);
    BOOST_CHECK( queue2.pop() == &buffer2);
    BOOST_CHECK( queue2.pop() == 0);
  }

  // test mixed arrivals and departures
  for(size_t loop = 0; loop < 2; ++loop)
  {
    BOOST_CHECK( queue1.push(&buffer1));
    BOOST_CHECK(!queue1.push(&buffer2));
    BOOST_CHECK(!queue1.push(&buffer3));
    BOOST_CHECK( queue1.pop() == &buffer1);
    BOOST_CHECK(!queue1.push(&buffer4));
    BOOST_CHECK( queue1.pop() == &buffer2);
    BOOST_CHECK(!queue1.push(&buffer1));
    BOOST_CHECK( queue1.pop() == &buffer3);
    BOOST_CHECK( queue1.pop() == &buffer4);
    BOOST_CHECK( queue1.pop() == &buffer1);
    BOOST_CHECK( queue1.pop() == 0);
  }

  // test mixed arrivals and departures with 2 queues
  for(size_t loop = 0; loop < 2; ++loop)
  {
    BOOST_CHECK( queue1.push(&buffer1));
    BOOST_CHECK(!queue1.push(&buffer2));
    BOOST_CHECK( queue2.push(queue1));
    BOOST_CHECK( queue1.push(&buffer3));
    BOOST_CHECK( queue2.pop() == &buffer1);
    BOOST_CHECK(!queue2.push(queue1));
    BOOST_CHECK(!queue2.push(queue1));
    BOOST_CHECK( queue1.push(&buffer4));
    BOOST_CHECK( queue2.pop() == &buffer2);
    BOOST_CHECK(!queue1.push(&buffer1));
    BOOST_CHECK( queue2.pop() == &buffer3);
    BOOST_CHECK( queue2.pop() == 0);
    BOOST_CHECK( queue2.push(queue1));
    BOOST_CHECK( queue2.pop() == &buffer4);
    BOOST_CHECK( queue2.pop() == &buffer1);
    BOOST_CHECK( queue2.pop() == 0);
    BOOST_CHECK( queue1.pop() == 0);
    BOOST_CHECK(!queue2.push(queue1));
    BOOST_CHECK( queue2.pop() == 0);
    BOOST_CHECK( queue1.pop() == 0);
  }

}

BOOST_AUTO_TEST_CASE(TestSingleServerBufferQueue)
{
  boost::mutex dummyMutex;
  boost::mutex::scoped_lock lock(dummyMutex);

  Communication::SingleServerBufferQueue queue1;

  Communication::LinkedBuffer buffer1(20);
  buffer1[0] = 1;
  buffer1[1] = 1;
  buffer1.setUsed(2);

  Communication::LinkedBuffer buffer2(20);
  buffer2[0] = 2;
  buffer2[1] = 1;
  buffer2.setUsed(2);

  Communication::LinkedBuffer buffer3(20);
  buffer2[0] = 3;
  buffer3[1] = 1;
  buffer2.setUsed(2);

  Communication::LinkedBuffer buffer4(20);
  buffer4[0] = 4;
  buffer4[1] = 1;
  buffer4.setUsed(2);

  // keep the first test simple
  // four buffers in, four buffers out
  // and make sure it works at least twice
  for(size_t loop = 0; loop < 2; ++loop)
  {
    BOOST_CHECK( queue1.push(&buffer1, lock));
    BOOST_CHECK( queue1.push(&buffer2, lock));
    BOOST_CHECK( queue1.startService(lock));
    BOOST_CHECK( queue1.serviceNext() == &buffer1);
    BOOST_CHECK( queue1.serviceNext() == &buffer2);
    BOOST_CHECK( queue1.serviceNext() == 0);
    BOOST_CHECK(!queue1.endService(true, lock));
  }

  // Next test gets more elaborate to simulate
  // buffers arriving while the queue is being serviced
  for(size_t loop = 0; loop < 2; ++loop)
  {
    BOOST_CHECK( queue1.push(&buffer1, lock));
    BOOST_CHECK( queue1.push(&buffer2, lock));
    BOOST_CHECK( queue1.startService(lock));
    // be sure only one service at a time
    BOOST_CHECK(!queue1.startService(lock));
    // push and return "service not necessary"
    BOOST_CHECK(!queue1.push(&buffer3, lock));
    BOOST_CHECK( queue1.serviceNext() == &buffer1);
    BOOST_CHECK(!queue1.startService(lock));
    BOOST_CHECK(!queue1.push(&buffer4, lock));
    BOOST_CHECK( queue1.serviceNext() == &buffer2);
    BOOST_CHECK(!queue1.push(&buffer1, lock));
    // batch should be complete even though new messages are queued
    BOOST_CHECK( queue1.serviceNext() == 0);
    // so when we endService it should return true->there's more to do
    BOOST_CHECK( queue1.endService(true, lock));
    BOOST_CHECK( queue1.serviceNext() == &buffer3);
    BOOST_CHECK( queue1.serviceNext() == &buffer4);
    BOOST_CHECK( queue1.serviceNext() == &buffer1);
    BOOST_CHECK( queue1.serviceNext() == 0);
    BOOST_CHECK(!queue1.endService(true, lock));
  }
}

BOOST_AUTO_TEST_CASE(TestStringBuffer)
{
  typedef StringBufferT<10> String10;
  const char * st1("12345");
  const char * st2("6789");

  const unsigned char * ut1(reinterpret_cast<const unsigned char *>("Hello"));
  const unsigned char * ut2(reinterpret_cast<const unsigned char *>("World"));

  String10 s1(st1);
  s1 += st2;
  BOOST_CHECK(s1 == "123456789");
  BOOST_CHECK(s1.growCount() == 0);
  s1 += 'A';
  BOOST_CHECK(s1 == "123456789A");
  BOOST_CHECK(s1.growCount() == 0);
  unsigned char b('B');
  s1 += b;
  BOOST_CHECK(s1 == "123456789AB");
  BOOST_CHECK(s1.growCount() == 1);

  String10 s2(s1);
  BOOST_CHECK(s2 == "123456789AB");
  BOOST_CHECK(s2.growCount() == 1);

  std::string stds2(s2);
  BOOST_CHECK(stds2 == "123456789AB");
  size_t capacity = s2.capacity();
  size_t size = s2.size();
  BOOST_CHECK(size <= capacity);
  size_t delta = capacity - size;
  String10 s3(delta, '!');
  BOOST_CHECK(s3.size() == delta);
  s2 += s3;
  BOOST_CHECK(s2.size() == capacity);
  BOOST_CHECK(s2.growCount() == 1);
  s2 += "?";
  BOOST_CHECK(s2.capacity() > capacity);
  BOOST_CHECK(s2.growCount() == 2);
}

BOOST_AUTO_TEST_CASE(TestWorkingBuffer)
{
  WorkingBuffer a;
  a.clear(false, 1);
  a.push('a');
  WorkingBuffer b;
  b.clear(false, 1);
  b.push('b');
  WorkingBuffer ab;
  ab.clear(false, a.size() + b.size());
  size_t abCapacity = ab.capacity();
  ab.append(a);
  ab.append(b);
  BOOST_CHECK(ab.size() ==  a.size() + b.size());
  BOOST_CHECK(ab.capacity() == abCapacity); // should not have forced a grow
  BOOST_CHECK(0 == std::strncmp("ab", reinterpret_cast<const char *>(ab.begin()), ab.size()));

  WorkingBuffer ba;
  ba.clear(true, a.size() + b.size());
  size_t baCapacity = ab.capacity();
  ba.append(a);
  ba.append(b);
  BOOST_CHECK(ba.size() ==  a.size() + b.size());
  BOOST_CHECK(ba.capacity() == baCapacity); // should not have forced a grow
  BOOST_CHECK(0 == std::strncmp("ba", reinterpret_cast<const char *>(ba.begin()), ba.size()));

  WorkingBuffer abc;
  size_t abcCap = abc.capacity();
  size_t abcHalf = abcCap / 2;
  std::string abcStr;
  abcStr.reserve(abcCap * 2);
  for(size_t n = 0; n < abcHalf; ++n)
  {
    abc.push(uchar('a' + n));
    abcStr += uchar('a' + n);
  }
  BOOST_CHECK_EQUAL(abc.size(), abcHalf);
  BOOST_CHECK_EQUAL(abc.capacity(), abcCap); // no grow so far
  BOOST_CHECK(0 == std::strncmp(abcStr.data(), reinterpret_cast<const char *>(abc.begin()), abcHalf));
  abc.append(abc); // also tests append-to-self
  abcStr += abcStr;
  BOOST_CHECK(abc.size() == abcHalf * 2);
  BOOST_CHECK(abc.capacity() == abcCap); // no grow so far
  BOOST_CHECK(0 == std::strncmp(abcStr.data(), reinterpret_cast<const char *>(abc.begin()), abcHalf * 2));

  abc.append(abc); // This one should force a grow
  abcStr += abcStr;
  BOOST_CHECK(abc.size() == abcHalf * 4);
  BOOST_CHECK(abc.capacity() > abcCap); // now we should have grown
  BOOST_CHECK(0 == std::strncmp(abcStr.data(), reinterpret_cast<const char *>(abc.begin()), abcHalf * 4));


  // TODO: This is a start, but we could use a lot more testing here.
}

BOOST_AUTO_TEST_CASE(TestDecimal)
{
  // Test basic decimal arithmetic

  Decimal zero(0,0);
  Decimal one(1,0);
  Decimal ten(10,0);
  Decimal hundred(1,2);

  Decimal a(zero);
  a += one;
  BOOST_CHECK_EQUAL(a, one);
  a += one;
  a += one;
  a += one;
  a += one;
  a += one;
  a += one;
  a += one;
  a += one;
  a += one;
  BOOST_CHECK_EQUAL(a, ten);
  a /= ten;
  a -= one;
  BOOST_CHECK_EQUAL(a, zero);
  a = hundred;
  a -= ten;
  a -= ten;
  a -= ten;
  a -= ten;
  a -= ten;
  a -= ten;
  a -= ten;
  a -= ten;
  a -= ten;
  a -= ten;
  BOOST_CHECK_EQUAL(a, zero);

  a = one;
  a /= ten;
  a /= ten;
  a /= ten;
  a /= ten;
  a /= ten;
  a /= ten;
  a /= ten;
  a /= ten;
  a /= ten;
  a /= ten;
  a *= hundred;
  a *= hundred;
  a *= hundred;
  a *= hundred;
  a *= hundred;
  BOOST_CHECK_EQUAL(a, one);

  Decimal b(0,0);
  Decimal c(123,0);
  Decimal d(456,0);
  b += c;
  b += d;
  b -= d;
  BOOST_CHECK_EQUAL(b,c);

  Decimal f(2095, -2);
  Decimal g(2000, -2);
  BOOST_CHECK_GT(f, g);
  f.normalize();
  g.normalize();
  BOOST_CHECK_GT(f, g);

}
