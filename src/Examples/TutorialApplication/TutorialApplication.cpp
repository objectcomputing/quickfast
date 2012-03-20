// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "TutorialApplication.h"
#include <Codecs/GenericMessageBuilder.h>
#include <Examples/MessageInterpreter.h>

using namespace QuickFAST;
using namespace Examples;

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
  // For the tutorial, we hard code two command line arguments.
  // Consider using the command argument parser: Application::CommandArgParser
  // for more flexible command line handling.
  bool ok = false;
  if(argc == 3)
  {
    configuration_.setTemplateFileName(argv[1]);
    configuration_.setFastFileName(argv[2]);
    // Set configuration options.
    //
    // Many of these are the defaults so they do not need to be set explicitly
    //
    // In interpretApplication these are set by command line options.
    // Since DecoderConfiguration includes command line parsing support, you can simply pass the args
    // to configuration_.
    //
    // See InterpretApplication::parseSingleArg and DecoderConfiguration::parseSingleArg for examples.
    //
    // In a production application you may want to hard code configuration options the way they are set here:
    configuration_.setReset(true);
    configuration_.setReceiverType(Application::DecoderConfiguration::RAWFILE_RECEIVER);
    configuration_.setAssemblerType(Application::DecoderConfiguration::STREAMING_ASSEMBLER);
    configuration_.setWaitForCompleteMessage(false);
    configuration_.setMessageHeaderType(Application::DecoderConfiguration::NO_HEADER);
    configuration_.setPacketHeaderType(Application::DecoderConfiguration::NO_HEADER);

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
    //////////////////////////////////////
    // Create an application object to use
    // the incoming data.  In this case to
    // accept complete messages and interpret
    // them to standard out.
    MessageInterpreter handler(std::cout);
    // and use the interpreter as the consumer
    // of generic messages.
    Codecs::GenericMessageBuilder builder(handler);

    ////////////////////
    // IMPORTANT:
    // GenericMessageBuilder is acceptable but
    // much higher performance can be achieved by
    // creating your own implementation of the
    // ValueMessageBuilder interface.
    //
    // If you want output as FIX messages rather than as
    // human readable messages, consider the ValueToFix
    // MessageBuilder defined in src/Examples/Examples/ValueToFix.h

    /////////////////////////////////////////////
    // Use the DecoderConfiguration to initialize the DecoderConnection:
    // Note: this assumes that execution stays in this block.   The handler and builder
    // object must stay in scope during the decoding process.
    // If you plan to use the main thread for other purposes, consider
    // making builder & handler member variables, or using smart pointers to
    // manage their lifetime.
    connection_.configure(builder, configuration_);

    /////////////////////////////////////
    // run the event loop in this thread.
    // Do not return until receiver stops.
    //
    // Decoded messages will be passed to the MessageInterpreter for processing.
    //
    // See DecoderConnection::runThreads() for an approach that does not
    // tie up the main thread.
    connection_.run();
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
