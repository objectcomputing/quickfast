// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDGROUP_H
#define FIELDGROUP_H
#include <Messages/Field.h>
#include <Messages/FieldSet_fwd.h>
#include <Messages/MessageBuilder_fwd.h>

namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing a subgroup of fields
    ///
    /// In the XML template file this field is described as &lt;group>
    class QuickFAST_Export FieldGroup : public Field{
      /// @brief construct given a group as the value of the field
      explicit FieldGroup(Messages::GroupCPtr group);
    public:
      /// @brief Construct the field from a Group value
      /// @param group the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(Messages::GroupCPtr group);

      /// @brief a typical virtual destructor.
      virtual ~FieldGroup();

      // implement selected virtual methods from Field
      virtual const Messages::GroupCPtr & toGroup() const;
      virtual bool operator == (const Field & rhs)const;
      virtual void valueToStringBuffer()const;
    private:
      Messages::GroupCPtr group_;
    };
  }
}
#endif // FIELDINT64_H
