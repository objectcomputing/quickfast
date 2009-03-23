// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include "Decimal.h"
#include "Sequence.h"
#include "FieldSet.h"


namespace QuickFASTDotNet{

  value struct Decimal;

  namespace Messages{

    ref class FieldSet;
    ref class Sequence;

    public enum struct FieldType
    {
      Int32,
      Int64,
      UInt32,
      UInt64,
      Decimal,
      AsciString,
      UnicodeString,
      ByteVector,
      Sequence,
      Group,
      Int8,     // not FAST Standard
      UInt8,    // not FAST Standard
      Int16,    // not FAST Standard
      UInt16,   // not FAST Standard
      Bitmap    // not FAST Standard
    };

    /// @brief Field interface.  It has a single property which returns the type of
    /// the field.
    public interface class Field
    {
    public:
      property FieldType FieldType
      {
        QuickFASTDotNet::Messages::FieldType get();
      }
    };


    /// @brief A field containing a signed 32 bit integer
    ///
    /// In the XML template file this field is described as &lt;int32>
    public ref class Int32Field: public Field
    {
    public:
      Int32Field(int value): value_(value){}
      property int Value {
        int get() { return value_;}
        void set(int rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::Int32;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        Int32Field^ castedObj = safe_cast<Int32Field^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_ == value_;
        }

        return false;
      }

    private:
      int value_;
    };


    /// @brief A field containing an unsigned 64 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint64>
    public ref class Int64Field: public Field
    {
    public:
      Int64Field(__int64 value): value_(value){}
      property __int64 Value {
        __int64 get() { return value_;}
        void set(__int64 rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::Int64;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        Int64Field^ castedObj = safe_cast<Int64Field^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_ == value_;
        }

        return false;
      }

    private:
      __int64 value_;
    };


    /// @brief A field containing an unsigned 32 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint32>
    public ref class UInt32Field: public Field
    {
    public:
      UInt32Field(unsigned int value): value_(value){}
      property unsigned int Value {
        unsigned int get() { return value_;}
        void set(unsigned int rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::UInt32;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        UInt32Field^ castedObj = safe_cast<UInt32Field^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_ == value_;
        }

        return false;
      }

    private:
      unsigned int value_;
    };


    /// @brief A field containing an unsigned 64 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint64>
    public ref class UInt64Field: public Field
    {
    public:
      UInt64Field(unsigned __int64 value): value_(value){}
      property unsigned __int64 Value {
        unsigned __int64 get() { return value_;}
        void set(unsigned __int64 rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::UInt64;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        UInt64Field^ castedObj = safe_cast<UInt64Field^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_ == value_;
        }

        return false;
      }

    private:
      unsigned __int64 value_;
    };


    /// @brief A field containing a scaled decimal number
    ///
    /// In the XML template file this field is described as &lt;decimal>
    public ref class DecimalField: public Field
    {
    public:
      DecimalField(Decimal value): value_(value){}
      property Decimal Value {
        Decimal get() { return value_;}
        void set(Decimal rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::Decimal;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        DecimalField^ castedObj = safe_cast<DecimalField^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_.Equals(value_);
        }

        return false;
      }

    private:
      Decimal value_;
    };


