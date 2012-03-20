// Copyright (c) 2009, 2010, 2011,Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

//#include <Common/QuickFAST_Export.h>
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

    /// @brief Special construtor to provide explicit type information about the failed conversion.
    ///
    /// @param actualType is the type of actually found
    /// @param desiredType is the type that was requested
    UnsupportedConversion(const std::string & actualType, const std::string & desiredType)
      : std::domain_error((actualType + " cannot be converted to " + desiredType).c_str())
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

  /// @brief Exception to be thrown when processing cannot continue due to a communication error
  class /* QuickFAST_Export */ CommunicationError: public std::runtime_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    CommunicationError(const std::string & reason)
      : std::runtime_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when the application misuses QuickFAST
  class /* QuickFAST_Export */ UsageError : public std::invalid_argument{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param errorType the type of problem. (Coding Error, Internal Error, etc.)
    /// @param description the actual problem.
    UsageError(const char * errorType, const char * description)
      : std::invalid_argument((std::string(errorType) + ": " +  description).c_str())
    {
    }
  };

  /// @brief Exception to be thrown when an internal logic error is detected
  class /* QuickFAST_Export */ InternalError : public std::logic_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param errorType the type of problem. (Coding Error, Internal Error, etc.)
    /// @param description the actual problem.
    InternalError(const char * errorType, const char * description)
      : std::logic_error((std::string(errorType) +  ": " + description).c_str())
    {
    }
  };
}
#endif // EXCEPTIONS_H
