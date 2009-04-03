// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <Common/Exceptions.h>

namespace QuickFASTDotNet{

  public ref class UnexpectedError: public System::Exception
  {
  public:
    UnexpectedError()
    {
    }

    UnexpectedError(System::String^ reason)
      : System::Exception(reason)
    {
    }
  };

  /// @brief Exception to be thrown when an attempt is made to convert a value to an incompatable type.
  public ref class UnsupportedConversion : public System::Exception{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    UnsupportedConversion(System::String^ reason)
      : System::Exception(reason)
    {
    }

  internal:
    UnsupportedConversion(const QuickFAST::UnsupportedConversion& inner)
      : System::Exception(gcnew System::String(inner.what()))
    {
    }
  };

  /// @brief Exception to be thrown when a value does not fit in a field
  public ref class OverflowError : public System::Exception{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    OverflowError(System::String^ reason)
      : System::Exception(reason)
    {
    }

  internal:
    OverflowError(const QuickFAST::OverflowError& inner)
      : System::Exception(gcnew System::String(inner.what()))
    {
    }
  };

  /// @brief Exception to be thrown when a mandatory field is not present or a requested field is not found.
  public ref class FieldNotPresent : public System::Exception{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    FieldNotPresent(System::String^ reason)
      : System::Exception(reason)
    {
    }

  internal:
    FieldNotPresent(const QuickFAST::FieldNotPresent& inner)
      : System::Exception(gcnew System::String(inner.what()))
    {
    }
  };

  /// @brief Exception to be thrown when a dynamic error is detected while encoding or decoding
  public ref class EncodingError : public System::Exception{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    EncodingError(System::String^ reason)
      : System::Exception(reason)
    {
    }

  internal:
    EncodingError(const QuickFAST::EncodingError& inner)
      : System::Exception(gcnew System::String(inner.what()))
    {
    }
  };

    /// @brief Exception to be thrown when a static error is detected while parsing or using templates.
  public ref class TemplateDefinitionError: public System::Exception {
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    TemplateDefinitionError(System::String^ reason)
      : System::Exception(reason)
    {
    }

  internal:
    TemplateDefinitionError(const QuickFAST::TemplateDefinitionError& inner)
      : System::Exception(gcnew System::String(inner.what()))
    {
    }
  };

  /// @brief Exception to be thrown when the application misuses QuickFAST
  public ref class UsageError : public System::Exception{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param error describes the problem.
    /// @param context describes the context in which the error was detected
    UsageError(System::String^ reason)
      : System::Exception(reason)
    {
    }

  internal:
    UsageError(const QuickFAST::UsageError& inner)
      : System::Exception(gcnew System::String(inner.what()))
    {
    }
  };
}
