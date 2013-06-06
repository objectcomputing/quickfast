// Copyright (c) 2009, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/FixedSizeHeaderAnalyzer.h>
#include <Codecs/NoHeaderAnalyzer.h>
#include <Codecs/PacketSequencingAssembler.h>
#include <Messages/FieldIdentity.h>
#include <Messages/SequentialSingleValueBuilder.h>
#include <Communication/RecoveryFeed.h>
#include <Communication/LinkedBuffer.h>
#include <Communication/Receiver.h>
#include <Common/ByteSwapper.h>

using namespace QuickFAST;

namespace
{
  const char template_xml[] =
    "<templates>"
    "  <template name=\"error_recovery\" id=\"1\">"
    "     <uInt32 name=\"internal_sequence_number\" id=\"1\"/>"
    "  </template>"
    "</templates>"
    ;

  struct Packet{
    sequence_t sequenceNumber;
    uchar pmap;
    uchar tid;
    uchar data;
    static const size_t byteCount = sizeof(sequence_t) + sizeof(uchar) + sizeof(uchar) + sizeof(uchar);

    Packet(uchar value)
      : sequenceNumber(value)
      , pmap('\xC0')
      , tid('\x81')
      , data('\x80' | value)
    {
    }
  };
#define PACKET(num) \
  Packet packet##num(num);\
  Communication::LinkedBuffer buffer##num(reinterpret_cast<unsigned char *>(&packet##num), Packet::byteCount, (void *)num);\
  Communication::LinkedBuffer bufferA##num(reinterpret_cast<unsigned char *>(&packet##num), Packet::byteCount, (void *)num);\
  Communication::LinkedBuffer bufferB##num(reinterpret_cast<unsigned char *>(&packet##num), Packet::byteCount, (void *)num)

  PACKET(0);
  PACKET(1);
  PACKET(2);
  PACKET(3);
  PACKET(4);
  PACKET(5);
  PACKET(6);
  PACKET(7);
  PACKET(8);
  PACKET(9);
  PACKET(10);
  PACKET(11);
  PACKET(12);
  PACKET(13);
  PACKET(14);
  PACKET(15);
  PACKET(16);
  PACKET(17);
  PACKET(18);
  PACKET(19);
  PACKET(20);
  PACKET(21);
  PACKET(22);
  PACKET(23);
  PACKET(24);
  PACKET(25);
  PACKET(26);
  PACKET(27);
  PACKET(28);
  PACKET(29);

  Communication::LinkedBuffer * buffers[] =
  {
    & buffer0,& buffer1,& buffer2,& buffer3,& buffer4,& buffer5,& buffer6,& buffer7,& buffer8,& buffer9,
    & buffer10,& buffer11,& buffer12,& buffer13,& buffer14,& buffer15,& buffer16,& buffer17,& buffer18,& buffer19,
    & buffer20,& buffer21,& buffer22,& buffer23,& buffer24,& buffer25,& buffer26,& buffer27,& buffer28,& buffer29,
    0
  };

  Communication::LinkedBuffer * bufferAs[] =
  {
    & bufferA0,& bufferA1,& bufferA2,& bufferA3,& bufferA4,& bufferA5,& bufferA6,& bufferA7,& bufferA8,& bufferA9,
    & bufferA10,& bufferA11,& bufferA12,& bufferA13,& bufferA14,& bufferA15,& bufferA16,& bufferA17,& bufferA18,& bufferA19,
    & bufferA20,& bufferA21,& bufferA22,& bufferA23,& bufferA24,& bufferA25,& bufferA26,& bufferA27,& bufferA28,& bufferA29,
    0
  };

  Communication::LinkedBuffer * bufferBs[] =
  {
    & bufferB0,& bufferB1,& bufferB2,& bufferB3,& bufferB4,& bufferB5,& bufferB6,& bufferB7,& bufferB8,& bufferB9,
    & bufferB10,& bufferB11,& bufferB12,& bufferB13,& bufferB14,& bufferB15,& bufferB16,& bufferB17,& bufferB18,& bufferB19,
    & bufferB20,& bufferB21,& bufferB22,& bufferB23,& bufferB24,& bufferB25,& bufferB26,& bufferB27,& bufferB28,& bufferB29,
    0
  };



  class TestReceiver : public Communication::Receiver
  {
  public:
    ~TestReceiver()
    {
    }

    void acceptBuffer(Communication::LinkedBuffer * buffer)
    {
      queue_.push(buffer);
    }

    virtual void run()
    {
    }

    virtual void run_one()
    {
    }

    virtual size_t poll()
    {
      return 0;
    }

    virtual size_t poll_one()
    {
      return 0;
    }

