// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONBYTEVECTOR_H
#define FIELDINSTRUCTIONBYTEVECTOR_H
#include <Codecs/FieldInstructionBlob.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Implement &lt;byteVector> field instruction.
    class QuickFAST_Export FieldInstructionByteVector : public FieldInstructionBlob
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionByteVector(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionByteVector();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionByteVector();

    private:
      virtual Messages::FieldCPtr createField(const uchar * buffer, size_t length)const;
      virtual Messages::FieldCPtr createField(const std::string & value)const;
      virtual Messages::FieldCPtr createEmptyField()const;
    };
  }
}
#endif // FIELDINSTRUCTIONBYTEVECTOR_H
