// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

// This file is intended to be used by external projects that are using the QuickFAST library.
// It is most likely that this file should be included from a precompilable header file in the
// QuickFAST-based application.

#ifdef _MSC_VER
# pragma once
// Push the warnings that will be disabled in QuickFASTPch.h
#pragma warning(push)
#endif
#ifndef QUICKFAST_H
#define QUICKFAST_H

#include <Common/QuickFASTPch.h>
#include <Common/Version.h>
#include <Common/Decimal.h>
#include <Common/StringBuffer.h>
#include <Application/DecoderConnection.h>
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Messages/FieldIdentity.h>

#ifdef _MSC_VER
// Restore the warning state
#pragma warning(pop)
#endif

#endif // QUICKFAST_H

