// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/Field.h>
#include <DotNet/FieldSet.h>
#include <DotNet/Sequence.h>
#include <DotNet/Decimal.h>
#include <DotNet/StlDotNet.h>
#include <DotNet/Exceptions.h>
#include <DotNet/ManagedHelpers.h>

#include "Messages/FieldAscii.h"
#include "Messages/FieldByteVector.h"
#include "Messages/FieldDecimal.h"
#include "Messages/FieldGroup.h"
#include "Messages/FieldInt32.h"
#include "Messages/FieldInt64.h"
#include "Messages/FieldUInt32.h"
#include "Messages/FieldUInt64.h"
#include "Messages/FieldSequence.h"
#include "Messages/FieldSet.h"
#include "Messages/FieldUtf8.h"
#include "Messages/FieldInt8.h"
#include "Messages/FieldUInt8.h"
#include "Messages/FieldInt16.h"
#include "Messages/FieldUInt16.h"
#include "Messages/FieldBitmap.h"

using namespace QuickFASTDotNet::Messages;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Text;
using namespace cli;

namespace QuickFASTDotNet{
  namespace Messages{

    QuickFAST::Messages::FieldCPtr cast_field(QuickFASTDotNet::Messages::Field^ field)
    {
      typedef QuickFAST::Messages::FieldCPtr FTFieldCPtr;
      FTFieldCPtr retVal;
      switch(field->FieldType)
      {
      case FieldType::AsciString:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldAscii::create(StlDotNet::string_cast<std::string>(safe_cast<AsciStringField^>(field)->Value)));
        break;
      case FieldType::ByteVector:
        {
          ByteVectorField^ bvField = safe_cast<ByteVectorField^>(field);
          int bufferLen = bvField->Value->Length;
          QuickFAST::uchar* buffer = new QuickFAST::uchar[bufferLen];
          Marshal::Copy(bvField->Value, 0, System::IntPtr(buffer), bufferLen);
          retVal = FTFieldCPtr(QuickFAST::Messages::FieldByteVector::create(buffer, bufferLen));
        }
        break;
      case FieldType::Decimal:
        {
          DecimalField^ dField = safe_cast<DecimalField^>(field);
          retVal = FTFieldCPtr(QuickFAST::Messages::FieldDecimal::create(QuickFAST::Decimal(dField->Value.Mantissa, dField->Value.Exponent)));
        }
        break;
      case FieldType::Group:
        {
          GroupField^ gField = safe_cast<GroupField^>(field);
          retVal  = FTFieldCPtr(QuickFAST::Messages::FieldGroup::create(QuickFAST::Messages::GroupCPtr(new QuickFAST::Messages::Group(gField->Value->FieldSetRef))));
          //retVal = FTFieldCPtr(new QuickFAST::Messages::FieldGroup(safe_cast<GroupField^>(field)->Value->FieldSetPtr.GetBoostPtr()));
        }
        break;
      case FieldType::Int32:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldInt32::create(safe_cast<Int32Field^>(field)->Value));
        break;
      case FieldType::Int64:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldInt64::create(safe_cast<Int64Field^>(field)->Value));
        break;
      case FieldType::UInt32:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldUInt32::create(safe_cast<UInt32Field^>(field)->Value));
        break;
      case FieldType::UInt64:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldUInt64::create(safe_cast<UInt64Field^>(field)->Value));
        break;
      case FieldType::Sequence:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldSequence::create(QuickFAST::Messages::SequenceCPtr(new QuickFAST::Messages::Sequence(safe_cast<SequenceField^>(field)->Value->Ref))));
        //retVal = FTFieldCPtr(new QuickFAST::Messages::FieldSequence(safe_cast<SequenceField^>(field)->Value->BoostPtr));
        break;
      case FieldType::UnicodeString:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldUtf8::create(StlDotNet::string_cast<std::string>(safe_cast<UnicodeStringField^>(field)->Value)));
        break;
      case FieldType::Int8:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldInt8::create(safe_cast<Int8Field^>(field)->Value));
        break;
      case FieldType::UInt8:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldUInt8::create(safe_cast<UInt8Field^>(field)->Value));
        break;
      case FieldType::Int16:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldInt16::create(safe_cast<Int16Field^>(field)->Value));
        break;
      case FieldType::UInt16:
        retVal = FTFieldCPtr(QuickFAST::Messages::FieldUInt16::create(safe_cast<UInt16Field^>(field)->Value));
        break;
      case FieldType::Bitmap:
        {
          BitmapField^ bmField = safe_cast<BitmapField^>(field);
          int bufferLen = bmField->Value->Length;
          QuickFAST::uchar* buffer = new QuickFAST::uchar[bufferLen];
          Marshal::Copy(bmField->Value, 0, System::IntPtr(buffer), bufferLen);
          retVal = FTFieldCPtr(QuickFAST::Messages::FieldBitMap::create(buffer, bufferLen));
        }
        break;
      default:
        throw gcnew UnexpectedError();
        ;
      };
      return retVal;
    }

    QuickFASTDotNet::Messages::Field^ cast_field(QuickFAST::Messages::FieldCPtr field)
    {
      Field^ retVal;

      switch(field->getType())
      {
      case QuickFAST::Messages::Field::INT32:
        retVal = gcnew Int32Field(field->toInt32());
        break;
      case QuickFAST::Messages::Field::UINT32:
        retVal = gcnew UInt32Field(field->toUInt32());
        break;
      case QuickFAST::Messages::Field::INT64:
        retVal = gcnew Int64Field(field->toInt64());
        break;
      case QuickFAST::Messages::Field::UINT64:
        retVal = gcnew UInt64Field(field->toUInt64());
        break;
      case QuickFAST::Messages::Field::DECIMAL:
        retVal = gcnew DecimalField(Decimal(field->toDecimal()));
        break;
      case QuickFAST::Messages::Field::ASCII:
        retVal = gcnew AsciStringField(StlDotNet::string_cast(field->toAscii()));
        break;
      case QuickFAST::Messages::Field::UTF8:
        retVal = gcnew UnicodeStringField(StlDotNet::string_cast(field->toUtf8()));
        break;
      case QuickFAST::Messages::Field::BYTEVECTOR:
        retVal = gcnew ByteVectorField(Encoding::ASCII->GetBytes(StlDotNet::string_cast(field->toByteVector())));
        break;
      case QuickFAST::Messages::Field::SEQUENCE:
        retVal = gcnew SequenceField(gcnew Sequence(*field->toSequence()));
        break;
      case QuickFAST::Messages::Field::GROUP:
        retVal = gcnew GroupField(gcnew FieldSet(*field->toGroup()));
        break;
      case QuickFAST::Messages::Field::INT8:
        retVal = gcnew Int8Field(field->toInt8());
        break;
      case QuickFAST::Messages::Field::UINT8:
        retVal = gcnew UInt8Field(field->toUInt8());
        break;
      case QuickFAST::Messages::Field::INT16:
        retVal = gcnew Int16Field(field->toInt16());
        break;
      case QuickFAST::Messages::Field::UINT16:
        retVal = gcnew UInt16Field(field->toUInt16());
        break;
      case QuickFAST::Messages::Field::BITMAP:
        retVal = gcnew UInt16Field(field->toUInt16());
        break;
      default:
        throw gcnew UnexpectedError();
      };

      return retVal;
    }
  }
}
