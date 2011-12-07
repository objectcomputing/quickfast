// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "FileToTCP.h"

#include <boost/asio.hpp>

using namespace QuickFAST;
using namespace Examples;
using boost::asio::ip::tcp;

FileToTCP::FileToTCP()
: portNumber_(9876)
, bufferSize_(1024)
, sendCount_(0)
, inFile_(0)
, verbose_(false)
, hack_(false)
{
}

FileToTCP::~FileToTCP()
{
}


bool
FileToTCP::init(int argc, char * argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
FileToTCP::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  std::string opt(argv[0]);
  try
  {
    if(opt == "-f")
    {
      fileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-p" && argc > 1)
    {
      portNumber_ = boost::lexical_cast<unsigned short>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-c" && argc > 1)
    {
      sendCount_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-hack")
    {
      hack_ = true;
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
FileToTCP::usage(std::ostream & out) const
{
  out << "  -f file     : File to send (required)" << std::endl;
  out << "  -c count    : How many time to deliver the file" << std::endl;
  out << "                (default 0 means forever.)" << std::endl;
  out << "  -p port     : Listen on the supplied port number (default 9876)" << std::endl;
}

bool
FileToTCP::applyArgs()
{
  bool ok = true;
  try
  {
    if(fileName_.empty())
    {
      ok = false;
      std::cerr << "ERROR: -f [file] option is required." << std::endl;
      commandArgParser_.usage(std::cerr);
    }
    if(ok)
    {
      inFile_ = fopen(fileName_.c_str(), "rb");
      if(!inFile_)
      {
        ok = false;
        std::cerr << "ERROR: Can't open file: "
          << fileName_
          << std::endl;
      }
    }
  }
  catch (std::exception & ex)
  {
    std::cerr << ex.what() << std::endl;
    ok = false;
  }
  return ok;
}

int
FileToTCP::run()
{
  try
  {
    tcp::endpoint endpoint(tcp::v4(), portNumber_);
    tcp::acceptor acceptor(io_service_, endpoint);

    for (size_t count = 0; count < sendCount_ || sendCount_ == 0; ++count)
    {
      tcp::iostream stream;
      if(verbose_)
      {
        std::cout << "Listening" << std::endl;
      }
      acceptor.accept(*stream.rdbuf());
      if(verbose_)
      {
        std::cout << "Accepting" << std::endl;
      }
      if(hack_)
      {
        while(stream.good())
        {
          char c;
          c = (char)stream.get();
          std::cout << c << std::flush;
        }
        return 0;
      }
      fseek(inFile_, 0, SEEK_SET);
      char * buffer = new char[bufferSize_];
      while(!feof(inFile_))
      {
        if(verbose_)
        {
          std::cout << "Reading " << bufferSize_ << " bytes." << std::endl;
        }
        size_t bytes = fread(buffer, 1, bufferSize_, inFile_);
        if(verbose_)
        {
          std::cout << "Sending " << bytes << " bytes." << std::endl;
        }
        if(bytes > 0)
        {
          stream.write(buffer, bytes);
        }
        else
        {
          break;
        }
      }
#ifdef _WIN32
      // On WIN32 if sender closes the socket before a localhost receiver
      // reads all data, any unread data is thrown away.
      // This ugly sleep lets the dust settle.
      Sleep(1000);
#endif // _WIN32
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

void
FileToTCP::fini()
{
}

