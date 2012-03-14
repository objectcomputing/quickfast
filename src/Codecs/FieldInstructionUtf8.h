// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONUTF8_H
#define FIELDINSTRUCTIONUTF8_H
#include <Codecs/FieldInstructionBlob.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Implement &lt;string charset="unicode"> field instruction.
    class QuickFAST_Export FieldInstructionUtf8 : public FieldInstructionBlob
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionUtf8(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionUtf8();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionUtf8();

    private:
      virtual Messages::FieldCPtr createField(
        const uchar * buffer,
        size_t length)const;
      virtual Messages::FieldCPtr createField(
        const std::string & value)const;
      virtual Messages::FieldCPtr createEmptyField()const;
    };
  }
}
#endif // FIELDINSTRUCTIONUTF8_H
