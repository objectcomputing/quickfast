// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "FileToMulticast.h"
#include <Communication/MulticastSender.h>
#include <Examples/StopWatch.h>
using namespace QuickFAST;
using namespace Examples;

namespace {
  void waitForEnter()
  {
    std::cout << "Hit Enter to continue:" << std::flush;
    std::cin.get();
  }
}

FileToMulticast::FileToMulticast()
: portNumber_(13014)
, sendAddress_("224.1.2.133")
, sendCount_(1)
, sendMicroseconds_(500)
, burst_(1)
, pauseEveryPass_(false)
, pauseEveryMessage_(false)
, verbose_(false)
, dataFile_(0)
, strand_(ioService_)
, timer_(ioService_)
, bufferSize_(0)
, nPass_(0)
, nMsg_(0)
, totalMessageCount_(0)
{
}

FileToMulticast::~FileToMulticast()
{
}


bool
FileToMulticast::init(int argc, char * argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
FileToMulticast::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  std::string opt(argv[0]);
  try
  {
    if(opt == "-f")
    {
      dataFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-p" && argc > 1)
    {
      portNumber_ = boost::lexical_cast<unsigned short>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-r" && argc > 1)
    {
      size_t mps = boost::lexical_cast<size_t>(argv[1]);
      if(mps > 0)
      {
        sendMicroseconds_ = 1000000/mps;
        consumed = 2;
      }
      else
      {
        sendMicroseconds_ = 0;
        consumed = 2;
      }
    }
    else if(opt == "-b" && argc > 1)
    {
      burst_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-c" && argc > 1)
    {
      sendCount_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-n" && argc > 1)
    {
      indexFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-a" && argc > 1)
    {
      sendAddress_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-pausemessage")
    {
      pauseEveryMessage_ = true;
      consumed = 1;
    }
    else if(opt == "-pausepass")
    {
      pauseEveryPass_ = true;
      consumed = 1;
    }
    else if(opt == "-v")
    {
      verbose_ = !verbose_;
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
FileToMulticast::usage(std::ostream & out) const
{
  out << "  -a dotted_ip  : Multicast send address (default is " << sendAddress_ << ")" << std::endl;
  out << "  -p port       : Multicast port number (default " << portNumber_ << ")" << std::endl;
  out << "  -f datafile   : File containing FAST encoded messages. (required)" << std::endl;
  out << "  -n indexfile  : File produced as an echo file with message boundaries" << std::endl;
  out << "                  by the InterpretFAST program." << std::endl;
  out << "                  If no index file is specified, the datafile is treated as" << std::endl;
  out << "                  a single message and must fit in one datagram." << std::endl;
  out << "  -r burst/sec  : Rate at which to send bursts of messages expressed as bursts per second (default = 2000)" << std::endl;
  out << "                : zero means send continuously." << std::endl;
  out << "  -b msg/burst  : Messages per burst(default = 1)" << std::endl;
  out << "  -c count      : How many times to send the file (passes)" << std::endl;
  out << "                  (default 1; 0 means forever.)" << std::endl;
  out << "  -pausemessage : Wait for 'Enter' before every message." << std::endl;
  out << "  -pausepass    : Wait for 'Enter' before every pass." << std::endl;
  out << "  -v            : Noise to the console while it runs" << std::endl;
}

bool
FileToMulticast::applyArgs()
{
  bool ok = true;
  try
  {
    if(dataFileName_.empty())
    {
      ok = false;
      std::cerr << "ERROR: -f [datafile] option is required." << std::endl;
      commandArgParser_.usage(std::cerr);
    }
    if(ok)
    {
      dataFile_ = fopen(dataFileName_.c_str(), "rb");
      if(!dataFile_)
      {
        ok = false;
        std::cerr << "ERROR: Can't open file: "
          << dataFileName_
          << std::endl;
      }
    }

    ok = ok && parseIndexFile();

    sender_.reset(new Communication::MulticastSender(
      ioService_,
      *this,
      sendAddress_, portNumber_));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    ok = false;
  }
  return ok;
}

bool
FileToMulticast::parseIndexFile()
{
  bool ok = true;
  size_t messageStart = 0;
  if(!indexFileName_.empty())
  {
    std::ifstream indexFile(indexFileName_.c_str());
    if(!indexFile.good())
    {
      ok = false;
      std::cerr << "ERROR: Can't open index file: "
        << indexFileName_
        << std::endl;
    }

    const std::string messagePrefix("***MESSAGE @");
    const std::string messageSuffix("***");
    size_t prefixLength = messagePrefix.length();
    size_t suffixLength = messageSuffix.length();

    while(ok && !indexFile.eof())
    {
      char lineBuffer[100];
      indexFile.getline(lineBuffer, sizeof(lineBuffer));
      std::string line(lineBuffer);
      if(line.compare(0, prefixLength, messagePrefix) == 0)
      {
//        size_t hexlength = line.length() - prefixLength - suffixLength;
        std::stringstream hex(line.substr(prefixLength, line.length() - suffixLength));
        size_t messageEnd = 0;
        hex >> std::hex >> messageEnd;
        if(messageStart < messageEnd)
        {
          messageIndex_.push_back(MessagePosition(messageStart, messageEnd - messageStart));
          messageStart = messageEnd;
        }
      }
    }
  }

  if(ok)
  {
    // anything left in the file is the last message
    // note this also handles the case that there is no index file
    // also note that we leave the file positioned at the end (but it doesn't matter)
    fseek(dataFile_, 0, SEEK_END);
    size_t messageEnd = ftell(dataFile_);
    if(messageStart < messageEnd)
    {
      messageIndex_.push_back(MessagePosition(messageStart, messageEnd - messageStart));
      messageStart = messageEnd;
    }
  }

  bufferSize_ = 0;
  for(size_t nMsg = 0; nMsg < messageIndex_.size(); ++nMsg)
  {
    const MessagePosition & position = messageIndex_[nMsg];
    if(position.second > bufferSize_)
    {
      bufferSize_ = position.second;
    }
  }

  buffer_.reset(new unsigned char[bufferSize_]);
  return ok;
}

int
FileToMulticast::run()
{
  try
  {
    sender_->initializeSender();
    if(verbose_)
    {
      std::cout << "Sending " << messageIndex_.size() << " messages. "
        << "Largest is " << bufferSize_ << " bytes." << std::endl;
    }

    strand_.dispatch(
        strand_.wrap(boost::bind(&FileToMulticast::sendBurst, this)));
    StopWatch lapse;
    this->ioService_.run();
    unsigned long sendLapse = lapse.freeze();
    std::cout << "sent "
      << totalMessageCount_
      << " messages in "
      << std::fixed << std::setprecision(3)
      << sendLapse
      << " milliseonds. [";
    std::cout << std::fixed << std::setprecision(3)
      << double(sendLapse)/double(totalMessageCount_) << " msec/message. = "
      << std::fixed << std::setprecision(0)
      << 1000. * double(totalMessageCount_)/double(sendLapse) << " message/second.]"
      << std::endl;

#ifdef _WIN32
    // On WIN32 if sender closes the socket before a localhost receiver
    // reads all data, any unread data is thrown away.
    // This ugly sleep lets the dust settle.
    Sleep(1000);
#endif // _WIN32
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

void
FileToMulticast::sendBurst()
{
  try
  {
    // set the next timeout
    if(sendMicroseconds_ != 0)
    {
      timer_.expires_from_now(boost::posix_time::microseconds(sendMicroseconds_));
      timer_.async_wait(
        strand_.wrap(boost::bind(&FileToMulticast::sendBurst, this))
        );
    }

    for(size_t nBurstMsg = 0; nBurstMsg < burst_; ++nBurstMsg)
    {
      if(nMsg_ >= messageIndex_.size())
      {
        // completed a pass;  count it and see if we should stop or pause
        nPass_ += 1;
        if(nPass_ >= sendCount_ && sendCount_ != 0)
        {
          ioService_.stopService();
          return;
        }
        if(verbose_)
        {
          std::cout << "Begin pass #" << nPass_ << " of " << sendCount_ << std::endl;
        }
        if(pauseEveryPass_)
        {
          waitForEnter();
        }
        nMsg_ = 0;
      }

      if(verbose_)
      {
        std::cout << "Send message #" << nMsg_ + 1 << " of " << messageIndex_.size() << std::endl;
      }
      if(pauseEveryMessage_)
      {
        waitForEnter();
      }

      // then send this message
      const MessagePosition & position = messageIndex_[nMsg_];
      nMsg_ += 1;
      totalMessageCount_ += 1;
      size_t messageStart = position.first;
      size_t messageLength = position.second;
      fseek(dataFile_,  long(messageStart), SEEK_SET);
      assert(messageLength <= bufferSize_);
      size_t bytesRead = fread(buffer_.get(), 1, messageLength, dataFile_);
      if(bytesRead == 0){} // avoid "Unused local" warning
      assert (bytesRead == messageLength);
      sender_->send(boost::asio::buffer(buffer_.get(), messageLength));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    ioService_.stopService();
  }
}


void
FileToMulticast::fini()
{
}

void
FileToMulticast::recycle(Communication::LinkedBuffer * emptyBuffer)
{
  int todo;
}
