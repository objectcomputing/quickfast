// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <Codecs/DataDestination.h>

namespace QuickFASTDotNet{
  namespace Codecs{

#pragma unmanaged

    class DestinationBufferImpl
      : public QuickFAST::Codecs::DestinationBuffer
    {
    public:
      typedef std::vector<QuickFAST::uchar> TBuffer;
      typedef TBuffer::iterator TBufferIterator;

      /// @brief Put the next byte into the buffer
      /// @param[in] byte to put.
      virtual void putByte(QuickFAST::uchar byte)
      {
            buffer_.push_back(byte);
      };

      /// @brief Reserve space
      /// @param size the number of bytes expected into this buffer
      virtual void reserve(size_t size)
      {
        buffer_.reserve(size);
      }

      size_t getSize() const
      {
        return buffer_.size();
      }

      TBufferIterator begin()
      {
        return buffer_.begin();
      }

      TBufferIterator end()
      {
        return buffer_.end();
      }

    private:
      TBuffer buffer_;
    };
#pragma managed



    public class DataDestinationImpl
      : public QuickFAST::Codecs::DataDestination
    {
    public:
      DataDestinationImpl(System::IO::Stream^ outStream)
        : outStream_(outStream)
      {
      }

      virtual void endMessage()
      {
        for(BufferVector::const_iterator it = buffers_.begin();
          it != buffers_.end();
          ++it)
        {
          DestinationBufferImpl * dbs = static_cast<DestinationBufferImpl *>((*it).get());

          size_t size = dbs->getSize();
          array<unsigned char>^ managedArray = gcnew array<unsigned char>(int(size));
          QuickFAST::uchar* pMem = dbs->begin()._Myptr;
          System::Runtime::InteropServices::Marshal::Copy(System::IntPtr(pMem), managedArray, 0, int(size));

          // Needs to catch exceptions.
          //outStream_->Write(managedArray, 0, managedArray->Length);

          // cant delete because its on the managed heap
          //delete managedArray;
        }
      }

    protected:
      /// @brief implementation specfic buffer allocator
      /// @returns a pointer to the newly allocated buffer
      virtual QuickFAST::Codecs::DestinationBufferPtr allocateBuffer()
      {
        return QuickFAST::Codecs::DestinationBufferPtr(new DestinationBufferImpl());
      }

    private:
      gcroot<System::IO::Stream^> outStream_;
    };
  }
}