    /// @brief A field containing ascii data.
    ///
    /// In the XML template file this field is described as &lt;string charset="ascii">
    public ref class AsciStringField: public Field
    {
    public:
      AsciStringField(System::String^ value): value_(value){}
      property System::String^ Value {
        System::String^ get() { return value_;}
        void set(System::String^ rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::AsciString;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        AsciStringField^ castedObj = safe_cast<AsciStringField^>(obj);
        if(castedObj != nullptr && value_ != nullptr)
        {
          return castedObj->value_->Equals(value_);
        }

        return false;
      }

    private:
      System::String^ value_;
    };


    /// @brief A field containing unicode data.
    ///
    /// In the XML template file this field is described as &lt;string charset="unicode">
    public ref class UnicodeStringField: public Field
    {
    public:
      UnicodeStringField(System::String^ value): value_(value){}
      property System::String^ Value {
        System::String^ get() { return value_;}
        void set(System::String^ rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::UnicodeString;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        UnicodeStringField^ castedObj = safe_cast<UnicodeStringField^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_->Equals(value_);
        }

        return false;
      }

    private:
      System::String^ value_;
    };

    /// @brief A field containing arbitrary binary data
    ///
    /// In the XML template file this field is described as &lt;byteVector>
    public ref class ByteVectorField: public Field
    {
    public:
      ByteVectorField(cli::array<unsigned char>^ value): value_(value){}
      property cli::array<unsigned char>^ Value {
        cli::array<unsigned char>^ get() { return value_;}
        void set(cli::array<unsigned char>^ rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::ByteVector;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        ByteVectorField^ castedObj = safe_cast<ByteVectorField^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_->Equals(value_);
        }

        return false;
      }

    private:
      cli::array<unsigned char>^ value_;
    };

    /// @brief A field containing a subgroup of fields
    ///
    /// In the XML template file this field is described as &lt;group>
    public ref class GroupField: public Field
    {
    public:
      GroupField(FieldSet^ value): value_(value){}
      property FieldSet^ Value {
        FieldSet^ get() { return value_;}
        void set(FieldSet^ rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::Group;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        GroupField^ castedObj = safe_cast<GroupField^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_->Equals(value_);
        }

        return false;
      }

    private:
      FieldSet^ value_;
    };

    /// @brief A field containing an ordered sequence of groups
    ///
    /// In the XML template file this field is described as &lt;sequence>
    public ref class SequenceField: public Field
    {
    public:
      SequenceField(Sequence^ value): value_(value){}
      property Sequence^ Value {
        Sequence^ get() { return value_;}
        void set(Sequence^ rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::Sequence;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        SequenceField^ castedObj = safe_cast<SequenceField^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_->Equals(value_);
        }

        return false;
      }

    private:
      Sequence^ value_;
    };




    /// @brief A field containing a signed 8 bit integer
    ///
    /// In the XML template file this field is described as &lt;int8>
    public ref class Int8Field: public Field
    {
    public:
      Int8Field(char value): value_(value){}
      property char Value {
        char get() { return value_;}
        void set(char rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::Int8;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        Int8Field^ castedObj = safe_cast<Int8Field^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_ == value_;
        }

        return false;
      }

    private:
      char value_;
    };


    /// @brief A field containing an unsigned 8 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint8>
    public ref class UInt8Field: public Field
    {
    public:
      UInt8Field(unsigned char value): value_(value){}
      property unsigned char Value {
        unsigned char get() { return value_;}
        void set(unsigned char rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::UInt8;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        UInt8Field^ castedObj = safe_cast<UInt8Field^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_ == value_;
        }

        return false;
      }

    private:
      unsigned char value_;
    };


    /// @brief A field containing a signed 16 bit integer
    ///
    /// In the XML template file this field is described as &lt;int8>
    public ref class Int16Field: public Field
    {
    public:
      Int16Field(short value): value_(value){}
      property short Value {
        short get() { return value_;}
        void set(short rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::Int16;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        Int16Field^ castedObj = safe_cast<Int16Field^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_ == value_;
        }

        return false;
      }

    private:
      short value_;
    };


    /// @brief A field containing an unsigned 16 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint16>
    public ref class UInt16Field: public Field
    {
    public:
      UInt16Field(unsigned short value): value_(value){}
      property unsigned short Value {
        unsigned short get() { return value_;}
        void set(unsigned short rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::UInt16;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        UInt16Field^ castedObj = safe_cast<UInt16Field^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_ == value_;
        }

        return false;
      }

    private:
      unsigned short value_;
    };


    /// @brief A field containing an unsigned 16 bit integer
    ///
    /// In the XML template file this field is described as &lt;bitmap>
    public ref class BitmapField: public Field
    {
    public:
      BitmapField(cli::array<unsigned char>^ value): value_(value){}
      property cli::array<unsigned char>^ Value {
        cli::array<unsigned char>^ get() { return value_;}
        void set(cli::array<unsigned char>^ rhs) { value_ = rhs; }
      }

      property QuickFASTDotNet::Messages::FieldType FieldType {
        virtual QuickFASTDotNet::Messages::FieldType get() = QuickFASTDotNet::Messages::Field::FieldType::get {
          return QuickFASTDotNet::Messages::FieldType::Bitmap;
        }
      }

      virtual System::String^ ToString() override
      {
        return System::String::Format("FieldType: {0}, Value: {1}", this->FieldType, System::Convert::ToString(value_));
      }

      virtual bool Equals(System::Object^ obj) override
      {
        BitmapField^ castedObj = safe_cast<BitmapField^>(obj);
        if(castedObj != nullptr)
        {
          return castedObj->value_->Equals(value_);
        }

        return false;
      }

    private:
      cli::array<unsigned char>^ value_;
    };


    //Helpers to convert between managed and unmanaged fields
    QuickFAST::Messages::FieldCPtr cast_field(QuickFASTDotNet::Messages::Field^ field);
    QuickFASTDotNet::Messages::Field^ cast_field(QuickFAST::Messages::FieldCPtr field);

  }
}