    virtual void runThreads(size_t threadCount = 0, bool useThisThread = true)
    {
    }

    virtual void joinThreads()
    {
    }

    virtual void resetService()
    {
    }

    virtual bool waitBuffer()
    {
      return true;
    }

    virtual Communication::LinkedBuffer * getBuffer(bool wait)
    {
      return queue_.pop();
    }

    virtual bool initializeReceiver()
    {
      return true;
    }

    virtual bool fillBuffer(
      Communication::LinkedBuffer * buffer,
      boost::mutex::scoped_lock& lock)
    {
      return true;
    }

  public: // because this is a test class
    Communication::BufferQueue queue_;
  };
}

BOOST_AUTO_TEST_CASE(TestPacketSequencingAssemblerReorderAndGapDetect)
{
  std::stringstream templateStream(template_xml);
  Codecs::XMLTemplateParser parser;
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  bool bigEndian = ByteSwapper::isBigEndian();
  Codecs::FixedSizeHeaderAnalyzer packetHeaderAnalyzer(0, bigEndian, 4, 0, 0, 4);
  Codecs::NoHeaderAnalyzer messageHeaderAnalyzer;

  Messages::SequentialSingleValueBuilder<uint32> builder;
  size_t lookAheadCount = 4;
  Communication::RecoveryFeedPtr recoveryFeed; // no recovery feed
  Codecs::PacketSequencingAssembler assembler(
      templateRegistry,
      packetHeaderAnalyzer,
      messageHeaderAnalyzer,
      builder,
      lookAheadCount,
      recoveryFeed);
  TestReceiver receiver;

  //////////////
  // Make sure it does nothing properly
  assembler.serviceQueue(receiver);
  BOOST_CHECK_EQUAL(builder.valueCount(), 0);
  BOOST_CHECK(!builder.hasGap());
  BOOST_CHECK(!builder.hasError());

  ////////////////////
  // Decode one packet
  receiver.acceptBuffer(& buffer0);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 1);
  BOOST_CHECK_EQUAL(builder.value(0), reinterpret_cast<std::ptrdiff_t> (buffer0.extra()));
  BOOST_CHECK(!builder.hasGap());
  BOOST_CHECK(!builder.hasError());

  ////////////////////
  // Decode one packet again
  builder.reset();
  receiver.acceptBuffer(& buffer1);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 1);
  BOOST_CHECK_EQUAL(builder.value(0), reinterpret_cast<std::ptrdiff_t> (buffer1.extra()));
  BOOST_CHECK(!builder.hasGap());
  BOOST_CHECK(!builder.hasError());

  /////////////////////////////////////////
  // Decode look_ahead packets out of order
  builder.reset();
  receiver.acceptBuffer(&buffer5);
  receiver.acceptBuffer(&buffer3);
  receiver.acceptBuffer(&buffer4);
  receiver.acceptBuffer(&buffer2);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 4);
  BOOST_CHECK(builder.value(0) == reinterpret_cast<std::ptrdiff_t> (buffer2.extra()));
  BOOST_CHECK(builder.value(1) == reinterpret_cast<std::ptrdiff_t> (buffer3.extra()));
  BOOST_CHECK(builder.value(2) == reinterpret_cast<std::ptrdiff_t> (buffer4.extra()));
  BOOST_CHECK(builder.value(3) == reinterpret_cast<std::ptrdiff_t> (buffer5.extra()));
  BOOST_CHECK(!builder.hasGap());
  BOOST_CHECK(!builder.hasError());

  /////////////////////////////////////////
  // Decode look_ahead packets out of order
  // with delays
  builder.reset();
  receiver.acceptBuffer(&buffer9);
  receiver.acceptBuffer(&buffer7);
  receiver.acceptBuffer(&buffer8);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 0);
  receiver.acceptBuffer(&buffer6);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 4);
  BOOST_CHECK(builder.value(0) == reinterpret_cast<std::ptrdiff_t> (buffer6.extra()));
  BOOST_CHECK(builder.value(1) == reinterpret_cast<std::ptrdiff_t> (buffer7.extra()));
  BOOST_CHECK(builder.value(2) == reinterpret_cast<std::ptrdiff_t> (buffer8.extra()));
  BOOST_CHECK(builder.value(3) == reinterpret_cast<std::ptrdiff_t> (buffer9.extra()));
  BOOST_CHECK(!builder.hasGap());
  BOOST_CHECK(!builder.hasError());

  //////////////////////////
  // Decode look_ahead and deferred packets
  // no gap detect as long as we're within
  // look_ahead packets of the last one when
  // we're caught up with the input.
  builder.reset();
  receiver.acceptBuffer(&buffer12);
  receiver.acceptBuffer(&buffer11);
  receiver.acceptBuffer(&buffer13);
  // toss a duplicate in there just for fun
  receiver.acceptBuffer(&bufferA13);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 0);
  // these will go in deferred queue
  receiver.acceptBuffer(&buffer16);
  receiver.acceptBuffer(&buffer15);
  receiver.acceptBuffer(&buffer17);
  // Some more duplicates
  receiver.acceptBuffer(&bufferA15);
  receiver.acceptBuffer(&bufferA16);
  receiver.acceptBuffer(&bufferA17);

  // this will trigger decoding of 10-13 and move 15-17 to look_ahead array
  receiver.acceptBuffer(&buffer10);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 4);
  BOOST_CHECK(builder.value(0) == reinterpret_cast<std::ptrdiff_t> (buffer10.extra()));
  BOOST_CHECK(builder.value(1) == reinterpret_cast<std::ptrdiff_t> (buffer11.extra()));
  BOOST_CHECK(builder.value(2) == reinterpret_cast<std::ptrdiff_t> (buffer12.extra()));
  BOOST_CHECK(builder.value(3) == reinterpret_cast<std::ptrdiff_t> (buffer13.extra()));
  // 15, 16, 17 should be in look_ahead array
  receiver.acceptBuffer(&buffer14);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 8);
  BOOST_CHECK(builder.value(4) == reinterpret_cast<std::ptrdiff_t> (buffer14.extra()));
  BOOST_CHECK(builder.value(5) == reinterpret_cast<std::ptrdiff_t> (buffer15.extra()));
  BOOST_CHECK(builder.value(6) == reinterpret_cast<std::ptrdiff_t> (buffer16.extra()));
  BOOST_CHECK(builder.value(7) == reinterpret_cast<std::ptrdiff_t> (buffer17.extra()));
  BOOST_CHECK(!builder.hasGap());
  BOOST_CHECK(!builder.hasError());

  ////////////////////////////
  // Test gap detection
  builder.reset();
  // this should go in the deferred queue
  receiver.acceptBuffer(&buffer22);
  // and this should trigger a gap report
  // after which buffer22 should be processed.
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE(builder.hasGap());
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 1);
  BOOST_REQUIRE(builder.value(0) == reinterpret_cast<std::ptrdiff_t> (buffer22.extra()));
  // already skipped this message, so it should NOT be processed
  receiver.acceptBuffer(&buffer18);
  // but this one should be processed immediately
  receiver.acceptBuffer(&buffer23);
  assembler.serviceQueue(receiver);
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 2);
  BOOST_REQUIRE(builder.value(1) == reinterpret_cast<std::ptrdiff_t> (buffer23.extra()));
  BOOST_CHECK(!builder.hasError());
}

