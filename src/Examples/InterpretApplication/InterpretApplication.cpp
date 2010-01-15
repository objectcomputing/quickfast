// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "InterpretApplication.h"
#include <Codecs/DataSourceStream.h>
#include <Codecs/SynchronousDecoder.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/GenericMessageBuilder.h>

#include <Examples/MessageInterpreter.h>

using namespace QuickFAST;
using namespace Examples;

///////////////////////
// InterpretApplication

InterpretApplication::InterpretApplication()
  : outputFile_(0)
  , isBlocked_(0)
  , echoFile_(0)
  , echoType_(Codecs::DataSource::HEX)
  , echoMessage_(true)
  , echoField_(false)
  , head_(0)
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
    if(opt == "-v")
    {
      connection_.setVerbose(true);
      consumed = 1;
    }
    else if(opt == "-r")
    {
      connection_.setResetOnMessage(true);
      consumed = 1;
    }
    else if(opt == "-s")
    {
      connection_.setStrict(true);
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
    else if(opt == "-b")
    {
      isBlocked_ = true;
      consumed = 1;
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
InterpretApplication::usage(std::ostream & out) const
{
  out << "  -t file     : Template file (required)" << std::endl;
  out << "  -f file     : FAST Message file (required)" << std::endl;
  out << "  -b          : FAST Message file contains block lengths." << std::endl;
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
  out << "  -v          : Toggle 'verbose' (default false)." << std::endl;
}

bool
InterpretApplication::applyArgs()
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
    }
    if(fastFileName_.empty())
    {
      ok = false;
      std::cerr << "ERROR: -f [FASTfile] option is required." << std::endl;
    }
    if(ok)
    {
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
      if(echoFileName_ == "cout")
      {
        echoFile_ = & std::cout;
      }
      else if(echoFileName_ == "cerr")
      {
        echoFile_ = & std::cerr;
      }
      else
      {
        echoFile_ = new std::ofstream(echoFileName_.c_str(), mode);
      }
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
InterpretApplication::run()
{
  int result = 0;
  try
  {
    connection_.parseTemplates(templateFileName_);

    connection_.setEcho(echoFile_, echoType_, echoMessage_, echoField_);

    MessageInterpreter handler(* outputFile_);
    connection_.decodeFile(
      fastFileName_,
      isBlocked_,
      handler);

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
