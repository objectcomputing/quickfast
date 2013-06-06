// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef CONTEXT_H
#define CONTEXT_H
#include <Common/QuickFASTPch.h>
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Common/Value.h>
#include <Common/Exceptions.h>
#include <Common/WorkingBuffer.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Codecs/Template_fwd.h>
#include <Messages/FieldIdentity_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Context in which Encode/Decode takes place.
    class QuickFAST_Export Context
    {
    public:
      /// @brief Return values from getDictionaryValue
      enum DictionaryStatus{
        UNDEFINED_VALUE,
        NULL_VALUE,
        OK_VALUE
      };
      /// @brief Template ID defined in SCP to mean: reset the xcoder.
      static const template_id_t SCPResetTemplateId = 120;
    public:
      /// @brief Construct with a TemplateRegistry containing all templates to be used.
      /// @param registry A registry containing all templates to be used to decode messages.
      Context(TemplateRegistryCPtr registry);

      /// @brief a typical virtual destructor.
      virtual ~Context();

      /// @brief Enable/disable strict checking of conformance to the FAST standard
      ///
      /// If strict is false some conditions such as integer overflow or inefficient encoding
      /// will be ignored.  The default is true -- strict checking is enabled.
      /// @param strict true to enable; false to disable strict checking
      void setStrict(bool strict)
      {
        strict_ = strict;
      }

      /// @brief get the current status of the strict property.
      /// @returns true if strict checking is enabled.
      bool getStrict()const
      {
        return strict_;
      }

      /// @brief Reset decoding state to initial conditions
      /// @param resetTemplateId Normally you want to reset the template ID
      ///        however there are cases when you don't.
      void reset(bool resetTemplateId = true);

      /// @brief Remember the id of the template driving the Xcoding.
      void setTemplateId(const template_id_t & templateId)
      {
        templateId_ = templateId;
      }

      /// @brief Recall the id of the template driving the Xcoding.
      template_id_t getTemplateId()const
      {
        return templateId_;
      }

      /// @brief Access the TemplateRegistry used in this context.
      /// @returns a constant pointer to the registry
      TemplateRegistryCPtr getTemplateRegistry()const
      {
        return templateRegistry_;
      }

      /// @brief Find a template in the TemplateRepository used by this Context
      /// @param name of the template being sought
      /// @param nameSpace that qualifies name
      /// @param result will point to the template that was found
      /// @returns true if successful
      bool findTemplate(const std::string & name, const std::string & nameSpace, TemplateCPtr & result) const;

      //////////////////////////////
      // Support for decoding fields
      /// @brief Find the definition of a field in the dictionary
      /// @param index identifies the dictionary entry corresponding to this field
      /// @param value receives the pointer to the value found
      /// @returns true if a valid entry was found
      bool findDictionaryField(size_t index, Value *& value);

      /// @brief Sets the value in the dictionary to NULL
      /// @param index identifies the dictionary entry corresponding to this field
      void setDictionaryValueNull(size_t index)
      {
        if(index > indexedDictionarySize_)
        {
          throw TemplateDefinitionError("Illegal dictionary index.");
        }
        indexedDictionary_[index].setNull();
      }

      /// @brief Sets the value in the dictionary to be undefined
      /// @param index identifies the dictionary entry corresponding to this field
      void setDictionaryValueUndefined(size_t index)
      {
        if(index > indexedDictionarySize_)
        {
          throw TemplateDefinitionError("Illegal dictionary index.");
        }
        indexedDictionary_[index].setUndefined();
      }

      /// @brief Sets the value in the dictionary
      /// @param index identifies the dictionary entry corresponding to this field
      /// @param value is the new value for the dictionary entry
      template<typename VALUE_TYPE>
      void setDictionaryValue(size_t index, const VALUE_TYPE & value)
      {
        if(index > indexedDictionarySize_)
        {
          throw TemplateDefinitionError("Illegal dictionary index.");
        }
        indexedDictionary_[index].setValue(value);
      }

      /// @brief Sets the string value in the dictionary
      /// @param index identifies the dictionary entry corresponding to this field
      /// @param value points to the string to be stored
      /// @param length is the lenght of the string pointed to by value
      void setDictionaryValue(size_t index, const unsigned char * value, size_t length)
      {
        if(index > indexedDictionarySize_)
        {
          throw TemplateDefinitionError("Illegal dictionary index.");
        }
        indexedDictionary_[index].setValue(value, length);
      }

      /// @brief Get a value from the dictionary
      /// @param index identifies the dictionary entry corresponding to this field
      /// @param value receives the stored value
      template<typename VALUE_TYPE>
      DictionaryStatus getDictionaryValue(size_t index, VALUE_TYPE & value)
      {
        if(index > indexedDictionarySize_)
        {
          throw TemplateDefinitionError("Illegal dictionary index.");
        }
        Value & entry = indexedDictionary_[index];
        if(!entry.isDefined())
        {
          return UNDEFINED_VALUE;
        }
        if(entry.isNull())
        {
          return NULL_VALUE;
        }
        (void)entry.getValue(value);
        return OK_VALUE;
      }

      /// @brief Sets the value in the dictionary
      /// @param index identifies the dictionary entry corresponding to this field
      /// @param value is the new value for the dictionary entry
      /// @param length is the length of the string pointed to by value
      DictionaryStatus getDictionaryValue(size_t index, const unsigned char *& value, size_t &length)
      {
        if(index > indexedDictionarySize_)
        {
          throw TemplateDefinitionError("Illegal dictionary index.");
        }
        Value & entry = indexedDictionary_[index];
        if(!entry.isDefined())
        {
          return UNDEFINED_VALUE;
        }
        if(entry.isNull())
        {
          return NULL_VALUE;
        }
        (void)entry.getValue(value, length);
        return OK_VALUE;
      }

      /// @brief Report a warning
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      virtual void reportWarning(const std::string & errorCode, const std::string & message);

      /// @brief Report a warning
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @param identity identifies the field being Xcoded
      virtual void reportWarning(
        const std::string & errorCode,
        const std::string & message,
        const Messages::FieldIdentity  & identity);

      /// @brief Report a warning
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @param name identifies the field being Xcoded
      virtual void reportWarning(
        const std::string & errorCode,
        const std::string & message,
        const std::string & name);

      /// @brief Report a recoverable error
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @throws EncodingError unless overridden.
      virtual void reportError(const std::string & errorCode, const std::string & message);

      /// @brief Report a recoverable error
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @param identity identifies the field being Xcoded
      /// @throws EncodingError unless overridden.
      virtual void reportError(
        const std::string & errorCode,
        const std::string & message,
        const Messages::FieldIdentity & identity
        );

      /// @brief Report a recoverable error
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @param name identifies the field being Xcoded
      /// @throws EncodingError unless overridden.
      virtual void reportError(
        const std::string & errorCode,
        const std::string & message,
        const std::string & name
        );

      /// @brief Report a fatal error (always throws)
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @throws EncodingError "always"
      virtual void reportFatal(const std::string & errorCode, const std::string & message);

      /// @brief Report a fatal error (always throws)
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @param identity identifies the field being Xcoded
      /// @throws EncodingError "always"
      virtual void reportFatal(
        const std::string & errorCode,
        const std::string & message,
        const Messages::FieldIdentity & identity
        );

      /// @brief Report a fatal error (always throws)
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @param name identifies the field being Xcoded
      /// @throws EncodingError "always"
      virtual void reportFatal(
        const std::string & errorCode,
        const std::string & message,
        const std::string & name
        );

      /// @brief get a working buffer for use during Xcoding.
      WorkingBuffer & getWorkingBuffer()
      {
        return workingBuffer_;
      }

      /// @brief Enable some debugging/diagnostic information to be written to an ostream
      /// @param out the ostream to receive the data
      void setVerboseOutput(std::ostream & out)
      {
        verboseOut_ = &out;
      }

      /// @brief Clear the verbose output that was enabled by setVerboseOutput()
      void disableVerboseOutput()
      {
        verboseOut_ = 0;
      }

      /// @brief for "internal" use only.
      /// @returns a pointer to the verbose output stream or zero if disabled
      std::ostream * getVerboseOut()const
      {
        return verboseOut_;
      }

      /// @brief enable logging to the supplied ostream
      /// @todo: do we want a fancier logger?
      /// @param out the ostream to which the log info will be written
      void setLogOutput(std::ostream & out)
      {
        logOut_ = &out;
      }

      /// @brief for internal use: get the logger stream
      /// @returns a pointer to the logger stream (zero means none)
      std::ostream * getLogOut()const
      {
        return logOut_;
      }

      /// @brief Write a message to the log
      /// @param message to be written
      void logMessage(const std::string & message);

      /// @brief Take exception if the templateRegistry hasn't been defined.
      /// A helper routine -- produces a more meaningful error message.
      void registryIsRequired() const
      {
        if(! templateRegistry_)
        {
          throw UsageError("Coding error", "Template Registry not defined when needed.");
        }
      }

    private:
      Context(const Context &);
      Context & operator = (const Context &);

    protected:
      /// if an ostream is supplied make the Xcoder noisy
      std::ostream * verboseOut_;

      /// if an ostream is supplied log unusual Xcoder events
      std::ostream * logOut_;

      /// The template registry to be used during this Xcoding process
      TemplateRegistryCPtr templateRegistry_;
      /// The ID of the currently active template
      template_id_t templateId_;

      /// false makes the Xcoder more forgiving
      bool strict_;
    private:
      size_t indexedDictionarySize_;
      typedef boost::scoped_array<Value> IndexedDictionary;
      IndexedDictionary indexedDictionary_;
      WorkingBuffer workingBuffer_;
    };
  }
}
#endif // CONTEXT_H
