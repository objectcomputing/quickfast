// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifndef DNDECODERCONNECTIONIMPL_H
#define DNDECODERCONNECTIONIMPL_H
#include <Application/DecoderConnection_fwd.h>
#include <Common/QuickFAST_Export.h>

#include <Application/DecoderConfiguration_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>
namespace QuickFAST
{
  namespace DotNet
  {
    class QuickFAST_Export DNDecoderConnectionImpl
    {
    public:
      DNDecoderConnectionImpl();
      ~DNDecoderConnectionImpl();

      void run(
        Messages::ValueMessageBuilder & builder,
        size_t extraThreadCount,
        bool useThisThread
        );

      void run(
        Messages::ValueMessageBuilder & builder,
        const unsigned char * pBuffer,
        long byteOffset,
        long bytesUsed,
        bool reset);

      void setHead(unsigned int value);

      void setReset(bool value);


      void setStrict(bool value);

      void setTemplateFileName(const std::string & value);

      void setFastFileName(const std::string & value);

      void setVerboseFileName(const std::string & value);

      void setPcapFileName(const std::string & value);

      void setEchoFileName(const std::string & value);

      void setEchoType(Application::DecoderConfigurationEnums::EchoType type);

      void setEchoMessage(bool value);

      void setEchoField(bool value);

      void setPcapWordSize(unsigned int value);

      void setHeaderType(Application::DecoderConfigurationEnums::HeaderType value);

      void setHeaderMessageSizeBytes(unsigned int value);

      void setHeaderBigEndian(bool value);

      void setHeaderPrefixCount(unsigned int value);

      void setHeaderSuffixCount(unsigned int value);

      void setAssemblerType(Application::DecoderConfigurationEnums::AssemblerType value);

      void setWaitForCompleteMessage(bool value);

      void setReceiverType(Application::DecoderConfigurationEnums::ReceiverType value);

      void setMulticastGroupIP(const std::string & value);

      void setPortNumber(unsigned short value);

      void setListenInterfaceIP(const std::string & value);

      void setHostName(const std::string & value);

      void setPortName(const std::string & value);

      void setBufferSize(unsigned int value);

      void setBufferCount(unsigned int value);
    private:
      Application::DecoderConfiguration * configuration_;
      Application::DecoderConnection * connection_;

    };

  }
}
#endif // DNDECODERCONNECTIONIMPL_H
