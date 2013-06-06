// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "InterpretApplication.h"
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/GenericMessageBuilder.h>

#include <Communication/Receiver.h>

#include <Examples/MessageInterpreter.h>
#include <Examples/ValueToFix.h>

namespace {
  template< typename T>
  void ignore_returnvalue(const T&) { }
}

using namespace QuickFAST;
using namespace Examples;

///////////////////////
//

InterpretApplication::InterpretApplication()
: configuration_(new Application::DecoderConfiguration)
, console_(false)
, fixOutput_(false)
, threads_(1)
, silent_(false)
{
}

InterpretApplication::~InterpretApplication()
{
}

bool
InterpretApplication::init(int argc, char * argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
InterpretApplication::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  std::string opt(argv[0]);
  try
  {
    if(opt == "-buffer" && argc > 1)
    {
      configuration_->setReceiverType(Application::DecoderConfiguration::BUFFER_RECEIVER);
      bufferFilename_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-threads" && argc > 1)
    {
      threads_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-ofix")
    {
      fixOutput_ = true;
      consumed = 1;
    }
    else if(opt == "-connection" && argc > 1)
    {
      if(!defaultConfiguration_)
      {
        defaultConfiguration_ = configuration_;
      }
      else
      {
        configurations_.push_back(configuration_);
      }
      configuration_.reset(new Application::DecoderConfiguration(*defaultConfiguration_));
      configuration_->setExtra("Name", argv[1]);
      consumed = 2;
    }
    else if(opt == "-console")
    {
      console_ = true;
      consumed = 1;
    }
    else if(opt == "-silent")
    {
      silent_ = true;
      consumed = 1;
    }
    else
    {
      consumed = configuration_->parseSingleArg(argc, argv);
    }
  }
  catch (std::exception & ex)
  {
    // because the lexical cast can throw
    std::cerr << ex.what() << " while interpreting " << opt << std::endl;
    consumed = 0;
  }
  return consumed;
}

void
InterpretApplication::usage(std::ostream & out) const
{
  configuration_->usage(out);
  out << std::endl;
  out << "  -connection name     : Begin configuring a new connection." << std::endl;
  out << "                         If this does not appear, then a single unnamed connection will be configured." << std::endl;
  out << "                         The -connection option may appear multiple times." << std::endl;
  out << "                         Each occurrence of this option configures a new connection." << std::endl;
  out << "                         The first time this appears any settings that were already set become defaults for" << std::endl;
  out << "                         all configurations.  These defaults can then be overridden by subsequent command" << std::endl;
  out << "                         line options." << std::endl;
  out << std::endl;
  out << "  -console             : While running, listen for single character commands from the console." << std::endl;
  out << "                         Currently defined commands are:" << std::endl;
  out << "                          <q>uit.    : exit the program." << std::endl;
  out << "                          <p>ause.   : temporarily unsubscribe from the multicast feed" << std::endl;
  out << "                          <r>esume.  : resubscribe after a pause." << std::endl;
  out << "                         Note that you must hit ENTER before the command will be recognized." << std::endl;
  out << std::endl;
  out << "  -ofix                : Write the output as newline separated FIX records." << std::endl;
  out << "  -buffer file         : Input from raw FAST message file into a buffer; decode from buffer." << std::endl;
  out << std::endl;
}

bool
InterpretApplication::applyArgs()
{
  bool ok = true;
  try
  {
    if(configuration_->templateFileName().empty())
    {
      ok = false;
      std::cerr << "ERROR: -t [templatefile] option is required." << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    ok = false;
  }
  if(!ok)
  {
    commandArgParser_.usage(std::cerr);
  }

  configurations_.push_back(configuration_);
  configuration_.reset();
  return ok;
}

int
InterpretApplication::run()
{
  int result = 0;
  try
  {
    MessageInterpreter handler(std::cout, silent_);
    for(Configurations::const_iterator pConfig = configurations_.begin();
      pConfig != configurations_.end();
      ++pConfig)
    {
      Messages::ValueMessageBuilderPtr builder;

      if(fixOutput_)
      {
        builder.reset(new ValueToFix(std::cout));
      }
      else
      {
        builder.reset(new Codecs::GenericMessageBuilder(handler));
      }
      builders_.push_back(builder);

      ConnectionPtr pConnection(new Application::DecoderConnection);

      pConnection->configure(*builder, **pConfig);
      connections_.push_back(pConnection);
    }

    if(console_)
    {
      // start threads to receive data, but reserve this thread for console input
      for(Connections::const_iterator pConnection = connections_.begin();
        pConnection != connections_.end();
        ++pConnection)
      {
        (*pConnection)->receiver().runThreads(threads_, false);
      }
      bool more = true;
      while(more)
      {
        char c;
        std::cin >> c;
        if(isgraph(c))
        {
          if(c == 'q')
          {
            // todo: extend this to stop a songle connection
            more = false;
            for(Connections::const_iterator pConnection = connections_.begin();
              pConnection != connections_.end();
              ++pConnection)
            {
              (*pConnection)->receiver().stop();
              (*pConnection)->receiver().joinThreads();
            }
          }
          else if (c == 'p')
          {
            // todo: refine this to pause a songle connection
            for(Connections::const_iterator pConnection = connections_.begin();
              pConnection != connections_.end();
              ++pConnection)
            {
              (*pConnection)->receiver().pause();
            }
          }
          else if( c == 'r')
          {
            // todo: refine this to pause a songle connection
            for(Connections::const_iterator pConnection = connections_.begin();
              pConnection != connections_.end();
              ++pConnection)
            {
              (*pConnection)->receiver().resume();
            }
          }
          else
          {
            std::cout << "(q)uit; (p)ause; (r)esume" << std::endl;
          }
        }
      }
    }
    else if(!bufferFilename_.empty())
    {
      // only one connection.
      // (this could be fixed)
      ConnectionPtr pConnection = connections_.front();
      std::FILE * bufferFile = std::fopen(bufferFilename_.c_str(),
#ifdef _WIN32
        "rb"
#else
        "r"
#endif
        );
      if(bufferFile <= 0)
      {
        std::cerr << "Can't open file " << bufferFilename_ << std::endl;
        return -1;
      }
      std::fseek(bufferFile, 0, SEEK_END);
      size_t fileSize = std::ftell(bufferFile);
      std::fseek(bufferFile, 0, SEEK_SET);
      boost::scoped_array<unsigned char> buffer(new unsigned char[fileSize]);
      ignore_returnvalue( std::fread(buffer.get(), 1, fileSize, bufferFile));
      pConnection->receiver().receiveBuffer(buffer.get(), fileSize);
    }
    else
    {
      // run the event loop in this thread.  Do not return until
      // receiver is stopped.
      // todo: refine this to pause a songle connection
      for(Connections::const_iterator pConnection = connections_.begin();
        pConnection != connections_.end();
        ++pConnection)
      {
        if(pConnection != connections_.end()-1)
        {
          (*pConnection)->receiver().runThreads(threads_,  false);
        }
        else
        {
          (*pConnection)->receiver().runThreads(threads_ - 1, true);
        }
      }
      connections_.pop_back(); // discard the last connection.  We've already joind it in runThreads
      for(Connections::const_iterator pConnection = connections_.begin();
        pConnection != connections_.end();
        ++pConnection)
      {
        (*pConnection)->receiver().joinThreads();
      }
    }
  }

  catch (std::exception & e)
  {
    std::cerr << e.what() << std::endl;
    result = -1;
  }
  return result;
}

void
InterpretApplication::fini()
{
}
