// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include <InterpretApplication/InterpretApplication.h>

//#include <Codecs/TCPReceiver.h>

using namespace QuickFAST;
using namespace Examples;

int main(int argc, char* argv[])
{
  int result = -1;
#if 0
  if(argc <= 1)
  {
    Codecs::TCPReceiver receiver("hello", 1000);
    
    return 0;
  }
#endif
  InterpretApplication application;
  if(application.init(argc, argv))
  {
    result = application.run();
    application.fini();
  }
  return result;
}
