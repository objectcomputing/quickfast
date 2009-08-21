// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <Common/LinkedBuffer.h>
#include <Common/Exceptions.h>

using namespace QuickFAST;
BOOST_AUTO_TEST_CASE(TestLinkedBuffer)
{
  // construct a root buffer
  LinkedBuffer root;
  BOOST_CHECK(root.capacity() == 0);
  BOOST_CHECK(root.used() == 0);
  BOOST_CHECK(root.link() == 0);
  BOOST_CHECK(root.get() == 0);

  LinkedBuffer buffer1(100);
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
}

BOOST_AUTO_TEST_CASE(TestSimpleBufferCollection)
{
  SimpleBufferCollection collection;
  BOOST_CHECK(collection.pop() == 0);

  LinkedBuffer buffer1(20);
  buffer1[0] = 1;
  buffer1[1] = 1;
  buffer1.setUsed(2);

  LinkedBuffer buffer2(20);
  buffer2[0] = 2;
  buffer2[1] = 1;
  buffer2.setUsed(2);

  LinkedBuffer buffer3(20);
  buffer2[0] = 3;
  buffer3[1] = 1;
  buffer2.setUsed(2);

  LinkedBuffer buffer4(20);
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
  LinkedBuffer * p = collection.pop();
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

  SimpleBufferCollection collection2;
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

BOOST_AUTO_TEST_CASE(TestSingleServerBufferQueue)
{
  boost::mutex dummyMutex;
  boost::mutex::scoped_lock lock(dummyMutex);

  SingleServerBufferQueue queue;

  LinkedBuffer buffer1(20);
  buffer1[0] = 1;
  buffer1[1] = 1;
  buffer1.setUsed(2);

  LinkedBuffer buffer2(20);
  buffer2[0] = 2;
  buffer2[1] = 1;
  buffer2.setUsed(2);

  LinkedBuffer buffer3(20);
  buffer2[0] = 3;
  buffer3[1] = 1;
  buffer2.setUsed(2);

  LinkedBuffer buffer4(20);
  buffer4[0] = 4;
  buffer4[1] = 1;
  buffer4.setUsed(2);

  // keep the first test simple
  // four buffers in, four buffers out
  // and make sure it works at least twice
  for(size_t loop = 0; loop < 2; ++loop)
  {
    BOOST_CHECK( queue.push(&buffer1, lock));
    BOOST_CHECK( queue.push(&buffer2, lock));
    BOOST_CHECK( queue.startService(lock));
    BOOST_CHECK( queue.serviceNext() == &buffer1);
    BOOST_CHECK( queue.serviceNext() == &buffer2);
    BOOST_CHECK( queue.serviceNext() == 0);
    BOOST_CHECK(!queue.endService(true, lock));
  }

  // Next test gets more elaborate to simulate
  // buffers arriving while the queue is being serviced
  for(size_t loop = 0; loop < 2; ++loop)
  {
    BOOST_CHECK( queue.push(&buffer1, lock));
    BOOST_CHECK( queue.push(&buffer2, lock));
    BOOST_CHECK( queue.startService(lock));
    // be sure only one service at a time
    BOOST_CHECK(!queue.startService(lock));
    // push and return "service not necessary"
    BOOST_CHECK(!queue.push(&buffer3, lock));
    BOOST_CHECK( queue.serviceNext() == &buffer1);
    BOOST_CHECK(!queue.startService(lock));
    BOOST_CHECK(!queue.push(&buffer4, lock));
    BOOST_CHECK( queue.serviceNext() == &buffer2);
    BOOST_CHECK(!queue.push(&buffer1, lock));
    // batch should be complete even though new messages are queued
    BOOST_CHECK( queue.serviceNext() == 0);
    // so when we endService it should return true->there's more to do
    BOOST_CHECK( queue.endService(true, lock));
    BOOST_CHECK( queue.serviceNext() == &buffer3);
    BOOST_CHECK( queue.serviceNext() == &buffer4);
    BOOST_CHECK( queue.serviceNext() == &buffer1);
    BOOST_CHECK( queue.serviceNext() == 0);
    BOOST_CHECK(!queue.endService(true, lock));
  }
}
