// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "TutorialApplication.h"
#include <Communication/RawFileReceiver.h>
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/StreamingAssembler.h>
#include <Codecs/NoHeaderAnalyzer.h>
#include <Codecs/GenericMessageBuilder.h>
#include <Examples/MessageInterpreter.h>

using namespace QuickFAST;
using namespace Examples;

namespace
{
  // On Windows we need to open a FAST data file in binary mode to avoid translation of end of line characters.
  // Other platforms do not have this requirement so we make binaryMode a "nop" for them.
#ifdef _WIN32
  const std::ios::openmode binaryMode = std::ios::binary;
#else
  const std::ios::openmode binaryMode = reinterpret_cast<std::ios::openmode>(0);
#endif
}

///////////////////////
// TutorialApplication

TutorialApplication::TutorialApplication()
{
}

TutorialApplication::~TutorialApplication()
{
}

bool
TutorialApplication::init(int argc, char * argv[])
{
  bool ok = false;
  if(argc == 3)
  {
    templateFileName_ = argv[1];
    fastFileName_ = argv[2];
    ok = true;
  }
  else
  {
    std::cout << "Usage: TutorialApplication [Template File Name] [FAST Data File Name]" << std::endl;
  }
  return ok;
}

int
TutorialApplication::run()
{
  int result = 0;
  try
  {
    ////////////////////////////////////////////////
    // Open files first to be sure they are present.
    std::ifstream templates(templateFileName_.c_str());
    if(!templates.good())
    {
      result = -1;
      std::cerr << "ERROR: Can't open template file: "
        << templateFileName_
        << std::endl;
    }

    std::ifstream fastFile(fastFileName_.c_str(), binaryMode);
    if(!fastFile.good())
    {
      result = -1;
      std::cerr << "ERROR: Can't open FAST data file: "
        << fastFileName_
        << std::endl;
    }

    if(result == 0)
    {
      ///////////////////////////////////////////////////////////////////////////////
      // Create an object to receive data from a raw data file (i.e to read the file)
      Communication::RawFileReceiver receiver(fastFile);

      /////////////////////////////////////////////
      // Parse the templates from the template file
      // errors are reported by throwing exceptions
      // which are caught below.
      Codecs::XMLTemplateParser parser;
      Codecs::TemplateRegistryPtr registry = parser.parse(templates);

      /////////////////////////////////////////////
      // For the tutorial assume there is no header
      // on the incoming messages.
      Codecs::NoHeaderAnalyzer analyzer;

      //////////////////////////////////////
      // Create an application object to use
      // the incoming data.  In this case to
      // accept complete messages and interpret
      // them to standard out.
      MessageInterpreter handler(std::cout);
      // and use the interpreter as the consumer
      // of generic messages.
      Codecs::GenericMessageBuilder builder(handler);

      //////////////////////////////////////
      // Now pull all the pieces together
      // into an assembler.
      Codecs::StreamingAssembler assembler(
            registry,
            analyzer,
            builder,
            false);

      /////////////////////////////////////////////
      // set options in the assembler if necessary.
      // In this case configure the assembler to
      // reset the decoder on every incoming message
      assembler.setReset(true);

      /////////////////////////////////////////////
      // do final initialzation of the data receiver.
      receiver.start(assembler);

      /////////////////////////////////////
      // run the event loop in this thread.
      // Do not return until receiver stops.
      // The RawFileReceiver will stop at end of file.
      receiver.run();
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
TutorialApplication::fini()
{
}
