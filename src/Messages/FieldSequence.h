// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDSEQUENCE_H
#define FIELDSEQUENCE_H
#include <Messages/Field.h>
#include <Messages/FieldSet_fwd.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing an ordered sequence of groups
    ///
    /// In the XML template file this field is described as &lt;sequence>
    class QuickFAST_Export FieldSequence : public Field{
      /// @brief Construct a field given a sequence for it to contain
      FieldSequence(Messages::SequenceCPtr sequence);
      /// @brief a typical virtual destructor.
      virtual ~FieldSequence();
    public:
      /// @brief Construct a field given a sequence for it to contain
      /// @param sequence the entries for this FieldSequence
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(Messages::SequenceCPtr sequence);
      /// @brief Construct a NULL field (not an empty string)
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief Set the sequence directly(for debugging)
//      void setSequence(Messages::SequenceCPtr sequence);

      // implement selected virtual methods from Field
      virtual uint32 toUInt32()const;
      virtual const Messages::SequenceCPtr & toSequence() const;
      virtual bool operator == (const Field & rhs)const;
      virtual void valueToStringBuffer()const;
    private:
      Messages::SequenceCPtr sequence_;
    };
  }
}
#endif // FIELDINT64_H
