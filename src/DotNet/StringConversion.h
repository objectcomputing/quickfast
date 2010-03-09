// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

namespace QuickFAST
{
  namespace DotNet
  {
    /// @brief Assign .NET string to C++ string-like object
    template<typename StdStringType>
    void assignString(
      StdStringType& stdStringOut,
      System::String^ stringIn)
    {
      using namespace System::Runtime::InteropServices;
      typedef typename StdStringType::value_type FTChar;

       FTChar* chars = reinterpret_cast<FTChar*>((Marshal::StringToHGlobalAnsi(stringIn)).ToPointer());
       stdStringOut = chars;
       Marshal::FreeHGlobal(System::IntPtr((void*)chars));
    }

    /// @brief Cast from a .NET string to the specified c++ string-like object
    template<typename StdStringType>
    StdStringType string_cast(System::String^ stringIn)
    {
      StdStringType stdStrOut;
      assignString(stdStrOut, stringIn);
      return stdStrOut;
    }

    /// @brief cast from a C++ string-like object to a .NET String
    template<typename StdString>
    System::String^ string_cast(const StdString& stdString)
    {
      return gcnew System::String(stdString.c_str());
    }

  }
}

