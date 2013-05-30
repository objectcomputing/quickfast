// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include "Stopwatch.h"

using namespace QuickFAST::DotNet;
Stopwatch::Stopwatch()
  : stopwatch_(System::Diagnostics::Stopwatch::StartNew())
{
}

void
Stopwatch::Stop()
{
  stopwatch_->Stop();
}

unsigned __int64
Stopwatch::ElapsedMilliseconds::get()
{
  return stopwatch_->ElapsedMilliseconds::get();
}

