// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "Dictionary_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/Field_fwd.h>

#include <boost/unordered_map.hpp>
//#include <boost/hash_map.hpp>

namespace QuickFAST{
  namespace Codecs{
    /// @brief A dictionary as defined in the FAST standard.
    ///
    /// maps a name (a character string) to a value (a Field)
    /// Values are immutable.  They may be replaced, but not changed <i>in situ</i>
    class QuickFAST_Export Dictionary{
    public:
      /// @brief expose enough implementation to allow iteration
//      typedef std::map<std::string, Messages::FieldCPtr> DictionaryImplementation;
      typedef boost::unordered_map<std::string, Messages::FieldCPtr> DictionaryImplementation;
//      typedef boost::hash_map<std::string, Messages::FieldCPtr> DictionaryImplementation;

        /// @brief expose enough implementation to allow iteration
      typedef DictionaryImplementation::iterator iterator;
      /// @brief expose enough implementation to allow iteration
      typedef DictionaryImplementation::const_iterator const_iterator;
    public:
      /// @brief construct an empty dictionary.
      Dictionary();
      /// @brief destroy the dictionary
      ~Dictionary();
      /// @brief Support iteration
      /// @returns an iterator to the first entry in the dictionary.
      iterator begin()
      {
        return dictionary_.begin();
      }
      /// @brief Support iteration
      /// @returns an iterator to the next entry after the dictionary.
      iterator end()
      {
        return dictionary_.end();
      }
      /// @brief Support const iteration
      /// @returns an const iterator to the first entry in the dictionary.
      const_iterator begin()const
      {
        return dictionary_.begin();
      }
      /// @brief Support const iteration
      /// @returns an iterator to the next entry after the dictionary.
      const_iterator end()const
      {
        return dictionary_.end();
      }

      /// @brief Add an entry or replace an existing entry.
      /// @param name to identify the entry
      /// @param field containing the type and value of the entry.
      void add(const std::string & name, Messages::FieldCPtr field);

      /// @brief Find and existing entry in the dictionary
      /// @param name of the entry being sought
      /// @param[out] field returns a pointer to the entry found
      /// @returns true if the entry was found.
      bool find(const std::string & name, Messages::FieldCPtr & field)const;
    private:
      DictionaryImplementation dictionary_;
    };
  }
}

#endif // DICTIONARY_H