void faultyHeader()
{
  std::stringstream templateStream(template_xml);
  Codecs::XMLTemplateParser parser;
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);
  Codecs::NoHeaderAnalyzer messageHeaderAnalyzer;
  Messages::SequentialSingleValueBuilder<uint32> builder;
  size_t lookAheadCount = 4;
  Communication::RecoveryFeedPtr recoveryFeed; // no recovery feed
  Codecs::PacketSequencingAssembler assembler(
      templateRegistry,
      messageHeaderAnalyzer,
      messageHeaderAnalyzer,
      builder,
      lookAheadCount,
      recoveryFeed);
}

BOOST_AUTO_TEST_CASE(TestPacketSequencingAssemblerDetectConfigError)
{
  BOOST_CHECK_THROW(faultyHeader(), UsageError);
}

class TestRecoveryFeed : public Communication::RecoveryFeed
{
public:
  TestRecoveryFeed()
    : fill_(false)
    , completeFill_(false)
  {
  }
  virtual ~TestRecoveryFeed()
  {
  }

  void setFill(bool fill)
  {
    fill_ = fill;
  }

  void setCompleteFill(bool completeFill)
  {
    completeFill_ = completeFill;
  }

  virtual bool reportGap(sequence_t firstMissing, sequence_t firstPresentAfterGap)
  {
    if(fill_)
    {
      acceptBuffer(bufferBs[firstMissing++]);
      while(completeFill_ && firstMissing < firstPresentAfterGap)
      {
        acceptBuffer(bufferBs[firstMissing++]);
      }
    }
    return fill_;
  }

  virtual bool stillWaiting(sequence_t firstMissing, sequence_t firstPresentAfterGap)
  {
    if(fill_)
    {
      acceptBuffer(bufferBs[firstMissing++]);
      while(completeFill_ && firstMissing < firstPresentAfterGap)
      {
        acceptBuffer(bufferBs[firstMissing++]);
      }
    }
    return fill_;
  }

private:
  TestRecoveryFeed(const TestRecoveryFeed &);
  TestRecoveryFeed & operator = (const TestRecoveryFeed &);
private:
  bool fill_;
  bool completeFill_;

};

