// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "Sequence_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/FieldSet.h>

namespace QuickFAST{
  namespace Messages{
    /// Container for a sequence of field groups
    class Sequence
    {
    public:
      /// @brief Each entry is a field set
      typedef FieldSetCPtr Entry;
      /// @brief We store them in a vector
      typedef std::vector<Entry> Entries;
      /// @brief We support iteration through the sequence
      typedef Entries::const_iterator const_iterator;

    public:
      /// @brief construct an empty sequence
      Sequence(
        const Messages::FieldIdentity & lengthFieldIdentity,
        size_t sequenceLength)
        : lengthIdentity_(lengthFieldIdentity)
      {
        this->entries_.reserve(sequenceLength);
      }

      ~Sequence()
      {
      }

      /// @brief get the identity of the sequence's length field
      const Messages::FieldIdentity & getLengthIdentity() const
      {
        return lengthIdentity_;
      }

      /// @brief Set the application data type associated with this sequence.
      void setApplicationType(const std::string & type)
      {
        applicationType_ = type;
      }

      /// @brief Get the application data type associated with this sequence.
      const std::string & getApplicationType()const
      {
        return applicationType_;
      }

      /// @brief Add a new group to the sequence
      void addEntry(FieldSetCPtr fieldSet)
      {
        entries_.push_back(fieldSet);
      }

      /// @brief How many entries are in the sequence.
      size_t size()const
      {
        return entries_.size();
      }

      /// @brief access an entry by index.
      const FieldSetCPtr & operator [](size_t index) const
      {
        return entries_[index];
      }

      /// @brief access an entry by index.
      FieldSetCPtr & operator [](size_t index)
      {
        return entries_[index];
      }

      /// @brief Support iteration from the beginning
      const_iterator begin() const
      {
        return entries_.begin();
      }

      /// @brief Support iteration to the end
      const_iterator end() const
      {
        return entries_.end();
      }

    private:
      Sequence();
      Sequence(const Sequence&);
      Sequence& operator=(const Sequence&);
    private:
      std::string applicationType_;
      const Messages::FieldIdentity & lengthIdentity_;
      Entries entries_;
    };
  }
}

#endif // SEQUENCE_H
