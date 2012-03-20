// Copyright (c) 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#ifdef _WIN32 // Asynchronous file writer only works on Win32 so disable this entire test on other platforms

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <Communication/BufferRecycler.h>
#include <Communication/AsynchFileSender.h>
#include <Communication/LinkedBuffer.h>
#include <Common/Exceptions.h>

#include <string.h>

using namespace QuickFAST;
namespace
{
  class AsynchronousFileCopier : public Communication::BufferRecycler
  {
    public:
#pragma warning(push) // we're on windows so don't worry about #ifdeffing around the pragmas
#pragma warning(disable:4355)//warning C4355: 'this' : used in base member initializer list

      AsynchronousFileCopier(
        const std::string & inputFilename,
        size_t bufferCount,
        size_t bufferSize,
        const std::string & outputFile)
        : sender_(*this, outputFile.c_str())
        , inputFileName_(inputFilename)
        , bufferCount_(bufferCount)
        , bufferSize_(bufferSize)
        , in_(0)
      {
      }
#pragma warning(pop)

      ~AsynchronousFileCopier()
      {
        if(in_ != 0)
        {
          std::fclose(in_);
          in_ = 0;
        }
      }

      void close()
      {
        if(in_ != 0)
        {
          std::fclose(in_);
          in_ = 0;
        }
      }

      void go(size_t threadCount = 2)
      {
        // binary because we're hoping for an exact copy
        in_ = fopen(inputFileName_.c_str(), "rb");
        if(in_ == 0)
        {
          throw CommunicationError(std::string("Can't open input file:") + inputFileName_);
        }
        sender_.open();
        for(size_t i = 0; !std::feof(in_) && i < bufferCount_; ++i)
        {
          Communication::LinkedBuffer * buffer = new Communication::LinkedBuffer(bufferSize_);
          startWrite(buffer);
        }
        sender_.runThreads(threadCount, true);
        sender_.close();
      }

      void startWrite(Communication::LinkedBuffer * buffer)
      {
        if(buffer == 0)
        {
          return;
        }
        // warning fails for files sizes larger than a long
        long offset = 0;
        {
          boost::mutex::scoped_lock lock(fileMutex_);

          if(std::feof(in_))
          {
            delete buffer;
            sender_.stop();
            return;
          }
          offset = std::ftell(in_);
          size_t bytesRead = std::fread(buffer->get(), 1, buffer->capacity(), in_);
          if(bytesRead <= 0)
          {
            delete buffer;
            sender_.stop();
            return;
          }
          buffer->setUsed(bytesRead);
        } // unlock the mutex before sending
        sender_.sendAt(buffer, offset);
      }

      virtual void recycle(Communication::LinkedBuffer * buffer)
      {
        startWrite(buffer);
      }
    private:
      Communication::AsynchFileSender sender_;
      std::string inputFileName_;
      size_t bufferCount_;
      size_t bufferSize_;

      FILE * in_;
      // protects access to the file when we are potentially mutlithreading
      boost::mutex fileMutex_;
    };
}


BOOST_AUTO_TEST_CASE(TestAsynchFileWriter)
{
  std::string root (std::getenv ("QUICKFAST_ROOT"));
  std::string workingDirectory = root + "/src/Tests/resources/";
  std::string inputFile = workingDirectory + "fileCopyTest.dat";
  std::string outputFile = workingDirectory + "fileCopyTest.out";
  boost::filesystem::remove(outputFile);

  const size_t bufferCount = 5;
  const size_t bufferSize = 10; // force a lot of I/O

  AsynchronousFileCopier copier(
    inputFile,
    bufferCount,
    bufferSize,
    outputFile
    );
  copier.go();

  FILE * ifile = std::fopen(inputFile.c_str(), "rb");
  BOOST_REQUIRE(ifile != 0);
  std::fseek(ifile, 0, SEEK_END);
  long ilen = ftell(ifile);
  std::fseek(ifile, 0, SEEK_SET);
  boost::scoped_array<char> ibuff(new char[ilen]);
  BOOST_REQUIRE_EQUAL(ilen, std::fread(ibuff.get(), 1, ilen, ifile));
  std::fclose(ifile);

  FILE * ofile = std::fopen(outputFile.c_str(), "rb");
  BOOST_REQUIRE(ofile != 0);
  std::fseek(ofile, 0, SEEK_END);
  long olen = ftell(ofile);
  std::fseek(ofile, 0, SEEK_SET);
  boost::scoped_array<char> obuff(new char[olen]);
  BOOST_REQUIRE_EQUAL(olen, std::fread(obuff.get(), 1, olen, ofile));
  std::fclose(ofile);


  BOOST_REQUIRE_EQUAL(ilen, olen);
  BOOST_REQUIRE_EQUAL(0, std::memcmp(ibuff.get(), obuff.get(), ilen));

  boost::filesystem::remove(outputFile);
}

#endif // _WIN32
