// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include <FileToTCP/FileToTCP.h>

using namespace QuickFAST;
using namespace Examples;

int main(int argc, char* argv[])
{
  int result = -1;
  FileToTCP application;
  if(application.init(argc, argv))
  {
    result = application.run();
    application.fini();
  }
  return result;
}
