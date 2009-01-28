// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef CONTEXT_H
#define CONTEXT_H
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Common/WorkingBuffer.h>
#include <Codecs/Dictionary_fwd.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Codecs/Template_fwd.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Context in which Encode/Decode takes place.
    class QuickFAST_Export Context
    {
    public:
      /// @brief Construct with a TemplateRegistry containing all templates to be used.
      /// @param registry A registry containing all templates to be used to decode messages.
      Context(Codecs::TemplateRegistryCPtr registry);

      /// @brief a typical virtual destructor.
      virtual ~Context();

      /// @brief Reset decoding state to initial conditions
      void reset();

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
      Codecs::TemplateRegistryCPtr getTemplateRegistry()const
      {
        return templateRegistry_;
      }

      /// @brief Find a template in the TemplateRepository used by this Context
      /// @brief name of the template being sought
      /// @brief nameSpace that qualifies name
      /// @brief result will point to the template that was found
      /// @returns true if successful
      bool findTemplate(const std::string & name, const std::string & nameSpace, TemplateCPtr & result) const;

      //////////////////////////////
      // Support for decoding fields
      /// @brief Find global dictionary
      /// @returns a pointer to the dictionary
      const DictionaryPtr & getGlobalDictionary()const;
      /// @brief Find dictionary associated with the current template
      /// @returns a pointer to the dictionary
      const DictionaryPtr & getTemplateDictionary()const;
      /// @brief Find dictionary by application type
      /// @param appName the application type to identify the dictionary
      /// @returns a pointer to the dictionary
      const DictionaryPtr & getTypeDictionary(const std::string & appName);
      /// @brief Find dictionary by user defined name
      /// @param name the user defined name to identify the dictionary
      /// @returns a pointer to the dictionary
      const DictionaryPtr & getNamedDictionary(const std::string & name);

      /// @brief Report a recoverable error
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      virtual void reportWarning(const std::string & errorCode, const std::string & message);
      /// @brief Report a recoverable error
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @throws EncodingError unless overridden.
      virtual void reportError(const std::string & errorCode, const std::string & message);
      /// @brief Report a fatal error (always throws)
      /// @param errorCode as defined in the FIX standard (or invented for QuickFAST)
      ///                  i.e [R123]
      /// @param message a text description of the problem.
      /// @throws EncodingError "always"
      virtual void reportFatal(const std::string & errorCode, const std::string & message);

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

    private:
      Context();
      Context(const Context &);
      Context & operator = (const Context &);

    protected:
      /// if an ostream is supplied make the Xcoder noisy
      std::ostream * verboseOut_;

      /// if an ostream is supplied log unusual Xcoder events
      std::ostream * logOut_;

    private:
      Codecs::TemplateRegistryCPtr templateRegistry_;
      template_id_t templateId_;
      DictionaryPtr globalDictionary_;
      DictionaryPtr templateDictionary_;

      typedef std::map<std::string, DictionaryPtr> NamedDictionaryMap;
      NamedDictionaryMap namedDictionaries_;
      NamedDictionaryMap typeDictionaries_;

      typedef std::map<template_id_t, DictionaryPtr> TemplateDictionaryMap;
      TemplateDictionaryMap templateDictionaries_;
      WorkingBuffer workingBuffer_;
    };
  }
}
#endif // CONTEXT_H
