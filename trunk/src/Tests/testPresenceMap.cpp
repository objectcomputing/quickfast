// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <Common/Types.h>
#include <Codecs/PresenceMap.h>
#include <Codecs/DataSourceString.h>
using namespace ::QuickFAST;

BOOST_AUTO_TEST_CASE(testPmap)
{
  Codecs::PresenceMap pmap(1);
  for(size_t n = 0; n < 10; ++n) // more than 7 anyway
  {
    BOOST_CHECK(!pmap.checkNextField());
  }

  uchar oneBytePMAP[] = {'\xFF'};
  pmap.setRaw(oneBytePMAP, 1);

  for(size_t n = 0; n < 7; ++n) // The first 7 should be present
  {
    BOOST_CHECK(pmap.checkNextField());
  }

  for(size_t n = 0; n < 10; ++n) // after 7; MIA
  {
    BOOST_CHECK(!pmap.checkNextField());
  }
  pmap.rewind();
  // Results should be reproducable
  for(size_t n = 0; n < 7; ++n) // The first 7 should be present
  {
    BOOST_CHECK(pmap.checkNextField());
  }

  for(size_t n = 0; n < 10; ++n) // after 7; MIA
  {
    BOOST_CHECK(!pmap.checkNextField());
  }

  uchar everyOtherOne[] = {'\x55', '\xAA'};
  BOOST_CHECK_EQUAL(sizeof(everyOtherOne), 2);
  pmap.setRaw(everyOtherOne, sizeof(everyOtherOne));
  size_t ones = 0;
  for(size_t n = 0; n < 20; ++n)
  {
    if(pmap.checkNextField())
    {
      ++ones;
    }
  }
  BOOST_CHECK_EQUAL(7, ones);

  // This case caused a failure
  std::string testString("\x6f\x62\xa0");
  Codecs::DataSourceString source(testString);
  Codecs::PresenceMap p1(1); // make it grow
  p1.decode(source);
  BOOST_CHECK( p1.checkNextField()); // 6 110
  BOOST_CHECK( p1.checkNextField());
  BOOST_CHECK(!p1.checkNextField());

  BOOST_CHECK( p1.checkNextField()); // F 1111
  BOOST_CHECK( p1.checkNextField());
  BOOST_CHECK( p1.checkNextField());
  BOOST_CHECK( p1.checkNextField());

  BOOST_CHECK( p1.checkNextField()); // 6 110
  BOOST_CHECK( p1.checkNextField());
  BOOST_CHECK(!p1.checkNextField());

  BOOST_CHECK(!p1.checkNextField()); // 2 0010
  BOOST_CHECK(!p1.checkNextField());
  BOOST_CHECK( p1.checkNextField());
  BOOST_CHECK(!p1.checkNextField());

  BOOST_CHECK(!p1.checkNextField()); // A 010
  BOOST_CHECK( p1.checkNextField());
  BOOST_CHECK(!p1.checkNextField());

  BOOST_CHECK(!p1.checkNextField()); // 0
  BOOST_CHECK(!p1.checkNextField());
  BOOST_CHECK(!p1.checkNextField());
  BOOST_CHECK(!p1.checkNextField());

  BOOST_CHECK(!p1.checkNextField()); // and to be sure: check past the end
  BOOST_CHECK(!p1.checkNextField());
  BOOST_CHECK(!p1.checkNextField());
  BOOST_CHECK(!p1.checkNextField());

}