BOOST_AUTO_TEST_CASE(TestPacketSequencingAssemblerGapFill)
{
  std::stringstream templateStream(template_xml);
  Codecs::XMLTemplateParser parser;
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  bool bigEndian = ByteSwapper::isBigEndian();
  Codecs::FixedSizeHeaderAnalyzer packetHeaderAnalyzer(0, bigEndian, 4, 0, 0, 4);
  Codecs::NoHeaderAnalyzer messageHeaderAnalyzer;

  Messages::SequentialSingleValueBuilder<uint32> builder;
  size_t lookAheadCount = 4;
  TestRecoveryFeed * testRecoveryFeed = new TestRecoveryFeed();
  Communication::RecoveryFeedPtr recoveryFeed(testRecoveryFeed);
  Codecs::PacketSequencingAssembler assembler(
      templateRegistry,
      packetHeaderAnalyzer,
      messageHeaderAnalyzer,
      builder,
      lookAheadCount,
      recoveryFeed);
  TestReceiver receiver;

  /////////////////////////////////////////////////////////////
  // Test with fill turned off:
  //
  testRecoveryFeed->setFill(false);
  // Decode one packet to set the next expected sequence number
  receiver.acceptBuffer(& buffer0);
  // and one to be a "gap"
  receiver.acceptBuffer(& buffer5);
  assembler.serviceQueue(receiver);
  BOOST_CHECK(builder.hasGap());
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 2);
  BOOST_CHECK_EQUAL(builder.value(0), reinterpret_cast<std::ptrdiff_t> (buffer0.extra()));
  BOOST_CHECK_EQUAL(builder.value(1), reinterpret_cast<std::ptrdiff_t> (buffer5.extra()));
  BOOST_CHECK(!builder.hasError());

  ////////////////////////////
  // Test with fill on; but complete fill off
  testRecoveryFeed->setFill(true);
  testRecoveryFeed->setCompleteFill(false);
  builder.reset();

  // missing buffer 6
  // missing buffer 7
  receiver.acceptBuffer(&buffer8);
  receiver.acceptBuffer(&buffer9);
  // this one will be deferred
  receiver.acceptBuffer(&buffer10);
  // this should trigger gap detection and recovery
  assembler.serviceQueue(receiver);
  // the builder should never see the gap
  BOOST_CHECK(!builder.hasError());
  BOOST_CHECK(!builder.hasGap());
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 5);
  BOOST_CHECK_EQUAL(builder.value(0), reinterpret_cast<std::ptrdiff_t> (buffer6.extra()));
  BOOST_CHECK_EQUAL(builder.value(1), reinterpret_cast<std::ptrdiff_t> (buffer7.extra()));
  BOOST_CHECK_EQUAL(builder.value(2), reinterpret_cast<std::ptrdiff_t> (buffer8.extra()));
  BOOST_CHECK_EQUAL(builder.value(3), reinterpret_cast<std::ptrdiff_t> (buffer9.extra()));
  BOOST_CHECK_EQUAL(builder.value(4), reinterpret_cast<std::ptrdiff_t> (buffer10.extra()));

  ////////////////////////////
  // Test with fill and complete fill on
  testRecoveryFeed->setFill(true);
  testRecoveryFeed->setCompleteFill(true);
  builder.reset();

  // missing buffer 11
  // missing buffer 12
  receiver.acceptBuffer(&buffer13);
  receiver.acceptBuffer(&buffer14);
  // this one will be deferred
  receiver.acceptBuffer(&buffer15);
  // this should trigger gap detection and recovery
  assembler.serviceQueue(receiver);
  // the builder should never see the gap
  BOOST_CHECK(!builder.hasError());
  BOOST_CHECK(!builder.hasGap());
  BOOST_REQUIRE_EQUAL(builder.valueCount(), 5);
  BOOST_CHECK_EQUAL(builder.value(0), reinterpret_cast<std::ptrdiff_t> (buffer11.extra()));
  BOOST_CHECK_EQUAL(builder.value(1), reinterpret_cast<std::ptrdiff_t> (buffer12.extra()));
  BOOST_CHECK_EQUAL(builder.value(2), reinterpret_cast<std::ptrdiff_t> (buffer13.extra()));
  BOOST_CHECK_EQUAL(builder.value(3), reinterpret_cast<std::ptrdiff_t> (buffer14.extra()));
  BOOST_CHECK_EQUAL(builder.value(4), reinterpret_cast<std::ptrdiff_t> (buffer15.extra()));
}
