// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "InterpretFAST.h"
#include <Codecs/DataSourceStream.h>
#include <Codecs/SynchronousDecoder.h>
#include <Codecs/TemplateRegistry.h>
#include <Messages/Message.h>

#include <Examples/MessageInterpreter.h>

using namespace QuickFAST;
using namespace Examples;

InterpretFAST::InterpretFAST()
  : verboseParse_(false)
  , verboseDecode_(false)
  , verboseExecution_(false)
  , resetOnMessage_(false)
  , strict_(true)
  , outputFile_(0)
  , echoFile_(0)
  , echoType_(Codecs::DataSource::HEX)
  , echoMessage_(true)
  , echoField_(false)
  , head_(0)
{
}

InterpretFAST::~InterpretFAST()
{
}

bool
InterpretFAST::init(int argc, char * argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
InterpretFAST::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  std::string opt(argv[0]);
  try
  {
    if(opt == "-vp")
    {
      verboseParse_ = !verboseParse_;
      consumed = 1;
    }
    else if(opt == "-vd")
    {
      verboseDecode_ = !verboseDecode_;
      consumed = 1;
    }
    else if(opt == "-vx")
    {
      verboseExecution_ = !verboseExecution_;
      consumed = 1;
    }
    else if(opt == "-r")
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
    else if(opt == "-o" && argc > 1)
    {
      outputFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-head" && argc > 1)
    {
      head_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-e" && argc > 1)
    {
      echoFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-ehex")
    {
      echoType_ = Codecs::DataSource::HEX;
      consumed = 1;
    }
    else if(opt == "-eraw")
    {
      echoType_ = Codecs::DataSource::RAW;
      consumed = 1;
    }
    else if(opt == "-enone")
    {
      echoType_ = Codecs::DataSource::NONE;
      consumed = 1;
    }
    else if(opt == "-em")
    {
      echoMessage_ = !echoMessage_;
      consumed = 1;
    }
    else if(opt == "-ef")
    {
      echoField_ = !echoField_;
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
InterpretFAST::usage(std::ostream & out) const
{
  out << "  -t file     : Template file (required)" << std::endl;
  out << "  -f file     : FAST Message file (required)" << std::endl;
  out << "  -o file     : Output file (defaults to standard out)" << std::endl;
  out << "  -e file     : Echo input to file" << std::endl;
  out << "    -ehex       : Echo as hexadecimal (default)." << std::endl;
  out << "    -eraw       : Echo as raw binary data" << std::endl;
  out << "    -enone      : Do not echo data (boundaries only)." << std::endl;
  out << "    -em         : Toggle 'echo message boundaries'(default true)" << std::endl;
  out << "    -ef         : Toggle 'echo field boundaries'(default false)" << std::endl;
  out << "  -head n     : process only the first 'n' messages" << std::endl;
  out << "  -r          : Toggle 'reset decoder on every message' (default false)." << std::endl;
  out << "  -s          : Toggle 'strict decoding rules' (default true)." << std::endl;
  out << "  -vp         : Toggle 'noisy template parsing' (default false)." << std::endl;
  out << "  -vd         : Toggle 'noisy decoding' (default false)." << std::endl;
  out << "  -vx         : Toggle 'noisy execution progress' (default false)." << std::endl;
}

bool
InterpretFAST::applyArgs()
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
      templateFile_.open(templateFileName_.c_str());
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
    if(outputFileName_.empty())
    {
      outputFile_ = &std::cout;
    }
    else if(ok)
    {
      outputFile_ = new std::ofstream(outputFileName_.c_str());
      if(!outputFile_->good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open output file: "
          << outputFileName_
          << std::endl;
      }
    }

    if(! echoFileName_.empty())
    {

      std::ios::openmode mode = std::ios::out | std::ios::trunc;
#ifdef _WIN32
      if(echoType_ == Codecs::DataSource::RAW)
      {
        mode |= std::ios::binary;
      }
#endif
      echoFile_ = new std::ofstream(echoFileName_.c_str(), mode);
      if(!echoFile_->good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open echo file: "
          << echoFileName_
          << std::endl;
      }
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
  return ok;
}

int
InterpretFAST::run()
{
  int result = 0;
  try
  {
    if(verboseExecution_)
    {
      std::cout << "Parsing templates." << std::endl;
    }

    if(verboseParse_)
    {
      parser_.setVerboseOutput(std::cout);
    }
    Codecs::TemplateRegistryPtr templateRegistry = parser_.parse(templateFile_);

    if(verboseExecution_)
    {
      std::cout << "Parsed " << templateRegistry->size() << " templates." << std::endl;
      std::cout << "Decoding messages" << std::endl;
    }

    MessageInterpreter handler(* outputFile_);
    Codecs::DataSourceStream source(fastFile_);

    Codecs::SynchronousDecoder<Messages::Message, Codecs::MessageConsumer> decoder(templateRegistry);
//    Codecs::SynchronousDecoder decoder(templateRegistry);
    decoder.setStrict(strict_);
    decoder.setResetOnMessage(resetOnMessage_);

    if(echoFile_ != 0)
    {
      source.setEcho(*echoFile_,
        echoType_,
        echoMessage_,
        echoField_);
    }
    if(verboseDecode_)
    {
      decoder.setVerboseOutput(std::cout);
    }
    decoder.decode(source, handler);

    if(verboseExecution_)
    {
      std::cout << "Decoded " << decoder.messageCount() << " messages." << std::endl;
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
InterpretFAST::fini()
{
}
