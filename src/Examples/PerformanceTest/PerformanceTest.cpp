// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "PerformanceTest.h"
#include <Codecs/DataSourceStream.h>
#include <Codecs/DataSourceBufferedStream.h>
#include <Codecs/SynchronousDecoder.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/GenericMessageBuilder.h>

#include <Examples/MessagePerformance.h>
#include <PerformanceTest/NullMessage.h>

#include <Examples/StopWatch.h>
#include <Common/Profiler.h>

using namespace QuickFAST;
using namespace Examples;

PerformanceTest::PerformanceTest()
  : resetOnMessage_(false)
  , strict_(true)
  , useNullMessage_(false)
  , performanceFile_(0)
  , profileFile_(0)
  , head_(0)
  , count_(1)
  , interpret_(false)
  , headerBytes_(0)
  , echo_(false)
{
}

PerformanceTest::~PerformanceTest()
{
}

bool
PerformanceTest::init(int argc, char* argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
PerformanceTest::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  std::string opt(argv[0]);
  try
  {
    if(opt == "-r")
    {
      resetOnMessage_ = !resetOnMessage_;
      consumed = 1;
    }
    else if(opt == "-s")
    {
      strict_ = !strict_;
      consumed = 1;
    }
    else if(opt == "-t" && argc > 1)
    {
      templateFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-f" && argc > 1)
    {
      fastFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-profiler" && argc > 1)
    {
      profileFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-p" && argc > 1)
    {
      performanceFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-i" && argc > 1)
    {
      interpret_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-null")
    {
      useNullMessage_ = true;
      consumed = 1;
    }
    else if(opt == "-head" && argc > 1)
    {
      head_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-c" && argc > 1)
    {
      count_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-hfix" && argc > 1)
    {
      headerBytes_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-e")
    {
      echo_ = true;
      consumed = 1;
    }
  }
  catch (std::exception & ex)
  {
    std::cerr << ex.what() << " while interpreting " << opt << std::endl;
    consumed = 0;
  }
  return consumed;
}

void
PerformanceTest::usage(std::ostream & out) const
{
  out << "  -t file     : Template file (required)" << std::endl;
  out << "  -f file     : FAST Message file (required)" << std::endl;
  out << "  -p file     : File to which performance measurements are written. (default standard output)" << std::endl;
  out << "  -profiler file : File to which profiler statistics are written (very optional)" << std::endl;
  out << "  -head n     : process only the first 'n' messages" << std::endl;
  out << "  -c count    : repeat the test 'count' times" << std::endl;
  out << "  -i count    : retrieve (interprete) field values count times." << std::endl;
  out << "  -r          : Toggle 'reset decoder on every message' (default false)." << std::endl;
  out << "  -null       : Use null message to receive fields." << std::endl;
  out << "  -s          : Toggle 'strict decoding rules' (default true)." << std::endl;
  out << "  -hfix n     : Skip n byte header before each message" << std::endl;
  out << std::endl;
  out << " THE FOLLOWING INVALIDATES THE PERFORMANCE TEST NUMBERS, OF COURSE." << std::endl;
  out << "  -e          : Echo input to standard out in hex; include message and field boundaries (for debugging)" << std::endl;
}

bool
PerformanceTest::applyArgs()
{
  bool ok = true;
  try
  {
    if(templateFileName_.empty())
    {
      ok = false;
      std::cerr << "ERROR: -t [templatefile] option is required." << std::endl;
    }
    if(ok)
    {
      templateFile_.open(templateFileName_.c_str(), std::ios::in
#ifdef _WIN32
        | std::ios::binary
#endif
        );

      if(!templateFile_.good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open template file: "
          << templateFileName_
          << std::endl;
      }
    }
    if(fastFileName_.empty())
    {
      ok = false;
      std::cerr << "ERROR: -f [FASTfile] option is required." << std::endl;
    }
    if(ok)
    {
      fastFile_.open(fastFileName_.c_str(), std::ios::in
#ifdef _WIN32
        | std::ios::binary
#endif // _WIN32
        );
      if(!fastFile_.good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open FAST Message file: "
          << fastFileName_
          << std::endl;
      }
    }
    if(ok && !performanceFileName_.empty())
    {
      performanceFile_ = new std::ofstream(performanceFileName_.c_str());
      if(!performanceFile_->good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open performance output file: "
          << performanceFileName_
          << std::endl;
      }
    }
    else
    {
      performanceFile_ = & std::cout;
    }

    if(ok && !profileFileName_.empty())
    {
      profileFile_ = new std::ofstream(profileFileName_.c_str());
      if(!profileFile_->good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open Profiler output file: "
          << profileFile_
          << std::endl;
      }
    }
  }
  catch (std::exception & ex)
  {
    std::cerr << ex.what() << std::endl;
    ok = false;
  }

  if(!ok)
  {
    commandArgParser_.usage(std::cerr);
  }
  return ok;
}

int
PerformanceTest::run()
{
  try
  {
    std::cout << "Parsing templates" << std::endl;
    Codecs::XMLTemplateParser parser;
    StopWatch parseTimer;
    Codecs::TemplateRegistryPtr templateRegistry = parser.parse(templateFile_);
    unsigned long parseLapse = parseTimer.freeze();
    size_t templateCount = templateRegistry->size();
    (*performanceFile_) << "Parsed "
      << templateCount
      << " templates in "
      << std::fixed << std::setprecision(3)
      << parseLapse
      << " milliseconds. [";
    (*performanceFile_) << std::fixed << std::setprecision(3)
      << double(parseLapse)/double(templateCount) << " msec/template. = "
      << std::fixed << std::setprecision(0)
      << 1000. * double(templateCount)/double(parseLapse) << " template/second.]"
      << std::endl;
    for(size_t nPass = 0; nPass < count_; ++nPass)
    {
      if(count_ > 1)
      {
        std::cout << "Decoding input; pass " << nPass + 1 << " of " << count_ << std::endl;
      }
      fastFile_.seekg(0, std::ios::beg);
      Codecs::DataSourceBufferedStream source(fastFile_);
      if(echo_)
      {
        source.setEcho(std::cout, Codecs::DataSource::HEX, true, true);
      }

      PerformanceBuilder builder;
      Codecs::SynchronousDecoder decoder(templateRegistry);
      decoder.setResetOnMessage(resetOnMessage_);
      decoder.setStrict(strict_);
      decoder.setLimit(head_);
      decoder.setHeaderBytes(headerBytes_);
      StopWatch decodeTimer;
      {
        PROFILE_POINT("Main");
        decoder.decode(source, builder);
      }//PROFILE_POINT
      unsigned long decodeLapse = decodeTimer.freeze();
      size_t messageCount = builder.msgCount();//handler.getMessageCount();
//      size_t groupCount = builder.groupCount();
      size_t fieldCount = builder.fieldCount();
//      size_t sequenceCount = builder.sequenceCount();
      size_t sequenceEntryCount = builder.sequenceEntryCount();
      (*performanceFile_)
#ifdef _DEBUG
        << "[debug] "
#endif // _DEBUG
        << "Decoded " << messageCount << " messages in "
        << std::fixed << std::setprecision(3)
        << decodeLapse
        << " milliseconds. [";
      if(decodeLapse != 0)
      {
        (*performanceFile_) << std::fixed << std::setprecision(3)
          << 1000 * double(decodeLapse)/double(messageCount) << " usec/message. = "
          << std::fixed << std::setprecision(3)
          << 1000. * double(messageCount)/double(decodeLapse) << " messages/second]"
          << std::endl;

        if(fieldCount != 0)
        {
          (*performanceFile_)
            << "      Fields: " << fieldCount << " -> "
            << std::fixed << std::setprecision(3)
            << 1000. * double(decodeLapse)/double(fieldCount) << " usec/field. = "
            << std::fixed << std::setprecision(3)
            << 1000. * double(fieldCount)/double(decodeLapse) << " fields/second]"
            << std::endl;
        }
        if(sequenceEntryCount != 0)
        {
          (*performanceFile_)
            << "      Sequence Entries: " << sequenceEntryCount << " -> "
            << std::fixed << std::setprecision(3)
            << 1000. * double(decodeLapse)/double(sequenceEntryCount) << " usec/entry. = "
            << std::fixed << std::setprecision(3)
            << 1000. * double(sequenceEntryCount)/double(decodeLapse) << " entries/second]"
            << std::endl;
        }
      }
    }
  }
  catch (std::exception & e)
  {
    std::cerr << e.what() << std::endl;
  }
  if(profileFile_ != 0)
  {
    ProfileAccumulator::write(*profileFile_);
  }
  return 0;
}

void
PerformanceTest::fini()
{
}
