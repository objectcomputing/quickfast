// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <Common/QuickFAST_Export.h>
namespace QuickFAST{
  /// @brief Exception to be thrown when an attempt is made to convert a value to an incompatable type.
  class /* QuickFAST_Export */ UnsupportedConversion : public std::domain_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    UnsupportedConversion(const std::string & reason)
      : std::domain_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when a value does not fit in a field
  class /* QuickFAST_Export */ OverflowError : public std::overflow_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    OverflowError(const std::string & reason)
      : std::overflow_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when a mandatory field is not present or a requested field is not found.
  class /* QuickFAST_Export */ FieldNotPresent : public std::invalid_argument{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    FieldNotPresent(const std::string & reason)
      : std::invalid_argument(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when a dynamic error is detected while encoding or decoding
  class /* QuickFAST_Export */ EncodingError : public std::runtime_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    EncodingError(const std::string & reason)
      : std::runtime_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when a static error is detected while parsing or using templates.
  class /* QuickFAST_Export */ TemplateDefinitionError: public std::runtime_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    TemplateDefinitionError(const std::string & reason)
      : std::runtime_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when the application misuses QuickFAST
  class /* QuickFAST_Export */ UsageError : public std::invalid_argument{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param error describes the problem.
    /// @param context describes the context in which the error was detected
    UsageError(const char * error, const char * context)
      : std::invalid_argument((std::string(error) + context).c_str())
    {
    }
  };
}
#endif // EXCEPTIONS_H
